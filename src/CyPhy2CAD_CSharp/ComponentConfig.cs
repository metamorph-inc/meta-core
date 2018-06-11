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
        public const string componentName = "CyPhy2CAD";
        
		// Specify an icon path
		public const string iconName = "CyPhy2CAD.ico";

        public const string tooltip = "CyPhy2CAD";

		// If null, updated with the assembly path + the iconName dynamically on registration
        public static string iconPath = null; 
        
		// Uncomment the flag if your component is paradigm independent.
		public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_INTERPRETER;
				
        public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Interpreter.CyPhy2CAD_CSharp";
        public const string guid = "509FEBCC-1352-42A3-B59F-1081B95E5AF6";
    }
}
