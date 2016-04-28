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
        public const string componentName = "ComponentLibraryManagerAddOn";
        
		// Select the object events you want the addon to listen to.
        public const int eventMask = (int)(objectevent_enum.OBJEVENT_CREATED) | (int)(objectevent_enum.OBJEVENT_ATTR);
		
        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_ADDON;

		public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_BOTH;
        public const string progID = "MGA.Addon.ComponentLibraryManagerAddOn";
        public const string guid = "F180C488-42D8-4BC4-8E26-D464F8187E67";        
    }
}
