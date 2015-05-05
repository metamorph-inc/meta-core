using System;
using System.Runtime.InteropServices;
using GME.MGA;
using GME;
using GME.MGA.Parser;
using System.IO;
using System.Diagnostics;
using Microsoft.Win32;

namespace PRISMATIC_toolchain
{
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    public class Run_PRISMATIC_toolchain : GME.MGA.IMgaComponentEx, GME.MGA.Core.IGMEVersionInfo
    {
        private string getPath(IMgaFCO fco)
        {
            string path = fco.Name;
            while (fco.ParentModel != null)
            {
                path = fco.ParentModel.Name + "/" + path;
                fco = fco.ParentModel;
            }
            IMgaFolder folder = fco.ParentFolder;
            while (folder != null)
            {
                path = folder.Name + "/" + path;
                folder = folder.ParentFolder;
            }
            return "/" + path;
        }

        public string GetMetaPathValue()
        {
            const string localMachineRoot = "HKEY_LOCAL_MACHINE";
            const string keyName = localMachineRoot + "\\SOFTWARE\\META";

            string metaPath = (string)Registry.GetValue(keyName,
                "META_PATH", "C:\\Program Files (x86)\\META");

            return metaPath;
        }

        public void InvokeEx2(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            string projectPath = Path.GetDirectoryName(project.ProjectConnStr.Substring(4)); // skip mga=
            string projectName = Path.GetFileNameWithoutExtension(project.ProjectConnStr.Substring(4));
            string cyphyFilename = project.ProjectConnStr.Substring(4);
            baseOutputDir = Path.Combine(projectPath, projectName + "_PRISMATIC");
            GME.CSharp.GMEConsole console = GME.CSharp.GMEConsole.CreateFromProject(project);
            console.Out.WriteLine("Output directory is " + baseOutputDir);

            META_PATH = GetMetaPathValue();
            if (!Directory.Exists(META_PATH))
            {
                throw new ApplicationException("META_PATH='" + META_PATH + "' doesn't exist. Please install the META toolchain and restart GME.");
            }

            string metaPath = Path.Combine( META_PATH, "meta" );
            if (!Directory.Exists(metaPath))
            {
                throw new ApplicationException(metaPath + " doesn't exist");
            }

            ensureDir(baseOutputDir);

            string CyPhyML_udm_xml_path = Path.Combine(metaPath, "CyPhyML_udm.xml");
            if (!File.Exists(CyPhyML_udm_xml_path))
            {
                CyPhyML_udm_xml_path = Path.Combine(metaPath, @"..\generated\CyPhyML\models\CyPhyML_udm.xml");
            }
            string CyPhyML_xsd_path = Path.Combine(metaPath, "CyPhyML.xsd");
            if (!File.Exists(CyPhyML_xsd_path))
            {
                CyPhyML_xsd_path = Path.Combine(metaPath, @"..\generated\CyPhyML.xsd");
            }


            console.Out.WriteLine("Extracting XML model...");
            runProgram( Path.Combine(metaPath, @"..\bin\UdmCopy.exe"), new string[] { 
                "-f",
                cyphyFilename, 
                Path.Combine( baseOutputDir, projectName + ".xml" ), 
                CyPhyML_udm_xml_path, 
                CyPhyML_xsd_path } );

            console.Out.WriteLine("Generating Prismatic files...");
            runProgram( "C:\\Python26\\python.exe", new string[] { META_PATH + "\\bin\\Prismatic\\" + "cmc.py", Path.Combine( baseOutputDir, projectName + ".xml" ), baseOutputDir } );
            console.Out.WriteLine("Running Prismatic...");
            runProgram( "C:\\Python27\\python.exe", new string[] { baseOutputDir + "\\prismatic.py" });
            console.Out.WriteLine("Prismatic work done.");
        }
        string baseOutputDir;
        string META_PATH;
        private string getSubdir(string subdir)
        {
            return baseOutputDir + "\\" + subdir;
        }

        private void ensureDir(string subdir)
        {
            if (!Directory.Exists(subdir))
            {
                Directory.CreateDirectory(subdir);
            }
        }

        private string escapeArguments(string[] arguments)
        {
            string ret = "";
            foreach (string arg in arguments)
            {
                ret += "\"" + arg + "\" ";
            }
            return ret;
        }
        private void runProgram(string program, string[] arguments)
        {
            runProgram(program, escapeArguments(arguments));
        }

        private void runProgram(string program, string arguments)
        {
            Process p = new Process();
            // Redirect the output stream of the child process.
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = false;
            p.StartInfo.FileName = program;
            p.StartInfo.Arguments = arguments;
            try
            {
                p.Start();
            }
            catch (System.ComponentModel.Win32Exception exc)
            {
                if (program.Contains("UdmCopy"))
                {
                    throw new Exception("Could not run UdmCopy. Please install Udm, then restart GME. " + exc.Message);
                }
                throw new Exception("Could not run " + program + ": " + exc.Message);
            }
            string output = p.StandardOutput.ReadToEnd();
            p.WaitForExit();
            if (p.ExitCode != 0)
            {
                Console.Out.WriteLine(" Exit code: " + p.ExitCode.ToString() );
                p = new Process();
                p.StartInfo.FileName = "cmd.exe";
                p.StartInfo.Arguments = " /k \"\"" + program + "\" " + arguments + "\"";
                p.StartInfo.WorkingDirectory = this.baseOutputDir;
                p.Start();

                throw new ApplicationException("Running " + program + " " + arguments + " failed: " + output );
            }
        }

        private delegate void voidDelegate();

        private void performInROTransaction(IMgaProject project, voidDelegate d)
        {
            IMgaTerritory territory = project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_READ_ONLY);
            try
            {
                d();
            }
            finally
            {
                try
                {
                    project.AbortTransaction();
                }
                catch { }
                try
                {
                    territory.Destroy();
                }
                catch { }
            }

        }

        #region IMgaComponentEx Members

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            try
            {
                InvokeEx2(project, currentobj, selectedobjs, param);
            }
            finally
            {
                project = null;
                currentobj = null;
                selectedobjs = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }

        public string ComponentName
        {
            get { return GetType().Name; }
        }

        public string ComponentProgID
        {
            get
            {
                RegistrationServices regService = new RegistrationServices();
                return regService.GetProgIdForType(GetType());
            }
        }

        public componenttype_enum ComponentType
        {
            get { return GME.MGA.componenttype_enum.COMPONENTTYPE_INTERPRETER; }
        }

        public void Enable(bool newval)
        {
        }

        public void Initialize(MgaProject p)
        {
        }

        protected bool interactiveMode = false;
        public bool InteractiveMode
        {
            get
            {
                return interactiveMode;
            }
            set
            {
                interactiveMode = value;
            }
        }

        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
            InvokeEx(Project, null, selectedobjs, 0);
        }

        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        public string Paradigm
        {
            get { return ComponentConfig.paradigmName; }
        }

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
                return "csharp";

            if (Name == "path")
                return GetType().Assembly.Location;

            if (Name == "fullname")
                return GetType().FullName;

            return null;
        }

        public void set_ComponentParameter(string Name, object pVal)
        {
        }

        #endregion

        #region IGMEVersionInfo Members

        public GME.MGA.Core.GMEInterfaceVersion_enum version
        {
            get { return GME.MGA.Core.GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        #endregion

        [ComRegisterFunctionAttribute]
        public static void GMERegister(Type t)
        {
            Registrar.RegisterComponentsInGMERegistry();
        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnregister(Type t)
        {
            Registrar.UnregisterComponentsInGMERegistry();
        }
    }

}
