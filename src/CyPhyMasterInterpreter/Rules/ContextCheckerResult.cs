namespace CyPhyMasterInterpreter.Rules
{
    using System;
    using System.Runtime.InteropServices;
    using GME.MGA;

    /// <summary>
    /// Result of a context check.
    /// </summary>
    [Guid("45F66501-A810-4CC0-8C5C-DFDC2CA6FA93"),
    ProgId("CyPhyMasterInterpreter.Rules.ContextCheckerResult"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class ContextCheckerResult
    {
        /// <summary>
        /// True if check passed, otherwise false.
        /// </summary>
        public bool Success { get; set; }

        /// <summary>
        /// GME object that cases the problem or the context where the problem occured.
        /// </summary>
        public IMgaObject Subject { get; set; }

        /// <summary>
        /// Detalied message about the check and hints how to fix the problem or what was checked.
        /// </summary>
        public string Message { get; set; }
    }
}
