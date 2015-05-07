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

namespace CyPhyComponentAuthoring
{
    public partial class CyPhyComponentAuthoringToolGUI : Form
    {
        private Action<string> dragNDropHandler;
        public CyPhyComponentAuthoringToolGUI(Action<string> dragNDropHandler)
        {
            InitializeComponent();

            tableLayoutPanel0.DragDrop += new System.Windows.Forms.DragEventHandler(CyPhyComponentAuthoringToolGUI_DragDrop);
            tableLayoutPanel0.DragEnter += new System.Windows.Forms.DragEventHandler(CyPhyComponentAuthoringToolGUI_DragEnter);
            tableLayoutPanel0.AllowDrop = true;

            this.dragNDropHandler = dragNDropHandler;
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
    }
}
