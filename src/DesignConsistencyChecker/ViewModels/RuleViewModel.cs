using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using DesignConsistencyChecker.DesignRule;
using DesignConsistencyChecker.Framework;
using DesignConsistencyChecker.Framework.Notification;
using DesignConsistencyChecker.Models;
using DesignConsistencyChecker.Framework.Command;
using System.Windows;
using System.IO;

namespace DesignConsistencyChecker.ViewModels
{
    internal class RuleViewModel : NotifyBase
    {

        private string _contextName;

        public string ContextName
        {
            get { return _contextName; }
            set
            {
                _contextName = value;
                OnPropertyChanged(() => ContextName);
            }
        }

        private string _contextType;

        public string ContextType
        {
            get { return _contextType; }
            set
            {
                _contextType = value;
                OnPropertyChanged(() => ContextType);
            }
        }

        private ObservableCollection<RuleWrapper> _ruleList;
        public ObservableCollection<RuleWrapper> RuleList
        {
            get { return _ruleList; }
            set
            {
                _ruleList = value;
                OnPropertyChanged(() => RuleList);
            }
        }

        private ObservableCollection<TagWrapper> _tagList;
        public ObservableCollection<TagWrapper> TagList
        {
            get { return _tagList; }
            set
            {
                _tagList = value;
                OnPropertyChanged(() => TagList);
            }
        }

        //private ObservableCollection<string> _selectedTagList;
        //public ObservableCollection<string> SelectedTagList
        //{
        //    get { return _selectedTagList; }
        //    set
        //    {
        //        _selectedTagList = value;
        //        OnPropertyChanged(() => SelectedTagList);
        //        RefreshselectedRules();
        //    }
        //}

        

        private void RefreshselectedRules()
        {
            foreach (var rule in RuleList)
            {
                var ruleTags = rule.Rule.Tags.Select(x => x.ToLower());

                if (TagList.Where(x=>x.IsSelected).Any(x => ruleTags.Contains(x.Tag.ToLower()))) rule.IsSelected = true;
                else rule.IsSelected = false;
            }
        }

        private DelegateCommand _checkCommand;
        public DelegateCommand CheckCommand
        {
            get { return _checkCommand; }
            set
            {
                _checkCommand = value;
                OnPropertyChanged(() => CheckCommand);
            }
        }

        //private bool? _isAllSelected;
        //public bool? IsAllSelected
        //{
        //    get { return _isAllSelected; }
        //    set
        //    {
        //        _isAllSelected = value;
        //        OnPropertyChanged(() => IsAllSelected);

        //        if (IsAllSelected.HasValue)
        //        {
        //            foreach (var ruleWrapper in RuleList)
        //            {
        //                ruleWrapper.IsSelected = IsAllSelected.Value;
        //            }
        //        }
        //    }
        //}

        public bool? IsAllSelected
        {
            get
            {
                return RuleList.All(x=>x.IsSelected)? true: (RuleList.All(x => x.IsSelected == false) ? (bool?)false : null);
            }
            set
            {
                if (value.HasValue)
                {
                    foreach (var ruleWrapper in RuleList)
                    {
                        ruleWrapper.IsSelected = value.Value;
                    }

                    if (value == false)
                    {
                        foreach (var tagWrapper in TagList)
                        {
                            tagWrapper.IsSelected = false;
                        }
                    }
                }
                OnPropertyChanged(() => IsAllSelected);
            }
        }

        private DelegateCommand _deselectAllRules;
        public DelegateCommand DeselectAllRules
        {
            get { return _deselectAllRules; }
            set
            {
                _deselectAllRules = value;
                OnPropertyChanged(() => DeselectAllRules);
            }
        }

        private DelegateCommand _closeCommand;

        public DelegateCommand CloseCommand
        {
            get { return _closeCommand; }
            set
            {
                _closeCommand = value;
                OnPropertyChanged(() => CloseCommand);
            }
        }

       
        private bool _isRulesLoading;
        public bool IsRulesLoading
        {
            get { return _isRulesLoading; }
            set
            {
                _isRulesLoading = value;
                OnPropertyChanged(() => IsRulesLoading);
            }
        }

        private bool _isModelElementsLoading;
        public bool IsModelElementsLoading
        {
            get { return _isModelElementsLoading; }
            set
            {
                _isModelElementsLoading = value;
                OnPropertyChanged(() => IsModelElementsLoading);
            }
        }

        private ObservableCollection<OutputMessageDescriptor> _feedbacks;
        public ObservableCollection<OutputMessageDescriptor> Feedbacks
        {
            get { return _feedbacks; }
            set
            {
                _feedbacks = value;
                OnPropertyChanged(() => Feedbacks);
            }
        }

        private readonly Checker _checker;
        private readonly Window _window;
        public RuleViewModel(Checker checker, Window window)
        {
            _window = window;
            _checker = checker;

            // list the name of the interpreters
            var interpreterNames = new List<string>()
            {
                "CyPhy2Modelica_v2",
                "CyPhyPET"
            };

            // these are the path, where the dll could be.
            // the search order is important.
            var searchLocations = new List<string>()
            {
                 META.VersionInfo.MetaPath + @"\bin\{0}.dll",
                 META.VersionInfo.MetaPath + @"\src\{0}\bin\Release\{0}.dll",
                 META.VersionInfo.MetaPath + @"\src\{0}\bin\Debug\{0}.dll",
            };

            // add all found dlls into this list then load them.
            var dllsToLoad = new List<string>();
            dllsToLoad.Add("DesignConsistencyChecker.dll");

            // search for the dlls
            foreach (var interpreterName in interpreterNames)
            {
                foreach (var searchLocation in searchLocations)
                {
                    var interpreterLocation = string.Format(searchLocation, interpreterName);
                    if (File.Exists(interpreterLocation))
                    {
                        dllsToLoad.Add(interpreterLocation);
                        break;
                    }
                }
            }

            //_ruleLoadTask = Task.Factory.StartNew(() =>
                                      //{
                                      //    IsRulesLoading = true;
                                            _checker.RegisterRuleDlls(dllsToLoad);
                                      //}).ContinueWith(o => { IsRulesLoading = false; });

            IsModelElementsLoading = true;
            _window.ContentRendered += (o, e) =>
                                           {
                                               _checker.RefreshChildrenCache();
                                               IsModelElementsLoading = false;
                                           };

            if (_checker.Model != null && _checker.Model.Meta != null)
            {
                ContextName = _checker.Model.Name;
                ContextType = _checker.Model.Meta.Name;
            }
            else
            {
                ContextName = ContextType = "_";
            }

            CheckCommand=new DelegateCommand(ExecuteCheckCommand, CanExecuteCheck);
            
            RuleList = new ObservableCollection<RuleWrapper>(checker.GetRegisteredRules.Select(x=>new RuleWrapper(x)));

            Feedbacks = new ObservableCollection<OutputMessageDescriptor>();
            TagList = new ObservableCollection<TagWrapper>(checker.AllTags.Select(x=>new TagWrapper(x){IsSelected = false}));

            foreach (var tagWrapper in TagList)
            {
                tagWrapper.SelectionChanged += (o, e) => { RefreshselectedRules(); };
            }


            //SelectAllRules = new DelegateCommand(o =>
            //                                         {
            //                                             foreach (var rule in RuleList)
            //                                             {
            //                                                 rule.IsSelected = true;
            //                                             }
            //                                         }, o => RuleList.Any(x=>!x.IsSelected));

            //DeselectAllRules = new DelegateCommand(o =>
            //{
            //    foreach (var rule in RuleList)
            //    {
            //        rule.IsSelected = false;
            //    }
            //}, o => RuleList.Any(x => x.IsSelected));

            CloseCommand = new DelegateCommand(o => _window.Close(),o=>true);

            foreach (var rule in RuleList)
            {
                rule.PropertyChanged += (o, e) =>
                {
                    if (e.PropertyName == "IsSelected")
                    {
                        CheckCommand.CanExecute(null);
                        OnPropertyChanged(()=>IsAllSelected);
                        //SelectAllRules.CanExecute(null);
                        //DeselectAllRules.CanExecute(null);
                    }
                };
            }

            //SelectDeselectAllCommand = new DelegateCommand(o =>
            //                                                   {
            //                                                       var c = o as CheckBox;
            //                                                   }, o=>true);

            CheckCommand.CanExecute(null);
            //SelectAllRules.CanExecute(null);
            //DeselectAllRules.CanExecute(null);
        }

        private bool CanExecuteCheck(object state)
        {
            return RuleList.Any(x => x.IsSelected);
        }

        private void ExecuteCheckCommand(object state)
        {
            Feedbacks.Clear();

            foreach (var currentRule in RuleList.Where(x => x.IsSelected))
            {
                List<RuleFeedbackBase> fb;
                List<CheckerFeedback> cb;

                ///////////////////
                // TODO: this is really slow while we call the elaborator as many times as many rules require it.
                ///////////////////
                _checker.CheckRule(currentRule.Rule, out fb, out cb);

                if (fb.Any(x => x.FeedbackType == FeedbackTypes.Error)) currentRule.ResultType = ResultTypes.Error;
                else if (fb.Any(x => x.FeedbackType == FeedbackTypes.Warning)) currentRule.ResultType = ResultTypes.Warning;
                else currentRule.ResultType = ResultTypes.Ok;

                if (cb.Any(x=>x.CheckerResultType == CheckerResultTypes.InvalidContext)) currentRule.CheckerResultType = CheckerResultTypes.InvalidContext;
                else currentRule.CheckerResultType = CheckerResultTypes.Ok;

            }
        }
    }
}
