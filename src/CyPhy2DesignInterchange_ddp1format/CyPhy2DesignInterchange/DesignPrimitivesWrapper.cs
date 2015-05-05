using System.Collections.Generic;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace CyPhy2DesignInterchange_ddp1format
{
    public class DesignPrimitivesWrapper
    {
        private object Primitive { get; set; }

        public DesignPrimitivesWrapper(object primitive)
        {
            Primitive = primitive;
        }

        public string Path
        {
            get
            {
                if (Primitive is CyPhy.DesignEntity)
                    return ((CyPhy.DesignEntity)Primitive).Path;
                return string.Empty;
            }
        }

        public List<CyPhy.Component> Components
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.Component>(((CyPhy.ComponentAssembly)Primitive).Children.ComponentCollection);
                else
                    return new List<CyPhy.Component>(((CyPhy.DesignContainer)Primitive).Children.ComponentCollection);
            }
        }

        public List<CyPhy.ComponentRef> ComponentRefs
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.ComponentRef>(((CyPhy.ComponentAssembly) Primitive).Children.ComponentRefCollection);
                else
                    return new List<CyPhy.ComponentRef>(((CyPhy.DesignContainer)Primitive).Children.ComponentRefCollection);
            }
        }

        public List<CyPhy.Parameter> Parameters
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.Parameter>(((CyPhy.ComponentAssembly)Primitive).Children.ParameterCollection);
                else
                    return new List<CyPhy.Parameter>(((CyPhy.DesignContainer)Primitive).Children.ParameterCollection);
            }
        }

        public List<CyPhy.Property> Properties
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.Property>(((CyPhy.ComponentAssembly)Primitive).Children.PropertyCollection);
                else
                    return new List<CyPhy.Property>(((CyPhy.DesignContainer)Primitive).Children.PropertyCollection);
            }
        }

        public List<CyPhy.ComponentAssembly> ComponentAssemblies
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.ComponentAssembly>(((CyPhy.ComponentAssembly)Primitive).Children.ComponentAssemblyCollection);
                else
                    return new List<CyPhy.ComponentAssembly>(((CyPhy.DesignContainer)Primitive).Children.ComponentAssemblyCollection);
                
            }
        }

        public List<CyPhy.DesignEntity> DesignContainers
        {
            get
            {
                if (Primitive is CyPhy.DesignContainer)
                    return new List<CyPhy.DesignEntity>(((CyPhy.DesignContainer)Primitive).Children.DesignContainerCollection);
                return new List<CyPhy.DesignEntity>();
            }
        }

        public List<CyPhy.ValueFlow> ValueFlows
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.ValueFlow>(((CyPhy.ComponentAssembly)Primitive).Children.ValueFlowCollection);
                else
                    return new List<CyPhy.ValueFlow>(((CyPhy.DesignContainer)Primitive).Children.ValueFlowCollection);
            }
        }

        public List<CyPhy.SimpleFormula> SimpleFormulas
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.SimpleFormula>(((CyPhy.ComponentAssembly)Primitive).Children.SimpleFormulaCollection);
                else
                    return new List<CyPhy.SimpleFormula>(((CyPhy.DesignContainer)Primitive).Children.SimpleFormulaCollection);
            }
        }

        public List<CyPhy.Port> Ports
        {
            get
            {
                if (Primitive is CyPhy.ComponentAssembly)
                    return new List<CyPhy.Port>(((CyPhy.ComponentAssembly)Primitive).Children.PortCollection);
                else
                    return new List<CyPhy.Port>(((CyPhy.DesignContainer)Primitive).Children.PortCollection);
            }
        }

    }
}