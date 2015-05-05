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
        public const string componentName = "CADAddOn";
        
		// Select the object events you want the addon to listen to.
       // public const int eventMask = (int)(objectevent_enum.OBJEVENT_CONNECTED);
        ///*
        public const int eventMask = (int)(
            objectevent_enum.OBJEVENT_REGISTRY |
            objectevent_enum.OBJEVENT_CREATED |
            objectevent_enum.OBJEVENT_ATTR |
            objectevent_enum.OBJEVENT_PRE_DESTROYED);
        //*/

		
        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_ADDON;

		public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Addon.CADAddOn";
        public const string guid = "BA3D5C07-6DAF-4BA2-AE43-5DE3018A368B";        
    }
}
