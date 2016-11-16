using System;
using System.Runtime.InteropServices;
using GME.Util;
using GME.MGA;

namespace GME.CSharp
{
    abstract class ComponentConfig_Intf
    {
        // Set paradigm name. Provide * if you want to register it for all paradigms.
        public const string paradigmName = "CyPhyML";

        // Set the human readable name of the interpreter. You can use white space characters.
        public const string componentName = "Meta-Link";

        // Specify an icon path
        public const string iconName = "CyPhyMLSync.ico";

        public const string tooltip = "Meta-Link";

        // If null, updated with the assembly path + the iconName dynamically on registration
        public static string iconPath = null;


        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_INTERPRETER;

        public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Interpreter.CyPhyMetaLink";
        public const string guid = "E9416E5D-3DBB-48E7-B07C-8EBFBBE15676";
    }
}
