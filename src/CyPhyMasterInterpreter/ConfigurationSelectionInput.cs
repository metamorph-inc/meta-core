using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;

namespace CyPhyMasterInterpreter
{
    public class ConfigurationSelectionOutput
    {
        /// <summary>
        /// Selected configurations.
        /// </summary>
        public GMELightObject[] SelectedConfigurations { get; set; }

        public bool PostToJobManager { get; set; }

        public bool KeepTemporaryModels { get; set; }

        public bool VerboseLogging { get; set; }

        public List<ConfigurationGroupLight> ConfigurationGroups { get; set; }
    }

    public class ConfigurationSelectionInput
    {
        /// <summary>
        /// In which context the interpreter is running.
        /// </summary>
        public GMELightObject Context { get; set; }

        /// <summary>
        /// Configuration groups.
        /// </summary>
        public ConfigurationGroupLight[] Groups { get; set; }

        public bool IsDesignSpace { get; set; }

        /// <summary>
        /// Status text about the mode of operation.
        /// </summary>
        public string OperationModeInformation { get; set; }

        /// <summary>
        /// Name of the interpreters from the workflow.
        /// </summary>
        public string[] InterpreterNames { get; set; }

        public string OutputDirectory { get; set; }
    }

    public class ConfigurationGroupLight
    {
        /// <summary>
        /// Owner of the group: either exported configurations object or null if
        /// top level system under test points to component assembly.
        /// </summary>
        public GMELightObject Owner { get; set; }

        /// <summary>
        /// CWCs or Component assemblies from which the selection can be made.
        /// </summary>
        public GMELightObject[] Configurations { get; set; }

        /// <summary>
        /// Configurations probably out of date.
        /// </summary>
        public bool IsDirty { get; set; }

        public override string ToString()
        {
            return this.Owner == null ? string.Empty : this.Owner.Name;
        }
    }

    public class ConfigurationGroup
    {
        /// <summary>
        /// Owner of the group: either exported configurations object or null if
        /// top level system under test points to component assembly.
        /// </summary>
        public IMgaModel Owner { get; set; }

        /// <summary>
        /// CWCs or Component assemblies from which the selection can be made.
        /// </summary>
        public IMgaFCOs Configurations { get; set; }

        /// <summary>
        /// Configurations probably out of date.
        /// </summary>
        public bool IsDirty { get; set; }

        public ConfigurationGroupLight ConvertToLight()
        {
            ConfigurationGroupLight configurationGroupLight = new ConfigurationGroupLight()
            {
                Configurations = this.Configurations.Cast<MgaFCO>().Select(x => GMELightObject.GetGMELightFromMgaObject(x)).ToArray(),
                IsDirty = this.IsDirty,
                Owner = this.Owner == null ? null : GMELightObject.GetGMELightFromMgaObject(this.Owner)
            };

            return configurationGroupLight;
        }
    }

    public class GMELightObject
    {
        public static string ShortenAbsPath(string absPath)
        {
            // Example:
            // input:  /@NewTesting|kind=Testing|relpos=0/@FEA_CompoundTIP_DS|kind=CADTestBench|relpos=0/@DesignContainer|kind=TopLevelSystemUnderTest|relpos=0
            // output: /@NewTesting/@FEA_CompoundTIP_DS/@DesignContainer
            return string.Join("/", absPath.Split('/').Select(x => string.Join("", x.TakeWhile(y => y != '|'))));
        }

        public string GMEId { get; set; }
        public string Name { get; set; }
        public string Type { get; set; }
        public string AbsPath { get; set; }

        public override int GetHashCode()
        {
            return this.GMEId.GetHashCode();
        }

        public string ToolTip { get; set; }

        public override string ToString()
        {
            return this.Name;
        }

        public static GMELightObject GetGMELightFromMgaObject(IMgaObject subject)
        {
            GMELightObject gmeLightObject = new GMELightObject();

            gmeLightObject.AbsPath = subject.AbsPath;
            gmeLightObject.GMEId = subject.ID;

            // TODO: figure out how to show to the user configurations when they have no uniqie names.

            gmeLightObject.Name = subject.Name;
            // TODO: set tool tip to ParentName/ObjectName
            // TODO: would be nice to use a regexp or something like that
            gmeLightObject.ToolTip = ShortenAbsPath(subject.AbsPath);
            gmeLightObject.Type = subject.MetaBase.Name;

            return gmeLightObject;
        }
    }
}
