namespace GME.CSharp
{
    using System;
    using System.Runtime.InteropServices;
    using GME.MGA.Core;
    using GME.Util;

    /// <summary>
    /// Used to register COM component in GME registry.
    /// </summary>
    [ComVisible(false)]
    internal static class Registrar
    {
        /// <summary>
        /// Registers component in the GME registry.
        /// </summary>
        /// <exception cref="RegistrationException">If GME Interface version does not match.</exception>
        public static void RegisterComponentsInGMERegistry()
        {
            if (ComponentConfig.IconPath == null)
            {
                ComponentConfig.IconPath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location) + '\\' + ComponentConfig.IconName;
            }

            MgaRegistrar registrar = new MgaRegistrar();
            
            Registrar.CheckGMEInterfaceVersion(registrar);

            registrar.RegisterComponent(ComponentConfig.ProgID, ComponentConfig.ComponentType, ComponentConfig.ComponentName, ComponentConfig.RegistrationMode);
            registrar.set_ComponentExtraInfo(ComponentConfig.RegistrationMode, ComponentConfig.ProgID, "Icon", ComponentConfig.IconPath);
            registrar.set_ComponentExtraInfo(ComponentConfig.RegistrationMode, ComponentConfig.ProgID, "Tooltip", ComponentConfig.Tooltip);

            if (!ComponentConfig.ParadigmName.Equals("*"))
            {
                registrar.Associate(
                   ComponentConfig.ProgID,
                    ComponentConfig.ParadigmName,
                    ComponentConfig.RegistrationMode);
            }
        }

        /// <summary>
        /// Unregisters this component from the GME Registry.
        /// </summary>
        /// <exception cref="RegistrationException">If GME Interface version does not match.</exception>
        public static void UnregisterComponentsInGMERegistry()
        {
            MgaRegistrar registrar = new MgaRegistrar();
            CheckGMEInterfaceVersion(registrar);

            registrar.UnregisterComponent(ComponentConfig.ProgID, ComponentConfig.RegistrationMode);
        }

        /// <summary>
        /// Checks GME interface version
        /// </summary>
        /// <param name="registrar">Registrar to get the current version of GME, which is used to register the component.</param>
        /// <exception cref="RegistrationException">If GME Interface version does not match.</exception>
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
    }

    /// <summary>
    /// Represents errors that occur during component registration.
    /// </summary>
    [ComVisible(false)]
    internal class RegistrationException : ApplicationException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RegistrationException"/> class with a specified
        /// error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        public RegistrationException(string message)
            : base(message)
        {
        }
    }
}