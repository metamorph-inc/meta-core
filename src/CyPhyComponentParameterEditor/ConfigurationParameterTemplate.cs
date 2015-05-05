using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;


namespace CyPhyComponentParameterEditor
{
    public class ConfigurationParameterTemplate
    {
        public static readonly ISet<string> SupportedKinds = new HashSet<string>()
        {
            typeof(CyPhy.TestBench).Name,
            typeof(CyPhy.BlastTestBench).Name,
            typeof(CyPhy.BallisticTestBench).Name,
            typeof(CyPhy.CADTestBench).Name,
            typeof(CyPhy.CFDTestBench).Name,
            typeof(CyPhy.TestBenchSuite).Name,
        };

        public static readonly List<ConfigurationParameterTemplate> Templates = new List<ConfigurationParameterTemplate>();

        static ConfigurationParameterTemplate()
        {
            string[] alltestbenches = new string[] { typeof(CyPhy.TestBench).Name, typeof(CyPhy.BallisticTestBench).Name, typeof(CyPhy.BlastTestBench).Name, typeof(CyPhy.CADTestBench).Name, typeof(CyPhy.CFDTestBench).Name };
            Templates.Add(new ConfigurationParameterTemplate("INTERFERENCE_CHECK", CyPhyClasses.Parameter.AttributesClass.DataType_enum.Integer, "0", "Controls whether interference checks will be calculated on the testbench output. Interference check may take a long time to execute so by default it is turned off.  It can be turned on by setting this parameter to 1.", true, alltestbenches));
            Templates.Add(new ConfigurationParameterTemplate("MAKE_REP", CyPhyClasses.Parameter.AttributesClass.DataType_enum.String, "", "Defines the name of the model representation for a Component within the Creo part/assembly file if the Component is a \"Buy\" component.", true, alltestbenches));
            Templates.Add(new ConfigurationParameterTemplate("BUY_REP", CyPhyClasses.Parameter.AttributesClass.DataType_enum.String, "", "Defines the name of the model representation for a Component within the Creo part/assembly file if the Component is a \"Make\" component.", true, alltestbenches));
            Templates.Add(new ConfigurationParameterTemplate("DEFAULT_REP", CyPhyClasses.Parameter.AttributesClass.DataType_enum.String, "", "Defines the name of the model representation for a Component within the Creo part/assembly file if the procurement menthod is not set on a Component.", true, alltestbenches));
            Templates.Add(new ConfigurationParameterTemplate("CADEXEPARAMS", CyPhyClasses.Parameter.AttributesClass.DataType_enum.String, "", "Parameter string concatenated to the CreateAssembly.exe command line. For the detailed description of command line parameters, see CreateAssembly.exe documentation.", true, new string[] { typeof(CyPhy.ComponentAssembly).Name }));
            Templates.Add(new ConfigurationParameterTemplate("SPECIALINSTRUCTIONS", CyPhyClasses.Parameter.AttributesClass.DataType_enum.String, "", "List of special instructions (comma-separated) for CAD Assembler.", true, new string[] { typeof(CyPhy.ComponentAssembly).Name, typeof(CyPhy.Component).Name }));
        }

        public readonly string Name;
        public readonly CyPhyClasses.Parameter.AttributesClass.DataType_enum Type;
        public readonly string DefaultValue;
        public readonly ISet<string> AppliesTo = new HashSet<string>();
        public readonly string HelpText;
        public readonly bool Unique;
        public ConfigurationParameterTemplate(string name, CyPhyClasses.Parameter.AttributesClass.DataType_enum type, string defaultvalue, string helptext, bool unique, params string[] containers)
        {
            this.Name = name;
            this.Type = type;
            this.DefaultValue = defaultvalue;
            this.HelpText = helptext;
            this.Unique = unique;
            foreach (string s in containers)
            {
                AppliesTo.Add(s);
            }
        }

        public CyPhy.Parameter Create(MgaFCO parent)
        {
            if (typeof(CyPhy.CFDTestBench).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.CFDTestBench.Cast(parent));
            }
            else if (typeof(CyPhy.TestBench).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.TestBench.Cast(parent));
            }
            else if (typeof(CyPhy.CADTestBench).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.CADTestBench.Cast(parent));
            }
            else if (typeof(CyPhy.BallisticTestBench).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.BallisticTestBench.Cast(parent));
            }
            else if (typeof(CyPhy.BlastTestBench).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.BlastTestBench.Cast(parent));
            }
            else if (typeof(CyPhy.ComponentAssembly).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.ComponentAssembly.Cast(parent));
            }
            else if (typeof(CyPhy.Component).Name == parent.MetaBase.Name)
            {
                return Create(CyPhyClasses.Component.Cast(parent));
            }
            else
            {
                throw new ArgumentException("Can't create parameter for " + parent.MetaBase.Name);
            }
        }

        public CyPhy.Parameter Create(ISIS.GME.Common.Interfaces.Container container)
        {
            CyPhy.Parameter result = ISIS.GME.Common.Utils.CreateObject<ISIS.GME.Dsml.CyPhyML.Classes.Parameter>(container, "" + ISIS.GME.Dsml.CyPhyML.Classes.Parameter.DefaultRole.Parameter.ToString());
            result.Name = Name;
            result.Attributes.Value = DefaultValue;
            result.Attributes.DataType = Type;
            return result;
        }

    }
}
