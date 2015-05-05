using System;
using System.Windows.Media.Imaging;
using DesignConsistencyChecker.Framework.Notification;
using DesignConsistencyChecker.DesignRule;
using System.Windows.Media;

namespace DesignConsistencyChecker.Models
{
    internal class OutputMessageDescriptor : NotifyBase
    {
        private RuleFeedbackBase _feedback;
        public RuleFeedbackBase Feedback
        {
            get { return _feedback; }
            set
            {
                _feedback = value;
                OnPropertyChanged(() => Feedback);
                OnPropertyChanged(() => Icon);
            }
        }

        public ImageSource Icon
        {
            get {
                if (Feedback == null) return null;
                if (Feedback.FeedbackType == FeedbackTypes.Error) return new BitmapImage(new Uri("/DesignConsistencyChecker;component/Images/CriticalError.png", UriKind.Relative));
                if (Feedback.FeedbackType == FeedbackTypes.Warning) return new BitmapImage(new Uri("/DesignConsistencyChecker;component/Images/Warning.png", UriKind.Relative));
                return null;
            }
        }

        //private string _message;
        //public string Message
        //{
        //    get { return _message; }
        //    set
        //    {
        //        _message = value;
        //        OnPropertyChanged(() => Message);
        //    }
        //}

        //private string _elementName;
        //public string ElementName
        //{
        //    get { return _elementName; }
        //    set
        //    {
        //        _elementName = value;
        //        OnPropertyChanged(() => ElementName);
        //    }
        //}

        //private string _elementId;
        //public string ElementId
        //{
        //    get { return _elementId; }
        //    set
        //    {
        //        _elementId = value;
        //        OnPropertyChanged(() => ElementId);
        //    }
        //}

        public OutputMessageDescriptor()
        {
            //Icon = @"\Images\CriticalError.png";
        }
    }
}