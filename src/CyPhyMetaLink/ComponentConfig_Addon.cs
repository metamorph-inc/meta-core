using System;
using System.Runtime.InteropServices;
using GME.Util;
using GME.MGA;

namespace GME.CSharp
{
    abstract class ComponentConfig_Addon
    {
        // Set paradigm name. Provide * if you want to register it for all paradigms.
        public const string paradigmName = "CyPhyML";

        // Set the human readable name of the addon. You can use white space characters.
        public const string componentName = "CyPhyMLPropagate";

        // Select the object events you want the addon to listen to.
        //public const int eventMask = (int)(objectevent_enum.OBJEVENT_PRE_STATUS | objectevent_enum.OBJEVENT_CREATED | objectevent_enum.OBJEVENT_ATTR | objectevent_enum.OBJEVENT_PROPERTIES | objectevent_enum.OBJEVENT_CONNECTED | objectevent_enum.OBJEVENT_OPENMODEL | objectevent_enum.OBJEVENT_CLOSEMODEL);
        public const int eventMask = (int)(objectevent_enum.OBJEVENT_CREATED | objectevent_enum.OBJEVENT_ATTR | objectevent_enum.OBJEVENT_PROPERTIES
            | objectevent_enum.OBJEVENT_CONNECTED | objectevent_enum.OBJEVENT_OPENMODEL | objectevent_enum.OBJEVENT_CLOSEMODEL
            | objectevent_enum.OBJEVENT_NEWCHILD | objectevent_enum.OBJEVENT_PRE_STATUS | objectevent_enum.OBJEVENT_RELATION | objectevent_enum.OBJEVENT_PRE_DESTROYED);

        // Uncomment the flag if your component is paradigm independent.
        public static componenttype_enum componentType = componenttype_enum.COMPONENTTYPE_ADDON;

        public const regaccessmode_enum registrationMode = regaccessmode_enum.REGACCESS_SYSTEM;
        public const string progID = "MGA.Addon.CyPhyMLPropagate";
        public const string guid = "4E6BDD25-67AE-4A03-9FB8-B5A097B29B09";
    }
}
