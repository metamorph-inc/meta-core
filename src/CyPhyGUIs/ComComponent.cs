using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Runtime.InteropServices;
using CyPhyGUIs;
using System.IO;
using System.Reflection;
using GME.Util;
using System.Xml.Serialization;
using System.Runtime.Serialization;

namespace META
{
    public class ComComponent
    {
        #region Public Properties

        /// <summary>
        /// Name of this component.
        /// </summary>
        public string ProgId { get; set; }

        public List<string> WorkflowParameterList
        {
            get
            {
                if (MgaComponent != null)
                {
                    if (MgaComponent is ICyPhyInterpreter)
                    {
                        var ret = new List<string>();
                        if (string.IsNullOrWhiteSpace((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId) == false)
                        {
                            Type configType = Type.GetTypeFromProgID((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId);
                            foreach (PropertyInfo property in configType.GetProperties())
                            {
                                if (property.GetCustomAttributes(typeof(WorkflowConfigItemAttribute), false).Length > 0)
                                {
                                    ret.Add(property.Name);
                                }
                            }
                        }
                        return ret;
                    }
                    else
                    {
                        string[] _parameterlist = MgaComponent.ComponentParameter["parameter_list"] as string[];
                        if (_parameterlist != null)
                        {
                            return _parameterlist.ToList();
                        }
                        return new List<string>();
                    }
                }
                return new List<string>();
            }
        }

        public string GetWorkflowParameterValue(string parameter)
        {
            if (MgaComponent != null)
            {
                if (MgaComponent is ICyPhyInterpreter)
                {
                    if (string.IsNullOrWhiteSpace((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId))
                    {
                        return null;
                    }
                    Type configType = Type.GetTypeFromProgID((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId);
                    foreach (PropertyInfo property in configType.GetProperties())
                    {
                        if (property.GetCustomAttributes(typeof(WorkflowConfigItemAttribute), false).Length > 0
                            && property.Name == parameter)
                        {
                            return property.GetValue(InterpreterConfig, null) as string;
                        }
                    }
                }
                else
                {
                    try
                    {
                        object value = MgaComponent.ComponentParameter[parameter];
                        if (value is string)
                            return value as string;
                    }
                    catch (COMException)
                    {
                        return null;
                    }
                }
            }
            return null;
        }

        public void SetWorkflowParameterValues()
        {
            foreach (string parameter in WorkflowParameterList)
            {
                string value;
                if (WorkflowParameters.TryGetValue(parameter, out value))
                {
                    SetWorkflowParameterValue(parameter, value);
                }
            }
        }

        private void SetWorkflowParameterValue(string parameter, string value)
        {
            if (MgaComponent != null)
            {
                if (MgaComponent is ICyPhyInterpreter)
                {
                    if (string.IsNullOrWhiteSpace((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId) == false)
                    {
                        Type configType = Type.GetTypeFromProgID((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId);
                        foreach (PropertyInfo property in configType.GetProperties())
                        {
                            if (property.GetCustomAttributes(typeof(WorkflowConfigItemAttribute), false).Length > 0
                                && property.Name == parameter)
                            {
                                property.SetValue(InterpreterConfig, value, null);
                                return;
                            }
                        }
                    }
                    throw new InterpreterException(String.Format("Invalid parameter name '{0}'", parameter));
                }
                else
                {
                    try
                    {
                        MgaComponent.ComponentParameter[parameter] = value;
                    }
                    catch (COMException)
                    {
                    }
                }
            }
        }

        public string Name { get; set; }
        public string Description { get; set; }
        public string Help { get; set; }
        public InterpreterMainParameters MainParameters { get; set; }
        private IInterpreterConfiguration _interpreterConfig;
        public IInterpreterConfiguration InterpreterConfig
        {
            get
            {
                if (_interpreterConfig == null && MgaComponent is ICyPhyInterpreter)
                {
                    if (string.IsNullOrWhiteSpace((MgaComponent as ICyPhyInterpreter).InterpreterConfigurationProgId) == false)
                    {
                        Type configType = Type.GetTypeFromProgID(((ICyPhyInterpreter)MgaComponent).InterpreterConfigurationProgId);
                        _interpreterConfig = (IInterpreterConfiguration)Activator.CreateInstance(configType);
                    }
                }
                return _interpreterConfig;
            }
            set
            {
                _interpreterConfig = value;
            }
        }
        public IInterpreterResult result { get; set; }

        /// <summary>
        /// Component parameters. These Key,Value pairs will be sent to
        /// the MgaComponent after the initialization and before the 
        /// component will be called.
        /// </summary>
        public Dictionary<string, string> WorkflowParameters { get; set; }

        /// <summary>
        /// True if the interpreter requires a current object.
        /// </summary>
        public bool CurrentObjectRequired { get; set; }

        /// <summary>
        /// Mga COM component wrapper.
        /// </summary>
        public IMgaComponentEx MgaComponent { get; set; }

        private string _tooltip;
        /// <summary>
        /// ToolTip of the component.
        /// </summary>
        public string ToolTip
        {
            get
            {
                if (_tooltip != null)
                {
                    return _tooltip;
                }
                MgaRegistrar registrar = new MgaRegistrar();
                string desc = "";
                try
                {
                    // FIXME: if the component is a C# dll this line throws a
                    // COMException.
                    desc = registrar.get_ComponentExtraInfo(
                        regaccessmode_enum.REGACCESS_BOTH,
                        ProgId,
                        "ToolTip");
                }
                catch (COMException)
                {
                    try
                    {
                        componenttype_enum Type;
                        registrar.QueryComponent(
                            ProgId,
                            out Type,
                            out desc,
                            regaccessmode_enum.REGACCESS_BOTH);
                    }
                    catch (COMException)
                    {
                        // ignore it
                        // if the dll path is empty
                    }
                }
                if (string.IsNullOrEmpty(desc))
                {
                    _tooltip = ProgId;
                    return ProgId;
                }
                else
                {
                    _tooltip = desc;
                    return desc;
                }
            }
        }

        /// <summary>
        /// Type of this COM component.
        /// </summary>
        public Type Type { get; set; }

        /// <summary>
        /// True if the component can be called throw COM.
        /// </summary>
        public bool isValid { get; set; }

        /// <summary>
        /// Indicates an Error message in a string format
        /// </summary>
        public string Error
        {
            get { return isValid ? "" : "Com object was not found."; }
        }

        #endregion

        #region Public Functions
        public void Initialize(MgaProject Project)
        {
            if (isValid == false)
            {
                return;
            }

            MgaComponent.Enable(true);
            MgaComponent.Initialize(Project);
            MgaComponent.InteractiveMode = true;

            if (MgaComponent is ICyPhyInterpreter)
            {

            }
            else
            {

                // set interpreter specific paramters
                foreach (KeyValuePair<string, string> kvp in WorkflowParameters)
                {
                    MgaComponent.ComponentParameter[kvp.Key] = kvp.Value;
                }
            }
        }

        #endregion

        public override string ToString()
        {
            if (string.IsNullOrEmpty(Name))
            {
                return ProgId;
            }
            else
            {
                return ToolTip;
                //return Name;
            }
            //return base.ToString();
        }

        #region Constructors

        public ComComponent(string name)
            : this(name, true)
        {
        }

        public ComComponent(
            string progId,
            bool currentObjectRequired)
        {
            //MGALib.IMgaProject project;
            //bool mode;
            //project.Open("MGA=...mga", out mode);
            //project.GetFCOByID("id-");
            //run_interpreter;
            //project.Close(true);

            MainParameters = new InterpreterMainParameters();
            WorkflowParameters = new Dictionary<string, string>();
            CurrentObjectRequired = currentObjectRequired;
            ProgId = progId;

            // default values
            Name = ProgId;
            Description = "Description: " + ProgId;
            Help = "Help: " + ProgId;


            if (string.IsNullOrWhiteSpace(ProgId))
            {
                isValid = false;
                return;
            }

            Type = Type.GetTypeFromProgID(ProgId);

            if (Type != null)
            {
                isValid = true;
                try
                {
                    MgaComponent = Activator.CreateInstance(Type) as IMgaComponentEx;
                }
                catch (Exception)
                {
                    //MessageBox.Show(
                    //  String.Format("Error: Creating COM component {0}." +
                    //    Environment.NewLine + "Detailed exception message: {1}",
                    //    ProgId,
                    //    ex.Message),
                    //  "Error",
                    //  MessageBoxButtons.OK,
                    //  MessageBoxIcon.Error);
                    isValid = false;
                    return;
                }

                if (MgaComponent == null)
                {
                    isValid = false;
                    return;
                }

                string _name = MgaComponent.ComponentParameter["name"] as string;
                string _description = MgaComponent.ComponentParameter["description"] as string;
                string _help = MgaComponent.ComponentParameter["help"] as string;
                if (_name != null)
                {
                    Name = _name;
                }
                if (_description != null)
                {
                    Description = _description;
                }
                if (_help != null)
                {
                    Help = _help;
                }
            }
            else
            {
                isValid = false;
            }
        }

        #endregion

        ~ComComponent()
        {
            MgaComponent = null;
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

        public bool DoGUIConfiguration(string projectDir, bool showGUI=true)
        {
            if (MgaComponent == null)
            {
                throw new ApplicationException(Error);
            }
            bool success = false;
            if (MgaComponent is ICyPhyInterpreter)
            {
                PreConfigArgs preConfigArgs = new PreConfigArgs();
                preConfigArgs.ProjectDirectory = projectDir;
                // FIXME: fill in other preConfigArgs
                IInterpreterPreConfiguration preConfig = (MgaComponent as ICyPhyInterpreter).PreConfig(preConfigArgs);
                if (InterpreterConfig != null)
                {
                    this.InterpreterConfig = META.ComComponent.DeserializeConfiguration(projectDir, InterpreterConfig.GetType(), ProgId);
                    // Set configuration based on Workflow Parameters. This will override all [WorkflowConfigItem] members.
                    this.SetWorkflowParameterValues();
                }
                else
                {
                    success = true;
                    return success;
                }

                if (showGUI == false)
                {
                    return true;
                }
                var newConfig = (MgaComponent as ICyPhyInterpreter).DoGUIConfiguration(preConfig, this.InterpreterConfig); 
                
                if (newConfig != null)
                {
                    success = true;
                    InterpreterConfig = newConfig;
                    // save the updated configuration that we got if they hit OK
                    META.ComComponent.SerializeConfiguration(projectDir, InterpreterConfig, ProgId);
                }
            }
            else
            {
                success = true;
                if (showGUI)
                {
                    MgaComponent.ComponentParameter["do_config_now"] = projectDir;
                }
                this.SetWorkflowParameterValues();
            }

            return success;
        }

        public void Main()
        {
            if (CurrentObjectRequired == true &&
                MainParameters.CurrentFCO == null)
            {
                throw new InterpreterException("Current object is not set for COM object.");
            }
            if (MgaComponent == null)
            {
                throw new ArgumentNullException("MgaComponent");
            }
            if (MgaComponent is ICyPhyInterpreter)
            {
                MainParameters.config = InterpreterConfig;
                result = ((ICyPhyInterpreter)MgaComponent).Main(MainParameters);
            }
            else
            {
                // set up the interpreter specific parameters
                if (ProgId == "MGA.Decorator.CyPhy2CAD")
                {
                    MgaComponent.ComponentParameter["generateCAD"] = "true";
                }
                MgaComponent.ComponentParameter["automation"] = "true";
                MgaComponent.ComponentParameter["output_dir"] = MainParameters.OutputDirectory;
                MgaComponent.ComponentParameter["console_messages"] = MainParameters.ConsoleMessages ? "on" : "off";
                MgaComponent.ComponentParameter["original_project_file"] = Path.Combine(MainParameters.ProjectDirectory, "fake.mga");
                MgaComponent.ComponentParameter["do_config"] = "false";

                MgaComponent.InvokeEx(
                    MainParameters.Project,
                    MainParameters.CurrentFCO,
                    MainParameters.SelectedFCOs,
                    MainParameters.StartModeParam);

                InterpreterResult result = new InterpreterResult();
                result.Labels = MgaComponent.ComponentParameter["labels"] as string;
                result.RunCommand = MgaComponent.ComponentParameter["runCommand"] as string;
                result.Success = true;
                result.ZippyServerSideHook = MgaComponent.ComponentParameter["results_zip_py"] as string;
                // FIXME
                result.LogFileDirectory = Path.Combine(MainParameters.ProjectDirectory, "log");
                result.BuildQuery = MgaComponent.ComponentParameter["build_query"] as string;
                this.result = result;
            }
            if (this.result.Success == false)
            {
                throw new InterpreterException(this.Name + " execution failed");
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="projectDir"></param>
        /// <param name="config"></param>
        /// <param name="progId">ProgID of the interpreter</param>
        public static void SerializeConfiguration(string projectDir, IInterpreterConfiguration config, string progId)
        {
            XmlSerializer serializer = new XmlSerializer(config.GetType());
            Directory.CreateDirectory(Path.Combine(projectDir, "config"));
            using (FileStream output = new FileStream(Path.Combine(projectDir, "config", progId + ".xml"), FileMode.Create))
            {
                serializer.Serialize(output, config);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="projectDir"></param>
        /// <param name="configType"></param>
        /// <param name="progId">ProgId of the interpreter</param>
        /// <returns></returns>
        public static IInterpreterConfiguration DeserializeConfiguration(string projectDir, Type configType, string progId)
        {
            IInterpreterConfiguration config = null;
            XmlSerializer serializer = new XmlSerializer(configType);
            try
            {
                using (FileStream input = File.Open(Path.Combine(projectDir, "config", progId + ".xml"), FileMode.Open))
                {
                    config = (IInterpreterConfiguration)serializer.Deserialize(input);
                }
            }
            catch (FileNotFoundException)
            {
            }
            catch (DirectoryNotFoundException)
            {
            }
            catch (SerializationException)
            {
                // TODO log
            }
            catch (InvalidOperationException)
            {
                // TODO log
            }

            return config;
        }
    }
}
