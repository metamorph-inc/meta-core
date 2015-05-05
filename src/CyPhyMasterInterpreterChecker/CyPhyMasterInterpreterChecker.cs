using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;

namespace CyPhyMasterInterpreterChecker
{
    public static class Extensions
    {
        public static string ToSentenceCase(this string str)
        {
            return System.Text.RegularExpressions.Regex.Replace(str, "[a-z][A-Z]", m => m.Value[0] + " " + char.ToLower(m.Value[1]));
        }

        public static string PadToCenter(this string str, int numChars)
        {
            return string.Format("{0,-" + numChars.ToString() + "}", String.Format("{0," + ((numChars + str.Length) / 2).ToString() + "}", str));
        }

        public static string PadToLeft(this string str, int numChars)
        {
            return string.Format("{0,-" + numChars.ToString() + "}", str);
        }

        public static string PadToRight(this string str, int numChars)
        {
            return string.Format("{0," + numChars.ToString() + "}", str);
        }
    }

    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyMasterInterpreterCheckerInterpreter : IMgaComponentEx, IGMEVersionInfo
    {
        /// <summary>
        /// Contains information about the GUI event that initiated the invocation.
        /// </summary>
        public enum ComponentStartMode
        {
            GME_MAIN_START = 0, 		// Not used by GME
            GME_BROWSER_START = 1,      // Right click in the GME Tree Browser window
            GME_CONTEXT_START = 2,		// Using the context menu by right clicking a model element in the GME modeling window
            GME_EMBEDDED_START = 3,		// Not used by GME
            GME_MENU_START = 16,		// Clicking on the toolbar icon, or using the main menu
            GME_BGCONTEXT_START = 18,	// Using the context menu by right clicking the background of the GME modeling window
            GME_ICON_START = 32,		// Not used by GME
            GME_SILENT_MODE = 128 		// Not used by GME, available to testers not using GME
        }

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            // TODO: Add your initialization code here...            
        }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        GMEConsole GMEConsole { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                GMEConsole = GMEConsole.CreateFromProject(project);
                MgaGateway = new MgaGateway(project);
                project.CreateTerritoryWithoutSink(out MgaGateway.territory);

                this.InteractiveMode = this.Convert(param) != ComponentStartMode.GME_SILENT_MODE;

                //bool runNewImplementationOnly = true;
                //runNewImplementationOnly = false;

                //if (runNewImplementationOnly)
                //{
                //    this.NewMasterInterpreterImplementationFull(currentobj);
                //    return;
                //}

                List<IMgaFCO> objectsToCheck = new List<IMgaFCO>();
                List<IMgaFCO> objectsToGetConfigurations = new List<IMgaFCO>();

                MgaGateway.PerformInTransaction(() =>
                {
                    if (currentobj == null)
                    {
                        var allObjects = project
                           .RootFolder
                           .ChildFolders
                           .Cast<MgaFolder>()
                           .Where(x => x.Name.StartsWith("0"))
                           .SelectMany(x => x.GetDescendantFCOs(project.CreateFilter()).Cast<IMgaFCO>())
                           .Where(x => x.RootFCO == x);

                        // get all objects from folders starts with 0 within the root folder.
                        objectsToCheck.AddRange(allObjects.Where(x => x.AbsPath.Contains("TestingContextChecker")));

                        objectsToGetConfigurations.AddRange(allObjects.Where(x => x.AbsPath.Contains("TestingGetConfigurations")));
                    }
                    else
                    {
                        objectsToCheck.Add(currentobj);
                        //objectsToGetConfigurations.Add(currentobj);
                    }

                    objectsToCheck.Sort((x, y) =>
                    {
                        return x.Meta.Name.CompareTo(y.Meta.Name) != 0 ?
                            x.Meta.Name.CompareTo(y.Meta.Name) :
                            x.AbsPath.CompareTo(y.AbsPath);
                    });
                });




                System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();

                sw.Start();

                int numContexts = objectsToCheck.Count;
                int numSuccess = 0;
                int numFailures = 0;

                StringBuilder sbAssumptions = new StringBuilder();

                sbAssumptions.Append("Implemented".PadToCenter(11));
                sbAssumptions.Append("Valid".PadToCenter(11));
                sbAssumptions.Append("Context".PadToCenter(30));
                sbAssumptions.Append("Assumption");
                sbAssumptions.AppendLine();

                foreach (var subject in objectsToCheck)
                {
                    var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI();

                    IEnumerable<CyPhyMasterInterpreter.Rules.ContextCheckerResult> contextCheckerResults = null;

                    // check context
                    var checkerSuccess = masterInterpreter.TryCheckContext(subject as MgaModel, out contextCheckerResults);

                    List<CyPhyMasterInterpreter.Rules.ContextCheckerResult> sortedResults = contextCheckerResults.ToList();

                    // sort results Passed, Failed, then alphabetically based on message.
                    sortedResults.Sort((x, y) => { return x.Success == y.Success ? x.Message.CompareTo(y.Message) : y.Success.CompareTo(x.Success); });


                    // print out nicely in the GME console
                    MgaGateway.PerformInTransaction(() =>
                    {
                        MgaObject parent = null;
                        GME.MGA.Meta.objtype_enum type;
                        subject.GetParent(out parent, out type);

                        var successExpected = parent.Name.ToLowerInvariant() == "invalid" ? false : true;

                        sbAssumptions.AppendFormat("{0}{1}{2}{3}",
                            (successExpected == checkerSuccess).ToString().PadToCenter(11),
                            successExpected.ToString().PadToCenter(11),
                            subject.Meta.Name.PadToCenter(30),
                            subject.Name.ToSentenceCase());

                        sbAssumptions.AppendLine();

                        if (successExpected == checkerSuccess)
                        {
                            numSuccess++;
                            //GMEConsole.Info.WriteLine("OK");
                            if (currentobj != null)
                            {
                                foreach (var result in sortedResults)
                                {
                                    TextWriter tw = null;
                                    StringBuilder sb = new StringBuilder();
                                    if (result.Success)
                                    {
                                        sb.Append("[<b style=\"color:darkgreen\">Passed</b>]");
                                        tw = GMEConsole.Info;
                                    }
                                    else
                                    {
                                        sb.Append("[<b style=\"color:red\">Failed</b>]");
                                        tw = GMEConsole.Error;
                                    }

                                    sb.AppendFormat(" <i><a href=\"mga:{0}\">{1}</a></i> ", result.Subject.ID, result.Subject.Name);

                                    sb.Append(result.Message);

                                    tw.WriteLine(sb);
                                }
                            }
                        }
                        else
                        {
                            foreach (var result in sortedResults)
                            {
                                TextWriter tw = null;
                                StringBuilder sb = new StringBuilder();
                                if (result.Success)
                                {
                                    sb.Append("[<b style=\"color:darkgreen\">Passed</b>]");
                                    tw = GMEConsole.Info;
                                }
                                else
                                {
                                    sb.Append("[<b style=\"color:red\">Failed</b>]");
                                    tw = GMEConsole.Error;
                                }

                                sb.AppendFormat(" <i><a href=\"mga:{0}\">{1}</a></i> ", result.Subject.ID, result.Subject.Name);

                                sb.Append(result.Message);

                                tw.WriteLine(sb);
                            }

                            numFailures++;
                            GMEConsole.Error.WriteLine("========= FAILED ==========");
                            GMEConsole.Error.WriteLine("= {0}", subject.Name);
                            GMEConsole.Error.WriteLine("= {0}", subject.AbsPath);
                            GMEConsole.Error.WriteLine("===========================");
                        }


                    });

                    if (currentobj != null)
                    {
                        CyPhyMasterInterpreter.AnalysisModelProcessor analysisModelProcessor = null;

                        MgaGateway.PerformInTransaction(() =>
                        {
                            analysisModelProcessor = CyPhyMasterInterpreter.AnalysisModelProcessor.GetAnalysisModelProcessor(subject as MgaModel);
                        });

                        GMEConsole.Info.WriteLine("AnalysisProcessor type: {0}", analysisModelProcessor.GetType().Name);
                        GMEConsole.Info.WriteLine("Interpreters: {0}", string.Join(", ", analysisModelProcessor.Interpreters));
                        GMEConsole.Info.WriteLine("InterpretersToConfigure: {0}", string.Join(", ", analysisModelProcessor.InterpretersToConfiguration));

                        MgaFCO configuration = null;
                        configuration = masterInterpreter.GetConfigurations(subject as MgaModel).Cast<MgaFCO>().FirstOrDefault();

                        var results = masterInterpreter.RunInTransaction(subject as MgaModel, configuration, keepTempModels: true);

                        MgaGateway.PerformInTransaction(() =>
                        {
                            foreach (var result in results)
                            {
                                TextWriter tw = null;
                                StringBuilder sb = new StringBuilder();
                                if (result.Success)
                                {
                                    sb.Append("[<b style=\"color:darkgreen\">Passed</b>]");
                                    tw = GMEConsole.Info;
                                }
                                else
                                {
                                    sb.Append("[<b style=\"color:red\">Failed</b>]");
                                    tw = GMEConsole.Error;
                                }

                                sb.AppendFormat(" <i>{0}</i> {1} ", result.Context.Name, result.Configuration.Name);

                                sb.Append(result.Message);

                                tw.WriteLine(sb);
                            }
                        });
                    }

                }

                File.WriteAllText("master_interpreter_assumptions.txt", sbAssumptions.ToString());

                foreach (var subject in objectsToGetConfigurations)
                {
                    var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI();

                    // we are not checking the models again, these test models must be valid enough to get configurations from them

                    var configurations = masterInterpreter.GetConfigurations(subject as MgaModel);

                    // print out nicely in the GME console
                    MgaGateway.PerformInTransaction(() =>
                    {
                        GMEConsole.Info.WriteLine("{0} has {1} configurations.", subject.Name, configurations.Count);
                        //GMEConsole.Info.WriteLine("- {0}", string.Join(", ", configurations.Cast<MgaFCO>().Select(x => x.Name)));
                    });

                    //CyPhyMasterInterpreter.MasterInterpreterResult[] masterInterpreterResults = null;

                    //using (var progressDialog = new CyPhyMasterInterpreter.ProgressDialog())
                    //{
                    //    masterInterpreter.MultipleConfigurationProgress += progressDialog.MultipleConfigurationProgressHandler;
                    //    masterInterpreter.SingleConfigurationProgress += progressDialog.SingleConfigurationProgressHandler;

                    //    progressDialog.ShowWithDisabledMainWindow();

                    //    masterInterpreterResults = masterInterpreter.RunInTransaction(subject as IMgaModel, configurations);
                    //}
                }

                GMEConsole.Info.WriteLine("ContextChecks: {0}", numContexts);
                GMEConsole.Info.WriteLine("Successful   : {0}", numSuccess);
                GMEConsole.Info.WriteLine("Failures     : {0}", numFailures);

                if (numContexts == numSuccess)
                {
                    GMEConsole.Info.WriteLine("ALL GREEN :-)");
                }
                else
                {
                    GMEConsole.Error.WriteLine("You need to work more on the code...");
                }

                GMEConsole.Info.WriteLine(
                    "Duration: {0}.",
                    sw.Elapsed.ToString("c"));

            }
            finally
            {
                System.Windows.Forms.Application.DoEvents();

                if (MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
                }
                MgaGateway = null;
                project = null;
                currentobj = null;
                selectedobjs = null;
                GMEConsole = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }



        private void NewMasterInterpreterImplementationFull(MgaFCO currentobj)
        {
            this.GMEConsole.Clear();
            System.Windows.Forms.Application.DoEvents();

            this.GMEConsole.Info.WriteLine("Running Master Interpreter 2.0");

            if (currentobj == null)
            {
                GMEConsole.Error.WriteLine("Context is invalid. This component can be executed only if a valid context is open in the main editor e.g. Test Bench.");
                return;
            }

            var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI();
            masterInterpreter.IsInteractive = this.InteractiveMode;

            IEnumerable<CyPhyMasterInterpreter.Rules.ContextCheckerResult> contextCheckerResults = null;

            // check context
            var checkerSuccess = masterInterpreter.TryCheckContext(currentobj as MgaModel, out contextCheckerResults);

            List<CyPhyMasterInterpreter.Rules.ContextCheckerResult> sortedResults = contextCheckerResults.ToList();

            // sort results Passed, Failed, then alphabetically based on message.
            sortedResults.Sort((x, y) => { return x.Success == y.Success ? x.Message.CompareTo(y.Message) : y.Success.CompareTo(x.Success); });

            // print out nicely in the GME console
            MgaGateway.PerformInTransaction(() =>
            {
                foreach (var result in sortedResults)
                {
                    TextWriter tw = null;
                    StringBuilder sb = new StringBuilder();
                    if (result.Success)
                    {
                        sb.Append("[<b style=\"color:darkgreen\">Passed</b>]");
                        tw = GMEConsole.Info;
                    }
                    else
                    {
                        sb.Append("[<b style=\"color:red\">Failed</b>]");
                        tw = GMEConsole.Error;
                    }

                    sb.AppendFormat(" <i><a href=\"mga:{0}\">{1}</a></i> ", result.Subject.ID, result.Subject.Name);

                    sb.Append(result.Message);

                    tw.WriteLine(sb);
                }
            });

            if (sortedResults.Any(x => x.Success == false))
            {
                GMEConsole.Error.WriteLine("Context is invalid see messeges above. Please fix the problems.");
                return;
            }

            // context is valid
            // show GUI for the user
            CyPhyMasterInterpreter.ConfigurationSelection selection = null;

            try
            {
                selection = masterInterpreter.ShowConfigurationSelectionForm(currentobj as MgaModel);
            }
            catch (CyPhyMasterInterpreter.ExecutionCanceledByUserException ex)
            {
                GMEConsole.Warning.WriteLine("Operation was canceled by user. {0}", ex.Message);
                return;
            }

            CyPhyMasterInterpreter.MasterInterpreterResult[] miResults = null;

            // Get a progress dialog
            using (var progressDialog = new CyPhyMasterInterpreter.ProgressDialog())
            {
                masterInterpreter.MultipleConfigurationProgress += progressDialog.MultipleConfigurationProgressHandler;
                masterInterpreter.SingleConfigurationProgress += progressDialog.SingleConfigurationProgressHandler;

                if (masterInterpreter.IsInteractive)
                {
                    progressDialog.ShowWithDisabledMainWindow();
                }

                try
                {
                    miResults = masterInterpreter.RunInTransaction(selection);
                }
                catch (CyPhyMasterInterpreter.AnalysisModelInterpreterConfigurationFailedException ex)
                {
                    GMEConsole.Warning.WriteLine("Operation was canceled by user. {0}", ex.Message);
                }
                catch (CyPhyMasterInterpreter.ExecutionCanceledByUserException ex)
                {
                    GMEConsole.Warning.WriteLine("Operation was canceled by user. {0}", ex.Message);
                }

            }

            if (selection.OpenDashboard)
            {
                masterInterpreter.OpenDashboardWithChrome();
            }


            GMEConsole.Info.WriteLine(" === Summary === ");
            MgaGateway.PerformInTransaction(() =>
            {
                // print failure/success statistics
                // user master interpreter's result set in case user canceled the execution
                foreach (var result in masterInterpreter.Results)
                {
                    TextWriter tw = null;
                    StringBuilder sb = new StringBuilder();
                    if (result.Success)
                    {
                        sb.Append("[<b style=\"color:darkgreen\">Passed</b>]");
                        tw = GMEConsole.Info;
                    }
                    else
                    {
                        sb.Append("[<b style=\"color:red\">Failed</b>]");
                        tw = GMEConsole.Error;
                    }

                    sb.AppendFormat(" <i>{0}</i> {1} ", result.Context.Name, result.Configuration.Name);

                    sb.Append(result.Message);

                    tw.WriteLine(sb);
                }
            });


            this.GMEConsole.Info.WriteLine("Done Master Interpreter 2.0");
        }

        private ComponentStartMode Convert(int param)
        {
            switch (param)
            {
                case (int)ComponentStartMode.GME_BGCONTEXT_START:
                    return ComponentStartMode.GME_BGCONTEXT_START;
                case (int)ComponentStartMode.GME_BROWSER_START:
                    return ComponentStartMode.GME_BROWSER_START;

                case (int)ComponentStartMode.GME_CONTEXT_START:
                    return ComponentStartMode.GME_CONTEXT_START;

                case (int)ComponentStartMode.GME_EMBEDDED_START:
                    return ComponentStartMode.GME_EMBEDDED_START;

                case (int)ComponentStartMode.GME_ICON_START:
                    return ComponentStartMode.GME_ICON_START;

                case (int)ComponentStartMode.GME_MAIN_START:
                    return ComponentStartMode.GME_MAIN_START;

                case (int)ComponentStartMode.GME_MENU_START:
                    return ComponentStartMode.GME_MENU_START;
                case (int)ComponentStartMode.GME_SILENT_MODE:
                    return ComponentStartMode.GME_SILENT_MODE;
            }

            return ComponentStartMode.GME_SILENT_MODE;
        }

        #region Component Information
        public string ComponentName
        {
            get { return GetType().Name; }
        }

        public string ComponentProgID
        {
            get
            {
                return ComponentConfig.progID;
            }
        }

        public componenttype_enum ComponentType
        {
            get { return ComponentConfig.componentType; }
        }
        public string Paradigm
        {
            get { return ComponentConfig.paradigmName; }
        }
        #endregion

        #region Enabling
        bool enabled = true;
        public void Enable(bool newval)
        {
            enabled = newval;
        }
        #endregion

        #region Interactive Mode
        protected bool interactiveMode = true;
        public bool InteractiveMode
        {
            get
            {
                return interactiveMode;
            }
            set
            {
                interactiveMode = value;
            }
        }
        #endregion

        #region Custom Parameters
        SortedDictionary<string, object> componentParameters = null;

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
                return "csharp";

            if (Name == "path")
                return GetType().Assembly.Location;

            if (Name == "fullname")
                return GetType().FullName;

            object value;
            if (componentParameters != null && componentParameters.TryGetValue(Name, out value))
            {
                return value;
            }

            return null;
        }

        public void set_ComponentParameter(string Name, object pVal)
        {
            if (componentParameters == null)
            {
                componentParameters = new SortedDictionary<string, object>();
            }

            componentParameters[Name] = pVal;
        }
        #endregion

        #region Unused Methods
        // Old interface, it is never called for MgaComponentEx interfaces
        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        // Not used by GME
        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        #endregion

        #endregion

        #region IMgaVersionInfo Members

        public GMEInterfaceVersion_enum version
        {
            get { return GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        #endregion

        #region Registration Helpers

        [ComRegisterFunctionAttribute]
        public static void GMERegister(Type t)
        {
            Registrar.RegisterComponentsInGMERegistry();

        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnRegister(Type t)
        {
            Registrar.UnregisterComponentsInGMERegistry();
        }

        #endregion


    }
}
