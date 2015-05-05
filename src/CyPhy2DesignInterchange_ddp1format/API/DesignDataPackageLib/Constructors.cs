using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AVM
{
    public partial class Component
    {
        public Component()
        {
            this.Features = new List<AVM.Feature>();
            this.Associations = new List<AVM.Association>();
            this.Files = new List<AVM.File>();
            this.Category = new List<String>();
        }
    }
    public partial class StructuralInterface
    {
        public StructuralInterface()
        {
            this.Datums = new List<StructuralInterfaceDatum>();
            this.DefaultJoins = new List<iFAB.Join>();
            this.id = Guid.NewGuid().ToString();
        }
    }
    public partial class Associable
    {
        public Associable()
        {
            this.id = Guid.NewGuid().ToString();
        }
    }

    namespace iFAB
    {
    }

    namespace META
    {
        public partial class BehaviorModel
        {
            public BehaviorModel()
            {
                this.Interfaces = new List<Interface>();
                this.MaterialSpecs = new List<MaterialSpec>();
                this.LimitChecks = new List<LimitCheck>();
                this.Phenomena = new List<Phenomenon>();
            }
        }
        public partial class CADModel
        {
            public CADModel()
            {
                this.Datums = new List<Datum>();
                this.CADParameters = new List<CADParameter>();
                this.Metrics = new List<Metric>();
            }
        }
        public partial class AggregatePort
        {
            public AggregatePort()
            {
                this.AggregatedPorts = new List<ExternalPort>();
            }
        }
        public partial class AggregateInterface
        {
            public AggregateInterface()
            {
                this.AggregatedInterfaces = new List<AVM.META.Interface>();
            }
        }
        public partial class ArchitectureModel
        {
            public ArchitectureModel()
            {
                this.Properties = new List<Property>();
            }
        }

        namespace Design
        {   
            public partial class DesignModel
            {
                public DesignModel()
                {
                    this.Containers = new List<Container>();
                    this.Connectors = new List<Connector>();                    
                }
            }
            public abstract partial class Container
            {
                public Container()
                {
                    this.ComponentInstances = new List<ComponentInstance>();
                    this.Containers = new List<Container>();
                    this.ContainerValues = new List<ContainerValue>();
                    this.ContainerStructuralInterfaces = new List<ContainerStructuralInterface>();
                    this.id = Guid.NewGuid().ToString();
                }
            }
            public partial class ComponentInstance
            {
                public ComponentInstance()
                {
                    this.NamedValueInstances = new List<ComponentNamedValueInstance>();
                    this.PortInstances = new List<PortInstance>();
                    this.StructuralInterfaceInstances = new List<StructuralInterfaceInstance>();
                    this.id = Guid.NewGuid().ToString();
                }
            }
            public partial class AbstractPort
            {
                public AbstractPort()
                {
                    this.id = Guid.NewGuid().ToString();
                }
            }
            public partial class AbstractStructuralInterface
            {
                public AbstractStructuralInterface()
                {
                    this.id = Guid.NewGuid().ToString();
                }
            }
            public partial class ValueType
            {
                public ValueType()
                {
                    this.id = Guid.NewGuid().ToString();
                }
            }
            public partial class PortConnector
            {
                public PortConnector()
                {
                    this.EndPoints = new List<AbstractPort>();
                }
            }
            public partial class StructuralInterfaceConnector
            {
                public StructuralInterfaceConnector()
                {
                    this.Joins = new List<iFAB.Join>();
                    this.EndPoints = new List<AbstractStructuralInterface>();
                }
            }
        }
    }

}
