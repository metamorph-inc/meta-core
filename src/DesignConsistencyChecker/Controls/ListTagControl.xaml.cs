using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using DesignConsistencyChecker.Framework.Command;
using DesignConsistencyChecker.Models;

namespace DesignConsistencyChecker.Controls
{
    /// <summary>
    /// Interaction logic for TagControl.xaml
    /// </summary>
    internal partial class ListTagControl
    {

        public bool? IsAllSelected
        {
            get
            {
                if (Tags == null) return null;
                return Tags.All(x => x.IsSelected) ? true : (Tags.All(x => x.IsSelected == false) ? (bool?)false : null);
            }
            set
            {
                if (value.HasValue)
                {
                    foreach (var ruleWrapper in Tags)
                    {
                        ruleWrapper.IsSelected = value.Value;
                    }
                }
                OnPropertyChanged(() => IsAllSelected);
            }
        }

        #region Dependency properties

        public static readonly DependencyProperty TagsProperty =
            DependencyProperty.Register("Tags", typeof(ObservableCollection<TagWrapper>), typeof(ListTagControl), new PropertyMetadata(default(ObservableCollection<TagWrapper>)));

        public ObservableCollection<TagWrapper> Tags
        {
            get { return (ObservableCollection<TagWrapper>)GetValue(TagsProperty); }
            set { SetValue(TagsProperty, value); }
        }



        #endregion

        public ListTagControl()
        {
            InitializeComponent();
            rGrid.DataContext = this;
            Tags = new ObservableCollection<TagWrapper>();
        }
    }
}
