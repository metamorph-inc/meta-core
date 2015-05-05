using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhyCOMInterfaces;
using System.Runtime.InteropServices;

namespace CyPhyGUIs
{
    [Guid("FE6F8929-DBAC-4267-9BBC-75A3BA04FC69")]
    [ComVisible(true)]
    public interface ICyPhyInterpreter
    {
        string InterpreterConfigurationProgId { get; }

        /// <summary>
        /// Called once per MasterInterpreter run (or equivalent)
        /// </summary>
        /// <param name="parameters"></param>
        /// <returns></returns>
        IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters);

        /// <summary>
        /// Called once only if running in interactive (non-automated) mode. Interpreters should not access the model during this method.
        /// </summary>
        /// <returns>null iff the user selects Cancel in the dialog</returns>
        IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig);

        IInterpreterResult Main(IInterpreterMainParameters parameters);
    }

    /// <summary>
    /// Only properties in InterpreterConfiguration with WorkflowConfigItemAttribute can be set in the WorkflowDecorator
    /// </summary>
    public sealed class WorkflowConfigItemAttribute : Attribute
    {
    }

    [Guid("6CA37FDE-1ADA-40A2-B315-627FDCF39CCB"),
    ComVisible(true)]
    public interface IPreConfigParameters
    {
        string OutputDirectory { get; set; }
        string ProjectDirectory { get; set; }
        IMgaProject Project { get; set; }
        int StartModeParam { get; set; }
    }

    /// <summary>
    /// Contains information from the model to display on the GUI. Must be Serializable and have ProgIdAttribute
    /// </summary>
    [Guid("2D51EDAC-2474-4BFF-A864-B3420F63481F"),
    ComVisible(true)]
    public interface IInterpreterPreConfiguration
    {
    }

    /// <summary>
    /// Contains information from the GUI or config file. Must be Serializable and have ProgIdAttribute
    /// </summary>
    [Guid("5CACBBE9-BB6F-4DD4-A4A4-B8AFE22A0285"),
    ComVisible(true)]
    public interface IInterpreterConfiguration
    {
    }

    [Guid("E5E93A3D-F422-4537-A8D1-AACD30743FAD"),
    ComVisible(true)]
    public interface IInterpreterMainParameters
    {
        MgaProject Project { get; }
        MgaFCO CurrentFCO { get; }
        MgaFCOs SelectedFCOs { get; }
        int StartModeParam { get; }
        /// <summary>
        /// Absolute path to the directory into which the interpreter puts its output files
        /// </summary>
        string OutputDirectory { get; }
        /// <summary>
        /// Absolute path the the directory that contains the Project (this is the original project under SoT)
        /// </summary>
        string ProjectDirectory { get; }
        IInterpreterConfiguration config { get; }
        bool ConsoleMessages { get; }
        CyPhyCOMInterfaces.IMgaTraceability GetTraceability();
    }

    [Guid("BAEFE1D2-9C3E-408D-86BF-AC06C650CB10"),
    ComVisible(true)]
    public interface IInterpreterResult
    {
        IMgaTraceability Traceability { get; }
        bool Success { get; }
        string RunCommand { get; }
        string LogFileDirectory { get; }
        bool ConsistencyCheckerResult { get; }
        string ZippyServerSideHook { get; }
        string Labels { get; }
        string BuildQuery { get; }
    }
}
