using System;
using System.Runtime.InteropServices;
using GME.MGA;
using GME.Util;


namespace GME.CSharp
{

	abstract class ComponentConfig
	{
		// Set paradigm name. Provide * if you want to register it for all 
		// paradigms.
		public const string paradigmName = "CyPhyML";

		// Set the human readable name of the addon.
		// You can use white space characters.
		public const string componentName = "CyPhyDecoratorAddon";

		// Select the object events you want the addon to listen to.
		public const int eventMask = (int)(
			objectevent_enum.OBJEVENT_CREATED |
			objectevent_enum.OBJEVENT_COPIED);

		// Uncomment the flag if your component is paradigm independent.
		public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_ADDON;

		public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;

		public const string progID = "MGA.Addon.CyPhyDecoratorAddon";
		public const string guid = "E50DB0D8-7E8D-4B1C-A519-693C5DBB4CB2";
	}
}
