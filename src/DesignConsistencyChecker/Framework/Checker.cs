using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using DesignConsistencyChecker.DesignRule;
using System.Threading.Tasks;
using System.Reflection;
using DesignConsistencyChecker.Models;
using GME.MGA;
using System.Windows.Threading;
using CyPhyCOMInterfaces;
using META;
using GME.CSharp;

namespace DesignConsistencyChecker.Framework
{
    public class Checker
    {
        public MgaFCO Model { get; private set; }
        public MgaProject Project { get; private set; }
        public GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public IMgaTraceability Traceability { get; set; }

        public Checker(MgaFCO model, MgaProject project, IMgaTraceability traceability = null, CyPhyGUIs.GMELogger logger = null)
        {
            Model = model;
            Project = project;
            if (logger != null)
            {
                this.Logger = logger;
            }
            else
            {
                GMEConsole = GMEConsole.CreateFromProject(project);
            }

            registeredRules = new ConcurrentDictionary<string, List<RuleDescriptor>>();

            if (traceability == null)
            {
                this.Traceability = new META.MgaTraceability();
            }
            else
            {
                this.Traceability = traceability;
            }
        }

        // DO NOT use ConcurrentBag it is buggy (slooooow) in 4.0
        private ConcurrentDictionary<string, List<RuleDescriptor>> registeredRules;

        #region Register rules

        /// <summary>
        /// Loads rule DLL files and instantiates all classes which implements IMgaRule. Synchron version.
        /// </summary>
        /// <param name="dllPaths">
        ///  - Filename list (if the ruledll is in the same directory)
        /// <example>exampleRule.dll</example>
        ///  - Filenames with ABSOLUTE path.
        /// <example>c:\exampleRule.dll</example>
        /// </param>
        public void RegisterRuleDlls(IEnumerable<string> dllPaths)
        {
            // Filter for duplicate files
            var hs = new HashSet<string>(dllPaths);

            // Load rules in parallel way
            // Parallel.ForEach(hs, dllPath =>
            foreach (var dllPath in hs)
            {
                var fi = new FileInfo(dllPath);
                if (!fi.Exists)
                {
                    // try in the META_PATH
                    var fulldllPath = Path.Combine(VersionInfo.MetaPath, "bin", dllPath);
                    fi = new FileInfo(fulldllPath);


                    // try current directory
                    if (!fi.Exists)
                    {
                        fulldllPath = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), dllPath);
                        fi = new FileInfo(fulldllPath);
                    }

                    if (!fi.Exists)
                        throw new FileNotFoundException("Rule Dll is missing!", fi.FullName);
                }

                // ZL: Using LoadFrom while LoadFile is deprecated
                //     AND dll's location is not in the search path 
                //     i.e. will NOT load the dll if it has dependencies.
                var asm = Assembly.LoadFrom(fi.FullName);
                string assemblyName = fi.FullName;
                RegisterRulesForAssembly(asm, assemblyName);
            }//);
            OnRulesRegistered();
        }

        public void RegisterRulesForAssembly(Assembly asm, string name)
        {
            var types = asm.GetTypes();
            RegisterRulesForTypes(types, name);
        }

        public void RegisterRulesForTypes(IEnumerable<Type> types, string name)
        {
            var ruleBase = typeof(RuleBase);
            var rules = types.Where(p => ruleBase.IsAssignableFrom(p) && p.IsClass && !p.IsAbstract).Select(x => (RuleBase)Activator.CreateInstance(x));
            bool added = registeredRules.TryAdd(name, rules.SelectMany(x => x.Rules).ToList());

            if (added == false)
            {
                throw new ArgumentException("Duplicate name '" + name + "'");
            }
        }

        public void RegisterRuleDll(string dllPath)
        {
            RegisterRuleDlls(new List<string> { dllPath });
        }

        /// <summary>
        /// Cancels the "RegisterRuleDll" function when started with BeginRegisterRuleDlls
        /// </summary>
        public CancellationTokenSource RegisterCancel { get; private set; }

        /// <summary>
        /// Loads rule DLL files and instantiates all classes which implements IMgaRule. Async version.
        /// Use RegisterCancel token source to cancel the execution.
        /// RolesRegistered event is fired when finished if successful.
        /// </summary>
        /// <param name="dllPaths">
        ///  - Filename list (if the ruledll is in the same directory)
        /// <example>exampleRule.dll</example>
        ///  - Filenames with ABSOLUTE path.
        /// <example>c:\exampleRule.dll</example>
        /// </param>
        public Task BeginRegisterRuleDlls(IEnumerable<string> dllPaths)
        {
            RegisterCancel = new CancellationTokenSource();
            return Task.Factory.StartNew(o => RegisterRuleDlls(dllPaths), RegisterCancel);
        }

        public event EventHandler RulesRegistered;
        protected virtual void OnRulesRegistered()
        {
            EventHandler handler = RulesRegistered;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        #endregion

        #region Execution



        public void RefreshChildrenCache()
        {
            if (Model != null)
            {
                _allChildrenCache = GetAllChildren(Model as MgaModel);

                List<MgaFCO> list = null;

                if (_allChildrenCache.TryGetValue(Model.Meta.Name, out list) == false)
                {
                    list = new List<MgaFCO>();
                    _allChildrenCache.Add(Model.Meta.Name, list);
                }

                list.Add(Model);
            }
        }

        public void CheckRule(RuleDescriptor selectedRule, out List<RuleFeedbackBase> ruleFeedbacks, out List<CheckerFeedback> checkerFeedbacks)
        {
            CheckRules(new List<RuleDescriptor> { selectedRule }, out ruleFeedbacks, out checkerFeedbacks);
        }

        private Dictionary<string, List<MgaFCO>> _allChildrenCache = new Dictionary<string, List<MgaFCO>>();
        public void CheckRules(IEnumerable<RuleDescriptor> selectedRules, out List<RuleFeedbackBase> ruleFeedbacks, out List<CheckerFeedback> checkerFeedbacks)
        {
            //if (!_allChildrenCache.Any())
            //{
            //    RefreshChildrenCache();
            //}

            RefreshChildrenCache();

            ruleFeedbacks = new List<RuleFeedbackBase>();
            checkerFeedbacks = new List<CheckerFeedback>();


            IEnumerable<RuleDescriptor> rulesToCheck = null;

            if (this.Model != null)
            {
                rulesToCheck = selectedRules.Where(x => x.RequiresElaboration == false);
            }
            else
            {
                // no model is open elaboration cannot be performed...
                rulesToCheck = selectedRules;
            }

            foreach (var rule in rulesToCheck)
            {
                this.CheckRule(ruleFeedbacks, checkerFeedbacks, rule);
            }

            if (this.Model != null && selectedRules.Any(x => x.RequiresElaboration))
            {
                var terr = this.Project.ActiveTerritory;
                this.Project.CommitTransaction();

                try
                {
                    this.Project.BeginTransaction(terr);
                    Type t = Type.GetTypeFromProgID("Mga.MgaFCOs");
                    MgaFCOs fcos = Activator.CreateInstance(t) as MgaFCOs;

                    if (this.CallElaborator(this.Project, this.Model, fcos, 0))
                    {
                        RefreshChildrenCache();

                        foreach (var rule in selectedRules.Where(x => x.RequiresElaboration))
                        {
                            this.CheckRule(ruleFeedbacks, checkerFeedbacks, rule);
                        }

                        //var collapsed = this.CallElaborator(this.Project, this.Model, fcos, 0, expand: false);
                        //if (collapsed == false)
                        //{
                        //    GMEConsole.Error.WriteLine("Elaborator collapse failed.");
                        //}
                    }
                    else
                    {
                        if (this.Logger != null)
                        {
                            this.Logger.WriteError("Rules below cannot be checked while elaboration failed.");
                        }
                        else
                        {
                            GMEConsole.Error.WriteLine("Rules below cannot be checked while elaboration failed.");
                        }

                        foreach (var rule in selectedRules.Where(x => x.RequiresElaboration))
                        {
                            if (this.Logger != null)
                            {
                                this.Logger.WriteDebug(" - Name: {0} - Description: {1}", rule.Name, rule.Description);
                            }
                            else
                            {
                                GMEConsole.Out.WriteLine(" - Name: {0} - Description: {1}", rule.Name, rule.Description);
                            }
                        }
                    }
                }
                finally
                {
                    this.Project.AbortTransaction();
                }

                this.Project.BeginTransaction(terr);
            }

        }


        private bool CallElaborator(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param,
            bool expand = true)
        {
            try
            {

                // call elaborator and expand the references
                Type t = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyElaborate");
                IMgaComponentEx elaborator = Activator.CreateInstance(t) as IMgaComponentEx;
                elaborator.Initialize(project);

                if (expand)
                {
                    elaborator.ComponentParameter["automated_expand"] = "true";
                }
                else
                {
                    elaborator.ComponentParameter["automated_collapse"] = "true";
                }

                elaborator.ComponentParameter["console_messages"] = "off";

                //GMEConsole.Info.WriteLine("Elaborating model...");
                //System.Windows.Forms.Application.DoEvents();

                elaborator.InvokeEx(project, currentobj, selectedobjs, param);

                CyPhyCOMInterfaces.IMgaTraceability traceability = elaborator.ComponentParameter["traceability"] as CyPhyCOMInterfaces.IMgaTraceability;

                this.Traceability = new META.MgaTraceability();

                if (traceability != null)
                {
                    traceability.CopyTo(this.Traceability);
                }

                //GMEConsole.Info.WriteLine("Elaboration is done.");
                //System.Windows.Forms.Application.DoEvents();

                // TODO: get exception message(s) from elaborator
                string msgs = elaborator.ComponentParameter["exception"] as string;
                if (string.IsNullOrWhiteSpace(msgs) == false)
                {
                    if (this.Logger != null)
                    {
                        this.Logger.WriteError("Elaborator exception occured: {0}", msgs);
                    }
                    else
                    {
                        GMEConsole.Error.WriteLine("Elaborator exception occured: {0}", msgs);
                    }
                    System.Windows.Forms.Application.DoEvents();
                    throw new Exception(msgs);
                }

            }
            catch (Exception ex)
            {
                GMEConsole.Error.WriteLine(ex);
                return false;
            }

            return true;
        }

        private void CheckRule(List<RuleFeedbackBase> ruleFeedbacks, List<CheckerFeedback> checkerFeedbacks, RuleDescriptor rule)
        {
            if (!rule.ValidContext.Any(context => _allChildrenCache.ContainsKey(context)))
                checkerFeedbacks.Add(new CheckerFeedback { CheckerResultType = CheckerResultTypes.InvalidContext });

            var passed = true;

            foreach (var context in rule.ValidContext.Where(context => _allChildrenCache.ContainsKey(context)))
            {
                foreach (var node in _allChildrenCache[context])
                {
                    var feedBacks = rule.CheckRuleDelegate(node).ToList();
                    ruleFeedbacks.AddRange(feedBacks);

                    if ((GMEConsole == null || GMEConsole.Error == null || GMEConsole.Warning == null) && this.Logger == null)
                    {
                        continue;
                    }

                    foreach (var feedBack in feedBacks)
                    {
                        switch (feedBack.FeedbackType)
                        {
                            case FeedbackTypes.Error:
                                if (this.Logger != null)
                                {
                                    this.Logger.WriteFailed(feedBack.Message);
                                    if (feedBack.InvolvedObjectsByRole != null)
                                    {
                                        this.Logger.WriteFailed("Involved Objects : ");
                                        foreach (var fco in feedBack.InvolvedObjectsByRole)
                                        {
                                            if (fco != null)
                                            {
                                                // TODO: turn object references to valid objects using Traceability map
                                                this.Logger.WriteFailed("[ {0} ]", fco.ToMgaHyperLink(this.Traceability));
                                            }
                                            else
                                            {
                                                this.Logger.WriteError("[ Could not get fco from InvolvedObjectsByRole for this error. (fco was null) ]");
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    GMEConsole.Error.WriteLine(feedBack.Message);
                                }

                                passed = false;
                                break;
                            case FeedbackTypes.Warning:
                                if (this.Logger != null)
                                {
                                    this.Logger.WriteWarning(feedBack.Message);
                                    if (feedBack.InvolvedObjectsByRole != null)
                                    {
                                        this.Logger.WriteWarning("Involved Objects : ");
                                        foreach (var fco in feedBack.InvolvedObjectsByRole)
                                        {
                                            if (fco != null)
                                            {
                                                // TODO: turn object references to valid objects using Traceability map
                                                this.Logger.WriteWarning("[ {0} ]", fco.ToMgaHyperLink(this.Traceability));
                                            }
                                            else
                                            {
                                                this.Logger.WriteError("[ Could not get fco from InvolvedObjectsByRole for this error. (fco was null) ]");
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    GMEConsole.Warning.WriteLine(feedBack.Message);
                                }
                                passed = false;
                                break;
                        }

                        //var sb = new StringBuilder();
                        //sb.AppendLine(feedBack.Message);
                        if (this.Logger == null)
                        {
                            if (feedBack.InvolvedObjectsByRole != null && feedBack.InvolvedObjectsByRole.Any())
                            {
                                //sb.AppendLine("- Involved objects: ");

                                GMEConsole.Out.WriteLine("[ Involved objects: ]");
                            }

                            foreach (var fco in feedBack.InvolvedObjectsByRole)
                            {
                                if (fco != null)
                                {

                                    GMEConsole.Out.WriteLine("[ {0} ]", fco.ToMgaHyperLink(this.Traceability));
                                }
                                else
                                {

                                    GMEConsole.Out.WriteLine("[ Could not get fco from InvolvedObjectsByRole for this error. (fco was null) ]");
                                }
                            }
                        }

                    }
                }
            }

            if (passed && rule.ValidContext.Any(context => _allChildrenCache.ContainsKey(context)))
            {
                if (this.Logger != null)
                {
                    this.Logger.WriteSuccess("{0} : {1}", rule.Name, rule.Description);
                }
                else
                {
                    GMEConsole.Info.WriteLine("PASSED: {0} : {1}", rule.Name, rule.Description);
                }
            }
        }


        private Dictionary<string, List<MgaFCO>> GetAllChildren(MgaModel root)
        {
            if (root == null) return new Dictionary<string, List<MgaFCO>>();

            var result = new Dictionary<string, List<MgaFCO>>();
            var models = new List<MgaModel>();

            foreach (var childObject in root.ChildObjects.OfType<MgaFCO>())
            {
                var kind = childObject.Meta.Name;
                if (!result.ContainsKey(kind))
                    result[kind] = new List<MgaFCO>();

                result[kind].Add(childObject);

                if (childObject is MgaModel) models.Add((MgaModel)childObject);
            }

            foreach (var model in models)
            {
                var tmp = GetAllChildren(model);

                foreach (var tmpChild in tmp.Where(tmpChild => tmpChild.Value != null))
                {
                    if (!result.ContainsKey(tmpChild.Key))
                        result[tmpChild.Key] = new List<MgaFCO>();

                    result[tmpChild.Key].AddRange(tmpChild.Value);
                }
            }

            return result;
        }

        #endregion

        public IEnumerable<RuleDescriptor> GetRegisteredRules
        {
            get { return registeredRules.Values.SelectMany(x => x); }
        }


        public IEnumerable<string> AllTags
        {
            get { return new HashSet<string>(GetRegisteredRules.SelectMany(x => x.Tags)); }
        }
    }
}