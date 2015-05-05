namespace CyPhy2Modelica_v2
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA.Meta;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

    /// <summary>
    /// Factory class to query meta reference ids and compare them instead of strings.
    /// </summary>
    public class CyPhyTypeFactory
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="CyPhyTypeFactory"/> class.
        /// </summary>
        /// <param name="rootMeta">Given meta model</param>
        public CyPhyTypeFactory(MgaMetaProject rootMeta)
        {
            this.ComponentMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.Component).Name, true].MetaRef;
            this.ComponentRefMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.ComponentRef).Name, true].MetaRef;
            this.ComponentAssemblyMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.ComponentAssembly).Name, true].MetaRef;
            this.TestComponentMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.TestComponent).Name, true].MetaRef;
            this.TestBenchMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.TestBench).Name, true].MetaRef;
            this.ConnectorMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.Connector).Name, true].MetaRef;
            this.ModelicaConnectorMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.ModelicaConnector).Name, true].MetaRef;
            this.ModelicaModelMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.ModelicaModel).Name, true].MetaRef;
            this.ParameterMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.Parameter).Name, true].MetaRef;
            this.PropertyMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.Property).Name, true].MetaRef;
            this.PostProcessingMeta = rootMeta.RootFolder.DefinedFCOByName[typeof(CyPhy.PostProcessing).Name, true].MetaRef;
        }

        /// <summary>
        /// Gets MetaRef id of Component
        /// </summary>
        public int ComponentMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of ComponentRef
        /// </summary>
        public int ComponentRefMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of ComponentAssembly
        /// </summary>
        public int ComponentAssemblyMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of TestComponent
        /// </summary>
        public int TestComponentMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of TestBench
        /// </summary>
        public int TestBenchMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of Connector
        /// </summary>
        public int ConnectorMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of ModelicaConnector
        /// </summary>
        public int ModelicaConnectorMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of ModelicaModel
        /// </summary>
        public int ModelicaModelMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of Parameter
        /// </summary>
        public int ParameterMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of Property
        /// </summary>
        public int PropertyMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of PostProcessing
        /// </summary>
        public int PostProcessingMeta { get; private set; }
    }
}
