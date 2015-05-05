using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DesignConsistencyChecker.DesignRule;
using DesignConsistencyChecker.Framework.Notification;
namespace DesignConsistencyChecker.Models
{
    internal class RuleWrapper : NotifyBase
    {
        public RuleDescriptor Rule { get; private set; }

        private bool _isSelected;
        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                _isSelected = value;
                OnPropertyChanged(() => IsSelected);
            }
        }

        private ResultTypes _resultType;

        public ResultTypes ResultType
        {
            get { return _resultType; }
            set
            {
                _resultType = value;
                OnPropertyChanged(() => ResultType);
                OnPropertyChanged(() => Icon);
                OnPropertyChanged(() => FeedbackResultMessage);
            }
        }

        private CheckerResultTypes _checkerResultType;
        public CheckerResultTypes CheckerResultType
        {
            get { return _checkerResultType; }
            set
            {
                _checkerResultType = value;
                OnPropertyChanged(() => CheckerResultType);
                OnPropertyChanged(() => CheckerIcon);
                OnPropertyChanged(() => CheckerResultMessage);
            }
        }

        public ImageSource Icon
        {
            get
            {
                if (ResultType == ResultTypes.Undefined) return null;
                if (ResultType == ResultTypes.Error) return new BitmapImage(new Uri("/DesignConsistencyChecker;component/Images/CriticalError.png", UriKind.Relative));
                if (ResultType == ResultTypes.Warning) return new BitmapImage(new Uri("/DesignConsistencyChecker;component/Images/Warning.png", UriKind.Relative));
                if (ResultType == ResultTypes.Ok) return new BitmapImage(new Uri("/DesignConsistencyChecker;component/Images/OK.png", UriKind.Relative));

                return null;
            }
        }

        public ImageSource CheckerIcon
        {
            get
            {
                if (CheckerResultType == CheckerResultTypes.InvalidContext) return new BitmapImage(new Uri("/DesignConsistencyChecker;component/Images/redwarning.png", UriKind.Relative));
                return null;
            }
        }

        public string FeedbackResultMessage
        {
            get
            {
                if (ResultType == ResultTypes.Undefined) return string.Empty;
                if (ResultType == ResultTypes.Error) return "The rule has finished with errors.";
                if (ResultType == ResultTypes.Warning) return "The rule has finished with warnings.";
                if (ResultType == ResultTypes.Ok) return "No errors or warnings.";

                return null;
            }
        }

        public string CheckerResultMessage
        {
            get
            {
                if (CheckerResultType == CheckerResultTypes.InvalidContext) return "Context is not valid for this rule!";
                return string.Empty;
            }
        }

        public RuleWrapper(RuleDescriptor rule)
        {
            Rule = rule;
        }
    }

    internal enum ResultTypes
    {
        Undefined,
        Ok,
        Warning,
        Error
    }

    
}