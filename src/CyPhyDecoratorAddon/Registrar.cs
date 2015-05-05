using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using GME;
using Microsoft.Win32;
using GME.Util;
using GME.MGA.Core;

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
		public static void RegisterComponentsInGMERegistry()
		{
			MgaRegistrar registrar = new MgaRegistrar();
			CheckGMEInterfaceVersion(registrar);
			registrar.RegisterComponent(ComponentConfig.progID, ComponentConfig.componentType, ComponentConfig.componentName, ComponentConfig.registrationMode);

			if (!ComponentConfig.paradigmName.Equals("*"))
			{
				registrar.Associate(
					 ComponentConfig.progID,
						ComponentConfig.paradigmName,
						ComponentConfig.registrationMode);
			}
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


		public static void UnregisterComponentsInGMERegistry()
		{
			MgaRegistrar registrar = new MgaRegistrar();
			CheckGMEInterfaceVersion(registrar);

			registrar.UnregisterComponent(ComponentConfig.progID, ComponentConfig.registrationMode);
		}
	}
}
