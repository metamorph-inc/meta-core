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
        
		// Set the human readable name of the addon. You can use white space characters.
        public const string componentName = "CyPhySignalBlocksAddOn";
        
		// Select the object events you want the addon to listen to.
        public const int eventMask = (int)(objectevent_enum.OBJEVENT_OPENMODEL | objectevent_enum.OBJEVENT_CLOSEMODEL | objectevent_enum.OBJEVENT_CREATED );
		
        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_ADDON;

		public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Addon.CyPhySignalBlocksAddOn";
        public const string guid = "0F29AE8F-B183-4013-872C-46DC0A0A68BE";        
    }
}
