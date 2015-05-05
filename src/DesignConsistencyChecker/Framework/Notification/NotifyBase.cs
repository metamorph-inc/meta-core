using System;
using System.Linq.Expressions;
using System.ComponentModel;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Threading;

namespace DesignConsistencyChecker.Framework.Notification
{
    internal abstract class NotifyBase : DispatcherObject, INotifyPropertyChanged
    {
        protected virtual void OnPropertyChanged(Expression<Func<object>> expression)
        {
            PropertyChanged.Raise(this, expression);
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }


    internal class UserControlNotifyBase : UserControl, INotifyPropertyChanged
    {
        protected virtual void OnPropertyChanged(Expression<Func<object>> expression)
        {
            PropertyChanged.Raise(this, expression);
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }

    internal class WindowNotifyBase : Window, INotifyPropertyChanged
    {
        protected virtual void OnPropertyChanged(Expression<Func<object>> expression)
        {
            PropertyChanged.Raise(this, expression);
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged(String propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (null != handler)
            {
                handler(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }


}
