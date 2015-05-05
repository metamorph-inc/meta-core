using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using DesignConsistencyChecker.Framework.Command;

namespace DesignConsistencyChecker.Controls
{
    /// <summary>
    /// Interaction logic for TagControl.xaml
    /// </summary>
    internal partial class TagControl
    {

        #region Commands

        private DelegateCommand _addTagCommand;
        public DelegateCommand AddTagCommand
        {
            get { return _addTagCommand; }
            set
            {
                _addTagCommand = value;
                OnPropertyChanged(() => AddTagCommand);
            }
        }

        private DelegateCommand _removeTagCommand;
        public DelegateCommand RemoveTagCommand
        {
            get { return _removeTagCommand; }
            set
            {
                _removeTagCommand = value;
                OnPropertyChanged(() => RemoveTagCommand);
            }
        }

        private DelegateCommand _selectAllTagCommand;
        public DelegateCommand SelectAllTagCommand
        {
            get { return _selectAllTagCommand; }
            set
            {
                _selectAllTagCommand = value;
                OnPropertyChanged(() => SelectAllTagCommand);
            }
        }

        private DelegateCommand _clearTagSelectionCommand;
        public DelegateCommand ClearTagSelectionCommand
        {
            get { return _clearTagSelectionCommand; }
            set
            {
                _clearTagSelectionCommand = value;
                OnPropertyChanged(() => ClearTagSelectionCommand);
            }
        }

        #endregion

        #region Properties
        
        private string _textAdd;
        public string TextAdd
        {
            get { return _textAdd; }
            set
            {
                _textAdd = value;
                OnPropertyChanged(() => TextAdd);
            }
        }

        #region Dependency properties

        public static readonly DependencyProperty AllTagsProperty =
            DependencyProperty.Register("AllTags", typeof(ObservableCollection<string>), typeof(TagControl), new PropertyMetadata(default(ObservableCollection<string>)));

        public ObservableCollection<string> AllTags
        {
            get { return (ObservableCollection<string>)GetValue(AllTagsProperty); }
            set { SetValue(AllTagsProperty, value); }
        }

        public static readonly DependencyProperty SelectedTagsProperty =
            DependencyProperty.Register("SelectedTags", typeof (ObservableCollection<string>), typeof (TagControl), new PropertyMetadata(default(ObservableCollection<string>)));

        public ObservableCollection<string> SelectedTags
        {
            get { return (ObservableCollection<string>) GetValue(SelectedTagsProperty); }
            set { SetValue(SelectedTagsProperty, value); }
        }

        //private ObservableCollection<string> _selectedTags;
        //public ObservableCollection<string> SelectedTags
        //{
        //    get { return _selectedTags; }
        //    set
        //    {
        //        _selectedTags = value;
        //        OnPropertyChanged(() => SelectedTags);
        //    }
        //}

        #endregion

        #endregion

        public TagControl()
        {
            InitializeComponent();
            rGrid.DataContext = this;
            SelectedTags = new ObservableCollection<string>();
            AllTags = new ObservableCollection<string>();


            AddTagCommand = new DelegateCommand(o =>
                                                    {
                                                        var s = (string)o;
                                                        if (SelectedTags == null || AllTags == null) return;
                                                        SelectedTags.Add(s);
                                                        AllTags.Remove(s);
                                                    }, o => true);

            RemoveTagCommand = new DelegateCommand(o =>
            {
                var s = (string)o;
                if (SelectedTags == null || AllTags == null) return;
                SelectedTags.Remove(s);
                AllTags.Add(s);
            }, o => true);

            ClearTagSelectionCommand = new DelegateCommand(o =>
                                                               {
                                                                   if (SelectedTags == null || AllTags == null) return;
                                                                   foreach (var selectedTag in SelectedTags)
                                                                   {
                                                                       AllTags.Add(selectedTag);
                                                                   }

                                                                   SelectedTags.Clear();
                                                               }, o => true);

            SelectAllTagCommand = new DelegateCommand(o =>{
                                                              if (SelectedTags == null || AllTags == null) return;
                                                              foreach (var tag in AllTags)
                                                              {
                                                                  SelectedTags.Add(tag);
                                                              }
                                                              AllTags.Clear();
            }, o=>true);

        }
    }
}
