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
        public const string componentName = "CyPhy2Modelica";

        // Specify an icon path
        public const string iconName = "CyPhy2Modelica_v2.ico";

        public const string tooltip = "CyPhy2Modelica";

        // If null, updated with the assembly path + the iconName dynamically on registration
        public static string iconPath = null;

        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_INTERPRETER;

        public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Interpreter.CyPhy2Modelica_v2";
        public const string guid = "FEFB8B19-A5F1-4322-AD20-6DD731F0BE95";
    }
}
