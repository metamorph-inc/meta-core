using System.Collections.ObjectModel;
using System.Windows;
using DesignConsistencyChecker.Models;

namespace DesignConsistencyChecker.Controls
{
    internal partial class OutputMessageResult
    {
        public ObservableCollection<OutputMessageDescriptor> DescriptorList
        {
            get
            {
                return (ObservableCollection<OutputMessageDescriptor>)GetValue(DescriptorListProperty);
            }
            set
            {
                SetValue(DescriptorListProperty, value);
            }
        }

        public static readonly DependencyProperty DescriptorListProperty =
            DependencyProperty.Register(
                "DescriptorList",
                typeof(ObservableCollection<OutputMessageDescriptor>),
                typeof(OutputMessageResult),
                new FrameworkPropertyMetadata(
                    ChangeFunctions));

        private static void ChangeFunctions(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ((OutputMessageResult)source).UpdateFunctions(e.NewValue as ObservableCollection<OutputMessageDescriptor>);
        }

        private void UpdateFunctions(ObservableCollection<OutputMessageDescriptor> newDescriptor)
        {

        }

        public OutputMessageResult()
        {
            InitializeComponent();
            listControl.DataContext = this;
        }
    }
}
