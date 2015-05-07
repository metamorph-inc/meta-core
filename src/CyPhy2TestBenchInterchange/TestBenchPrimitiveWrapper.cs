using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace CyPhy2TestBenchInterchange
{
    public class TestBenchPrimitiveWrapper
    {
        private object _primitive;

        public object Primitive { get { return _primitive; } }

        public TestBenchPrimitiveWrapper(object primitive)
        {
            if (primitive is CyPhy.ComponentAssembly ||
                primitive is CyPhy.DesignContainer)
                _primitive = primitive;
            else
                throw new NotImplementedTypeException();
        }

        public string ConfigurationUniqueID
        {
            get
            {
                if (_primitive is CyPhy.ComponentAssembly)
                {
                    return ((CyPhy.ComponentAssembly)_primitive).Attributes.ConfigurationUniqueID;
                }
                else if (_primitive is CyPhy.DesignContainer)
                {
                    return string.Empty;//((CyPhy.DesignContainer)_primitive).Attributes.;
                }
                return string.Empty;
            }
        }

        public int ID
        {
            get
            {
                if (_primitive is CyPhy.ComponentAssembly)
                {
                    return ((CyPhy.ComponentAssembly)_primitive).Attributes.ID;
                }
                else if (_primitive is CyPhy.DesignContainer)
                {
                    return ((CyPhy.DesignContainer)_primitive).Attributes.ID;
                }
                return -1;
            }
        }

        private Dictionary<object, List<CyPhy.Parameter>> _parameterCache = new Dictionary<object, List<CyPhy.Parameter>>();
        public IEnumerable<CyPhy.Parameter> ParameterCollection
        {
            get
            {
                if (!_parameterCache.ContainsKey(Primitive))
                {
                    _parameterCache[Primitive] = new List<CyPhy.Parameter>();
                    if (_primitive is CyPhy.ComponentAssembly)
                        _parameterCache[Primitive] = new List<CyPhy.Parameter>(((CyPhy.ComponentAssembly)_primitive).Children.ParameterCollection);
                    else if (_primitive is CyPhy.DesignContainer)
                        _parameterCache[Primitive] = new List<CyPhy.Parameter>(((CyPhy.DesignContainer)_primitive).Children.ParameterCollection);
                }

                return _parameterCache[Primitive];
            }
        }

        private Dictionary<object, List<CyPhy.Property>> _propertyCache = new Dictionary<object,List<CyPhy.Property>>();
        public IEnumerable<CyPhy.Property> PropertyCollection
        {
            get
            {
                if (!_propertyCache.ContainsKey(Primitive))
                {
                    _propertyCache[Primitive] = new List<CyPhy.Property>();
                    if (_primitive is CyPhy.ComponentAssembly)
                        _propertyCache[Primitive] = new List<CyPhy.Property>(((CyPhy.ComponentAssembly)_primitive).Children.PropertyCollection);
                    else if (_primitive is CyPhy.DesignContainer)
                        _propertyCache[Primitive] = new List<CyPhy.Property>(((CyPhy.DesignContainer)_primitive).Children.PropertyCollection);
                }

                return _propertyCache[Primitive];
            }
        }

        private Dictionary<object, List<CyPhy.Connector>> _connectorCache = new Dictionary<object, List<CyPhy.Connector>>();
        public IEnumerable<CyPhy.Connector> ConnectorCollection
        {
            get
            {
                if (!_connectorCache.ContainsKey(Primitive))
                {
                    _connectorCache[Primitive] = new List<CyPhy.Connector>();
                    if (_primitive is CyPhy.ComponentAssembly)
                        _connectorCache[Primitive] = new List<CyPhy.Connector>(((CyPhy.ComponentAssembly)_primitive).Children.ConnectorCollection);
                    else if (_primitive is CyPhy.DesignContainer)
                        _connectorCache[Primitive] = new List<CyPhy.Connector>(((CyPhy.DesignContainer)_primitive).Children.ConnectorCollection);
                }

                return _connectorCache[Primitive];
            }
        }

        private Dictionary<object, List<CyPhy.ModelicaConnector>> _modelicaConnectorCache = new Dictionary<object,List<CyPhy.ModelicaConnector>>();
        public IEnumerable<CyPhy.ModelicaConnector> ModelicaConnectorCollection
        {
            get
            {
                if (!_modelicaConnectorCache.ContainsKey(Primitive))
                {
                    _modelicaConnectorCache[Primitive] = new List<CyPhy.ModelicaConnector>();
                    if (_primitive is CyPhy.ComponentAssembly)
                        _modelicaConnectorCache[Primitive] = new List<CyPhy.ModelicaConnector>(((CyPhy.ComponentAssembly)_primitive).Children.ModelicaConnectorCollection);
                    else if (_primitive is CyPhy.DesignContainer)
                        _modelicaConnectorCache[Primitive] = new List<CyPhy.ModelicaConnector>(((CyPhy.DesignContainer)_primitive).Children.ModelicaConnectorCollection);
                }

                return _modelicaConnectorCache[Primitive];
            }
        }
    }

    public class NotImplementedTypeException : Exception
    {
    }
}
