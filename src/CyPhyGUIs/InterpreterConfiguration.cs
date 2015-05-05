using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CyPhyGUIs
{
    [Guid("82FADB53-8EAA-471F-BE03-084338D9C830"),
    ProgId("ISIS.CyPhyML.InterpreterResult"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class InterpreterResult : IInterpreterResult
    {
        public CyPhyCOMInterfaces.IMgaTraceability Traceability
        {
            get;
            set;
        }

        public bool Success
        {
            get;
            set;
        }

        public string RunCommand
        {
            get;
            set;
        }
        public string LogFileDirectory
        {
            get;
            set;
        }

        public bool ConsistencyCheckerResult
        {
            get;
            set;
        }

        public string ZippyServerSideHook
        {
            get;
            set;
        }

        public string Labels
        {
            get;
            set;
        }

        public string BuildQuery
        {
            get;
            set;
        }
    }

    [Guid("FA6EE6D3-97FA-4B9F-B350-AAE8AEFC4312")]
    [ProgId("ISIS.CyPhyML.PreConfigArgs"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class PreConfigArgs : IPreConfigParameters
    {
        public string OutputDirectory
        {
            get;
            set;
        }

        public GME.MGA.IMgaProject Project
        {
            get;
            set;
        }

        /**
         * see component_startmode_enum in C:\Program Files (x86)\GME\Interfaces\Mga.idl
         */
        public int StartModeParam
        {
            get;
            set;
        }

        public string ProjectDirectory
        {
            get;
            set;
        }
    }


    [Guid("6C9DC950-A72A-40A2-8EFA-87FEF8777BA3")]
    [ProgId("ISIS.CyPhyML.InterpreterConfiguration"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class InterpreterMainParameters : IInterpreterMainParameters
    {
        public InterpreterMainParameters()
        {
            ConsoleMessages = true;
        }

        public GME.MGA.MgaProject Project
        {
            get;
            set;
        }

        public GME.MGA.MgaFCO CurrentFCO
        {
            get;
            set;
        }

        public GME.MGA.MgaFCOs SelectedFCOs
        {
            get;
            set;
        }

        public int StartModeParam
        {
            get;
            set;
        }

        public string OutputDirectory
        {
            get;
            set;
        }

        public string ProjectDirectory
        {
            get;
            set;
        }

        public IInterpreterConfiguration config
        {
            get;
            set;
        }

        public bool ConsoleMessages
        {
            get;
            set;
        }

        public META.MgaTraceability Traceability
        {
            get;
            set;
        }

        public CyPhyCOMInterfaces.IMgaTraceability GetTraceability()
        {
            return Traceability;
        }

        public bool VerboseConsole
        {
            get;
            set;
        }
    }

    [Guid("846F6E5E-16B3-46FE-A3D9-16C6A2DA9CC6")]
    [ProgId("ISIS.CyPhyML.NullInterpreterConfiguration"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class NullInterpreterConfiguration : IInterpreterConfiguration
    {
    }
}
