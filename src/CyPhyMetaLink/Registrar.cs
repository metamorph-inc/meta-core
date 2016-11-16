using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using GME;
using GME.Util;
using GME.MGA;
using GME.MGA.Core;
using Microsoft.Win32;

namespace GME.CSharp
{
    [ComVisible(false)]
    internal class RegistrationException : ApplicationException
    {
        public RegistrationException(string message) : base(message) { }
    }

    [ComVisible(false)]
    internal static class Registrar
    {
        public static void RegisterAddon()
        {
            MgaRegistrar registrar = new MgaRegistrar();
            CheckGMEInterfaceVersion(registrar);
            registrar.RegisterComponent(ComponentConfig_Addon.progID, ComponentConfig_Addon.componentType, ComponentConfig_Addon.componentName, ComponentConfig_Addon.registrationMode);

            if (!ComponentConfig_Addon.paradigmName.Equals("*"))
            {
                registrar.Associate(
                   ComponentConfig_Addon.progID,
                    ComponentConfig_Addon.paradigmName,
                    ComponentConfig_Addon.registrationMode);
            }
        }

        public static MgaRegistrar RegisterInterpreter()
        {
            if (ComponentConfig_Intf.iconPath == null)
            {
                ComponentConfig_Intf.iconPath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + '\\' + ComponentConfig_Intf.iconName;
            }

            MgaRegistrar registrar = new MgaRegistrar();
            CheckGMEInterfaceVersion(registrar);
            registrar.RegisterComponent(ComponentConfig_Intf.progID, ComponentConfig_Intf.componentType, ComponentConfig_Intf.componentName, ComponentConfig_Intf.registrationMode);
            registrar.set_ComponentExtraInfo(ComponentConfig_Intf.registrationMode, ComponentConfig_Intf.progID, "Icon", ComponentConfig_Intf.iconPath);
            registrar.set_ComponentExtraInfo(ComponentConfig_Intf.registrationMode, ComponentConfig_Intf.progID, "Tooltip", ComponentConfig_Intf.tooltip);

            if (!ComponentConfig_Intf.paradigmName.Equals("*"))
            {
                registrar.Associate(
                   ComponentConfig_Intf.progID,
                    ComponentConfig_Intf.paradigmName,
                    ComponentConfig_Intf.registrationMode);
            }
            return registrar;
        }

        private static void CheckGMEInterfaceVersion(MgaRegistrar registrar)
        {
            if ((int)GMEInterfaceVersion_enum.GMEInterfaceVersion_Current != (int)((IGMEVersionInfo)registrar).version)
            {
                throw new RegistrationException("GMEInterfaceVersion mismatch: this assembly is using " +
                    (int)GMEInterfaceVersion_enum.GMEInterfaceVersion_Current +
                    " but the GME interface version is " + (int)((IGMEVersionInfo)registrar).version +
                    "\n\nPlease install a compatible GME version or update the interop dlls.");
            }
        }

        public static void UnregisterInterpreter()
        {
            MgaRegistrar registrar = new MgaRegistrar();
            CheckGMEInterfaceVersion(registrar);

            registrar.UnregisterComponent(ComponentConfig_Intf.progID, ComponentConfig_Intf.registrationMode);
        }

        public static void UnregisterAddon()
        {
            MgaRegistrar registrar = new MgaRegistrar();
            CheckGMEInterfaceVersion(registrar);

            registrar.UnregisterComponent(ComponentConfig_Addon.progID, ComponentConfig_Addon.registrationMode);
        }
    }
}
