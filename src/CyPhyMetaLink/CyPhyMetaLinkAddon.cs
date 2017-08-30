using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using GME.CSharp;
using GME.MGA.Core;
using System.Runtime.InteropServices;
using System.IO;

namespace CyPhyMetaLink
{
    /// <summary>
    /// This file contains GME-related plugin functionality.
    /// Most of the implemented functions implement interface functionality
    /// </summary>
    partial class CyPhyMetaLinkAddon // : IMgaComponentEx, IGMEVersionInfo, IMgaEventSink
    {
        // Used in automated tests, no UI activity if on
        public bool TestMode;
        public bool TestMode_NoAutomaticCreoStart;

        // The latest (current) configuration
        MetaLinkConfiguration Configuration;

        public void Initialize(MgaProject project)
        {
            try
            {
                Configuration = MetaLinkConfiguration.Create(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "metalink.config"));
            }
            catch (Exception e)
            {
                GMEConsole.Warning.WriteLine("Unable to read Meta-Link configuration: " + e.Message);
                Configuration = new MetaLinkConfiguration();
            }

            // Creating addon 
            project.CreateAddOn(this, out addon);

            // Setting event mask (see ComponentConfig.eventMask)
            unchecked
            {
                addon.EventMask = (uint)ComponentConfig_Addon.eventMask;
            }
            // DISABLE BY DEFAULT
            Enable(false);

            // Create the proxy windows control which sends-receives messages
            this.SyncControl = new System.Windows.Forms.Control();
            IntPtr handle = SyncControl.Handle; // If the handle has not yet been created, referencing this property will force the handle to be created.
        }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException(); // Not called by addon
        }


        #region Component Information
        public string ComponentName
        {
            get { return GetType().Name; }
        }

        public string ComponentProgID
        {
            get
            {
                return ComponentConfig_Addon.progID;
            }
        }

        public componenttype_enum ComponentType
        {
            get { return ComponentConfig_Addon.componentType; }
        }
        public string Paradigm
        {
            get { return ComponentConfig_Addon.paradigmName; }
        }
        #endregion

        bool enabled = true;
        public void Enable(bool newval)
        {
            if (!newval)
            {
                // clear table
                InstanceIDToConstraint_Table.Clear();
            }

            enabled = newval;
        }
        protected bool interactiveMode = true;
        public bool InteractiveMode
        {
            get
            {
                return interactiveMode;
            }
            set
            {
                interactiveMode = value;
            }
        }
        #region Custom Parameters
        SortedDictionary<string, object> componentParameters = null;

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
            {
                return "csharp";
            }

            if (Name == "path")
            {
                return GetType().Assembly.Location;
            }

            if (Name == "fullname")
            {
                return GetType().FullName;
            }

            object value;
            if (componentParameters != null && componentParameters.TryGetValue(Name, out value))
            {
                return value;
            }

            return null;
        }

        public void set_ComponentParameter(string Name, object pVal)
        {
            if (componentParameters == null)
            {
                componentParameters = new SortedDictionary<string, object>();
            }

            componentParameters[Name] = pVal;
        }
        #endregion
        #region Unused Methods
        // Old interface, it is never called for MgaComponentEx interfaces
        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
        }

        // Not used by GME
        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        #endregion
        #region IMgaVersionInfo Members

        public GMEInterfaceVersion_enum version
        {
            get { return GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        #endregion

        #region Registration Helpers

        [ComRegisterFunctionAttribute]
        public static void GMERegister(Type t)
        {
            Registrar.RegisterAddon();
        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnRegister(Type t)
        {
            Registrar.UnregisterAddon();
        }

        #endregion
    }
}
