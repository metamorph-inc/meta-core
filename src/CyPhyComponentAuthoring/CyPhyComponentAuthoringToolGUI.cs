using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using EventArgs = System.EventArgs;

namespace CyPhyComponentAuthoring
{
    public partial class CyPhyComponentAuthoringToolGUI : Form
    {
        private Action<string> dragNDropHandler;
        public CyPhyComponentAuthoringToolGUI(Action<string> dragNDropHandler)
        {
            InitializeComponent();

            this.splitContainer1.DragDrop += new System.Windows.Forms.DragEventHandler(CyPhyComponentAuthoringToolGUI_DragDrop);
            this.splitContainer1.DragEnter += new System.Windows.Forms.DragEventHandler(CyPhyComponentAuthoringToolGUI_DragEnter);
            this.splitContainer1.AllowDrop = true;

            this.dragNDropHandler = dragNDropHandler;
        }

        protected override void ScaleControl(SizeF factor, BoundsSpecified specified)
        {
            base.ScaleControl(factor, specified);

            var originalTileSize = CatModuleListView.TileSize;

            CatModuleListView.TileSize = new Size((int) (originalTileSize.Width * factor.Width), (int) (originalTileSize
                                                                                                            .Height *
                                                                                                        factor.Height));
            
            //Save our collection of images
            var savedImages = new List<Image>(ComponentIconList.Images.Cast<Image>());
            var originalImageSize = ComponentIconList.ImageSize;

            ComponentIconList.ImageSize = new Size((int)(originalImageSize.Width * factor.Width), (int)(originalImageSize
                                                                                                           .Height *
                                                                                                       factor.Height));

            foreach (var image in savedImages)
            {
                ComponentIconList.Images.Add(image);
            }
        }

        private void CyPhyComponentAuthoringToolGUI_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = null;
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                files = (string[])e.Data.GetData(DataFormats.FileDrop);
            }
            else if (e.Data.GetDataPresent("FileGroupDescriptorW"))
            {
                List<string> filesList = new List<string>();

                var buffer = new byte[4096];
                var handle = GCHandle.Alloc(buffer);
                var fileGroupDescriptor = (Stream)e.Data.GetData("FileGroupDescriptorW");
                int bytesRead = fileGroupDescriptor.Read(buffer, 0, buffer.Length);
                int numElements = Marshal.ReadInt32(Marshal.UnsafeAddrOfPinnedArrayElement(buffer, 0));
                for (int i = 0; i < numElements; i++)
                {
                    var fd = (FILEDESCRIPTOR)Marshal.PtrToStructure(Marshal.UnsafeAddrOfPinnedArrayElement(buffer, 4 + i * Marshal.SizeOf(typeof(FILEDESCRIPTOR))), typeof(FILEDESCRIPTOR));
                    filesList.Add(fd.cFileName);
                }
                handle.Free();

                files = filesList.ToArray();
            }

            if (files != null)
            {
                foreach (string filename in files)
                {
                    dragNDropHandler(filename);
                }
            }
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        struct FILEDESCRIPTOR
        {
            public uint dwFlags;
            public Guid clsid;
            public System.Drawing.Size sizel;
            public System.Drawing.Point pointl;
            public UInt32 dwFileAttributes;
            public System.Runtime.InteropServices.ComTypes.FILETIME ftCreationTime;
            public System.Runtime.InteropServices.ComTypes.FILETIME ftLastAccessTime;
            public System.Runtime.InteropServices.ComTypes.FILETIME ftLastWriteTime;
            public UInt32 nFileSizeHigh;
            public UInt32 nFileSizeLow;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string cFileName;
        }

        private void CyPhyComponentAuthoringToolGUI_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
            else if (e.Data.GetDataPresent("FileGroupDescriptorW"))
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        private void CatModuleListView_ItemActivate(object sender, EventArgs e)
        {
            RunSelectedItem();
        }

        private void RunSelectedItem()
        {
            if (CatModuleListView.SelectedItems.Count > 0)
            {
                var selectedItem = CatModuleListView.SelectedItems[0];

                if (selectedItem is CatToolListViewItem)
                {
                    (selectedItem as CatToolListViewItem).RunAction(this);
                }
            }
        }

        private void CatModuleListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (CatModuleListView.SelectedItems.Count > 0)
            {
                var selectedItem = CatModuleListView.SelectedItems[0];

                if (selectedItem is CatToolListViewItem)
                {
                    var selectedListViewItem = selectedItem as CatToolListViewItem;

                    ModuleNameLabel.Text = selectedListViewItem.Attributes.NameVal;
                    ModuleDetailsTextBox.Text = selectedListViewItem.Attributes.DescriptionVal;

                    RunToolButton.Enabled = true;
                }
                else
                {
                    ModuleNameLabel.Text = "";
                    ModuleDetailsTextBox.Text = "";

                    RunToolButton.Enabled = false;
                }
            }
            else
            {
                ModuleNameLabel.Text = "";
                ModuleDetailsTextBox.Text = "";

                RunToolButton.Enabled = false;
            }
        }

        private void CyPhyComponentAuthoringToolGUI_Load(object sender, EventArgs e)
        {
            // Set the default dialog font on each child control
            //FixControlFont(this);

            //ModuleNameLabel.Font = new Font(SystemFonts.MessageBoxFont, FontStyle.Bold);
        }

        /**
         * Currently unused, because changing the control font to the system font
         * makes our icons blurry.  Yay Winforms.
         */
        private void FixControlFont(Control c)
        {
            foreach (var subcontrol in c.Controls)
            {
                if (subcontrol is Control)
                {
                    FixControlFont(subcontrol as Control);
                }
            }

            c.Font = SystemFonts.MessageBoxFont;
        }

        private void RunToolButton_Click(object sender, EventArgs e)
        {
            RunSelectedItem();
        }
    }

    public class CatToolListViewItem : ListViewItem
    {
        public CyPhyComponentAuthoringInterpreter.CATName Attributes { get; }

        public EventHandler Action { get; set; }

        public CatToolListViewItem(CyPhyComponentAuthoringInterpreter.CATName attributes, ImageList iconImageList) : base()
        {
            Attributes = attributes;

            this.Text = attributes.NameVal;

            var iconKey = attributes.IconResourceKey ?? "generic_module";

            var icon = Resources.ResourceManager.GetObject(iconKey) as Icon ?? Resources.generic_module;
            icon = new Icon(icon, 48, 48);

            iconImageList.Images.Add(icon);
            this.ImageIndex = iconImageList.Images.Count - 1;
        }

        public void RunAction(object sender)
        {
            Action?.Invoke(sender, EventArgs.Empty);
        }
    }
}
