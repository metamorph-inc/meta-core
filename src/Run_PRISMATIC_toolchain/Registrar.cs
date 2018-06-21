using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Win32;

// new GME registrar
namespace GME
{
    [ComVisible(false)]
    class RegistrationException : ApplicationException
    {
        public RegistrationException(string message) : base(message) { }
    }

    [ComVisible(false)]
    class Registrar
    {
        private Registrar()
        {
        }

        public static void RegisterComponentsInGMERegistry()
        {

            if (ComponentConfig.iconPath == null )
            {
                ComponentConfig.iconPath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + '\\' + ComponentConfig.iconName;
            }

            GME.Util.MgaRegistrar registrar = new GME.Util.MgaRegistrar();
            if ((int)GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current != (int)((GME.MGA.Core.IGMEVersionInfo)registrar).version)
            {
                throw new RegistrationException("MgaInterfaceVersion mismatch: this assembly is using " +
                    (int)GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current +
                    " but the GME interface version is " + (int)((GME.MGA.Core.IGMEVersionInfo)registrar).version +
                    "\n\nPlease install a compatible GME version or update the interop dlls.");
            }

            registrar.RegisterComponent(ComponentConfig.progID, ComponentConfig.componentType, ComponentConfig.componentName, ComponentConfig.registrationMode);
            registrar.set_ComponentExtraInfo(ComponentConfig.registrationMode, ComponentConfig.progID, "Icon", ComponentConfig.iconPath);
            registrar.set_ComponentExtraInfo(ComponentConfig.registrationMode, ComponentConfig.progID, "Tooltip", ComponentConfig.componentName);

            if (!ComponentConfig.paradigmName.Equals("*"))
            {
                foreach (String paradigm in ComponentConfig.paradigmName.Split(','))
                {
                    registrar.Associate(ComponentConfig.progID, paradigm, ComponentConfig.registrationMode);
                }
            }
        }

        public static void UnregisterComponentsInGMERegistry()
        {
            GME.Util.MgaRegistrar registrar = new GME.Util.MgaRegistrar();
            if ((int)GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current != (int)((GME.MGA.Core.IGMEVersionInfo)registrar).version)
            {
                throw new RegistrationException("MgaInterfaceVersion mismatch: this assembly is using " +
                    (int)GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current +
                    " but the GME interface version is " + (int)((GME.MGA.Core.IGMEVersionInfo)registrar).version +
                    "\n\nPlease install a compatible GME version or update the interop dlls.");
            }

            registrar.UnregisterComponent(ComponentConfig.progID, ComponentConfig.registrationMode);
        }

        public void DLLRegisterServer(int regMode)
        {
            //register dll 
            //same as:
            //post build event: C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727\regasm.exe $(TargetPath) /codebase

            RegistrationServices regAsm = new RegistrationServices();
            bool bResult = regAsm.RegisterAssembly(ComponentConfig.typeToRegister.Assembly, AssemblyRegistrationFlags.SetCodeBase);
        }
    }
}

#region Old-GME style registrar
namespace GME.CSharp
{
    [ComVisible(false)]
    public class Registrar
    {
        public Registrar()
        {
        }

        GME.Util.regaccessmode_enum regacc_translate(int x)
        {
            return (GME.Util.regaccessmode_enum)x;
        }

        public void DLLRegisterServer(int regMode)
        {
            //register dll 
            //same as:
            //post build event: C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727\regasm.exe $(TargetPath) /codebase

            RegistrationServices regAsm = new RegistrationServices();
            bool bResult = regAsm.RegisterAssembly(
                ComponentConfig.typeToRegister.Assembly,
                AssemblyRegistrationFlags.SetCodeBase);

            GME.Util.MgaRegistrar reg = new GME.Util.MgaRegistrar();
            if ((int)GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current != (int)((GME.MGA.Core.IGMEVersionInfo)reg).version)
            {
                throw new Exception("MgaInterfaceVersion mismatch: this assembly is using " +
                    (int)GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current +
                    " but the GME interface version is " + (int)((GME.MGA.Core.IGMEVersionInfo)reg).version +
                    "\n\nPlease install a compatible GME version or update the interop dlls.");
            }
            reg.RegisterComponent(
                regAsm.GetProgIdForType(ComponentConfig.typeToRegister),
                GME.MGA.componenttype_enum.COMPONENTTYPE_INTERPRETER,
                ComponentConfig.componentName,
                regacc_translate(regMode));

            reg.set_ComponentExtraInfo(regacc_translate(regMode), regAsm.GetProgIdForType(ComponentConfig.typeToRegister), "Icon", ComponentConfig.iconId);

            if (!ComponentConfig.paradigmName.Equals("*"))
            {
                reg.Associate(regAsm.GetProgIdForType(ComponentConfig.typeToRegister), ComponentConfig.paradigmName, (GME.Util.regaccessmode_enum)regMode);
            }

        }
    }
}
#endregion
