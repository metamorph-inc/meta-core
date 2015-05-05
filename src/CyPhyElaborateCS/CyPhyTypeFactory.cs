namespace CyPhyElaborateCS
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA.Meta;

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
            this.TestBenchMeta = rootMeta.RootFolder.DefinedFCOByName["TestBench", true].MetaRef;
            this.CADTestBenchMeta = rootMeta.RootFolder.DefinedFCOByName["CADTestBench", true].MetaRef;
            this.BlastTestBenchMeta = rootMeta.RootFolder.DefinedFCOByName["BlastTestBench", true].MetaRef;
            this.BallisticTestBenchMeta = rootMeta.RootFolder.DefinedFCOByName["BallisticTestBench", true].MetaRef;
            this.CFDTestBenchMeta = rootMeta.RootFolder.DefinedFCOByName["CFDTestBench", true].MetaRef;
            this.KinematicTestBenchMeta = rootMeta.RootFolder.DefinedFCOByName["KinematicTestBench", true].MetaRef;

            this.ComponentMeta = rootMeta.RootFolder.DefinedFCOByName["Component", true].MetaRef;
            this.ComponentRefMeta = rootMeta.RootFolder.DefinedFCOByName["ComponentRef", true].MetaRef;
            this.ComponentAssemblyMeta = rootMeta.RootFolder.DefinedFCOByName["ComponentAssembly", true].MetaRef;

            this.DesignContainerMeta = rootMeta.RootFolder.DefinedFCOByName["DesignContainer", true].MetaRef;
            this.CWCMeta = rootMeta.RootFolder.DefinedFCOByName["CWC", true].MetaRef;
            this.ConfigurationsMeta = rootMeta.RootFolder.DefinedFCOByName["Configurations", true].MetaRef;

            // will be used to query is derived from relationship
            this.TestInjectionPointsMeta = new HashSet<int>();
            this.TestInjectionPointsMeta.Add(rootMeta.RootFolder.DefinedFCOByName["TestInjectionPoint", true].MetaRef);
            this.TestInjectionPointsMeta.Add(rootMeta.RootFolder.DefinedFCOByName["BallisticTarget", true].MetaRef);
            this.TestInjectionPointsMeta.Add(rootMeta.RootFolder.DefinedFCOByName["CriticalComponent", true].MetaRef);
        }

        /// <summary>
        /// Gets MetaRef id of TestBench
        /// </summary>
        public int TestBenchMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of CADTestBench
        /// </summary>
        public int CADTestBenchMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of BlastTestBench
        /// </summary>
        public int BlastTestBenchMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of BallisticTestBench
        /// </summary>
        public int BallisticTestBenchMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of CFDTestBench
        /// </summary>
        public int CFDTestBenchMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of KinematicTestBench
        /// </summary>
        public int KinematicTestBenchMeta { get; private set; }

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
        /// Gets MetaRef id of DesignContainer
        /// </summary>
        public int DesignContainerMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of Configurations
        /// </summary>
        public int ConfigurationsMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef id of CWC
        /// </summary>
        public int CWCMeta { get; private set; }

        /// <summary>
        /// Gets MetaRef ids of TestInjectionPoint elements
        /// </summary>
        public HashSet<int> TestInjectionPointsMeta { get; private set; }
    }
}
