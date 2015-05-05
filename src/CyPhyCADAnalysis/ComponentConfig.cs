using System;
using System.Runtime.InteropServices;
using GME.Util;
using GME.MGA;

namespace GME.CSharp
{

    abstract class ComponentConfig
    {
        // Set paradigm name. Provide * if you want to register it for all paradigms.
        public const string paradigmName = "CyPhyML";

        // Set the human readable name of the interpreter. You can use white space characters.
        public const string componentName = "CyPhyCADAnalysis";

        // Specify an icon path
        public const string iconName = "CyPhyCADAnalysis.ico";

        public const string tooltip = "CyPhyCADAnalysis";

        // If null, updated with the assembly path + the iconName dynamically on registration
        public static string iconPath = null;

        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_INTERPRETER;

        public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Interpreter.CyPhyCADAnalysis";
        public const string guid = "7D170543-D1D1-4CF1-A93B-BEA1B8C89679";
    }
}
