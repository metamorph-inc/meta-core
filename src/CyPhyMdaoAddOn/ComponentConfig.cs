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
        public const string componentName = "CyPhyMdaoAddOn";
        
		// Select the object events you want the addon to listen to.
        public const int eventMask = (int)(
					objectevent_enum.OBJEVENT_ATTR |
					objectevent_enum.OBJEVENT_CREATED |
                    objectevent_enum.OBJEVENT_NEWCHILD |
                    objectevent_enum.OBJEVENT_LOSTCHILD |
                    objectevent_enum.OBJEVENT_OPENMODEL);
		
        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_ADDON;

		public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Addon.CyPhyMdaoAddOn";
        public const string guid = "47113616-7182-4CAC-AF91-3111B23546E1";        
    }
}
