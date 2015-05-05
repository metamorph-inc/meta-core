using System;
using DesignConsistencyChecker.DesignRule;
using DesignConsistencyChecker.Framework.Notification;
namespace DesignConsistencyChecker.Models
{
    internal class TagWrapper : NotifyBase
    {
        public string Tag { get; private set; }

        private bool _isSelected;
        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                _isSelected = value;
                OnPropertyChanged(() => IsSelected);
                OnSelectionChanged();
            }
        }

        public TagWrapper(string tag)
        {
            Tag = tag;

            PropertyChanged += (o, e) => { };
        }

        public event EventHandler SelectionChanged;

        protected virtual void OnSelectionChanged()
        {
            var handler = SelectionChanged;
            if (handler != null) handler(this, EventArgs.Empty);
        }
    }
}