using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhyGUIs
{
    public class NativeWindowWrapper : System.Windows.Forms.IWin32Window
    {
        public NativeWindowWrapper(IntPtr handle)
        {
            _hwnd = handle;
        }

        public IntPtr Handle
        {
            get { return _hwnd; }
        }

        private IntPtr _hwnd;
    }
}
