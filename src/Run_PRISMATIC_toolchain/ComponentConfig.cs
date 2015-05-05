using System;
using System.Runtime.InteropServices;
using GME.Util;
using GME.MGA;

namespace GME
{
    abstract class ComponentConfig
    {
        public const string paradigmName = "CyPhyML";
        public const string componentName = "Run PRISMATIC toolchain";
        public const string iconName = "prism.ico";
        public const string iconId = "prism.ico";
        public static string iconPath = null;
        public const string tooltip = "Run PRISMATIC";
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_INTERPRETER;
        public static Type typeToRegister = typeof(PRISMATIC_toolchain.Run_PRISMATIC_toolchain);
        public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Interpreter.Run_PRISMATIC_toolchain";
        public const string guid = "B82B87C6-8D28-4B04-AE7E-142BB01F09BF";
    }
}
