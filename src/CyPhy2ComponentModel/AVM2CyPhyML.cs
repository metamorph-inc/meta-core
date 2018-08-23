using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using avm;
using CyPhy2ComponentModel;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Meta;
// using domain specific interfaces
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace AVM2CyPhyML
{
    using System.Diagnostics;
    using System.Text.RegularExpressions;
    using TypePair = KeyValuePair<Type, Type>;

    public abstract class AVM2CyPhyMLBuilder
    {
        public AVM2CyPhyMLBuilder(CyPhyML.RootFolder rootFolder, object messageConsoleParameter = null)
        {
            _cyPhyMLRootFolder = rootFolder;
            messageConsole = messageConsoleParameter;
        }

        protected IMgaProject project { get { return _cyPhyMLRootFolder.Impl.Project; } }
        protected Dictionary<String, CyPhyML.unit> _unitSymbolCyPhyMLUnitMap = new Dictionary<string, CyPhyML.unit>();

        protected CyPhyML.RootFolder _cyPhyMLRootFolder;
        protected List<CyPhyML.Units> _cyPhyMLUnitsFolders;
        protected CyPhyML.Ports _cyPhyMLPorts;

        /// <summary>
        /// Tracks whether any incoming members have layout data.
        /// If not, we'll run a layout algorithm at the end.
        /// </summary>
        protected Boolean aMemberHasLayoutData = false;

        protected Dictionary<object, Object> _avmCyPhyMLObjectMap = new Dictionary<object, object>();
        protected Dictionary<string, avm.PortMapTarget> _avmPortIDMap = new Dictionary<string, avm.PortMapTarget>();
        protected HashSet<avm.PortMapTarget> _avmPortSet = new HashSet<avm.PortMapTarget>();
        protected Dictionary<string, avm.Resource> _avmResourceIDMap = new Dictionary<string, avm.Resource>();
        protected Dictionary<string, KeyValuePair<avm.ValueNode, object>> _avmValueNodeIDMap = new Dictionary<string, KeyValuePair<ValueNode, object>>();
        protected HashSet<KeyValuePair<avm.ValueNode, object>> _avmValueNodeSet = new HashSet<KeyValuePair<ValueNode, object>>();
        private object _messageConsole = null;

        protected static readonly Regex cadResourceRegex = new Regex("^(.*)(\\.prt|\\.asm)\\.([0-9]*)$", RegexOptions.IgnoreCase);

        protected Dictionary<String, CreateMethodProxyBase> _cyPhyMLNameCreateMethodMap = new Dictionary<String, CreateMethodProxyBase>() {
            { typeof(avm.cad.Axis).ToString(),                         CreateMethodProxy<CyPhyMLClasses.Axis>.get_singleton() },
            { typeof(avm.cad.CoordinateSystem).ToString(),             CreateMethodProxy<CyPhyMLClasses.CoordinateSystem>.get_singleton() },
            { typeof(avm.cad.Plane).ToString(),                        CreateMethodProxy<CyPhyMLClasses.Surface>.get_singleton() },
            { typeof(avm.cad.Point).ToString(),                        CreateMethodProxy<CyPhyMLClasses.Point>.get_singleton() },
            { typeof(avm.AbstractPort).ToString(),                     CreateMethodProxy<CyPhyMLClasses.AbstractPort>.get_singleton() },
            { typeof(avm.modelica.Connector).ToString(),               CreateMethodProxy<CyPhyMLClasses.ModelicaConnector>.get_singleton() },
            { typeof(avm.SecurityClassification).ToString(),           CreateMethodProxy<CyPhyMLClasses.SecurityClassification>.get_singleton() },
            { typeof(avm.Proprietary).ToString(),                      CreateMethodProxy<CyPhyMLClasses.Proprietary>.get_singleton() },
            { typeof(avm.ITAR).ToString(),                             CreateMethodProxy<CyPhyMLClasses.ITAR>.get_singleton() },
            { typeof(avm.DoDDistributionStatement).ToString(),         CreateMethodProxy<CyPhyMLClasses.DoDDistributionStatement>.get_singleton() } 

            //{ typeof( CyPhyMLClasses.Axis ).Name,                      CreateMethodProxy<CyPhyMLClasses.Axis>.get_singleton() },
            //{ typeof( CyPhyMLClasses.Surface ).Name,                   CreateMethodProxy<CyPhyMLClasses.Surface>.get_singleton() },
            //{ typeof( CyPhyMLClasses.Point ).Name,                     CreateMethodProxy<CyPhyMLClasses.Point>.get_singleton() },
            //{ typeof( CyPhyMLClasses.CoordinateSystem ).Name,          CreateMethodProxy<CyPhyMLClasses.CoordinateSystem>.get_singleton() },
            //{ typeof( CyPhyMLClasses.AxisGeometry ).Name,              CreateMethodProxy<CyPhyMLClasses.AxisGeometry>.get_singleton() },
            //{ typeof( CyPhyMLClasses.SurfaceGeometry ).Name,           CreateMethodProxy<CyPhyMLClasses.SurfaceGeometry>.get_singleton() },
            //{ typeof( CyPhyMLClasses.PointGeometry ).Name,             CreateMethodProxy<CyPhyMLClasses.PointGeometry>.get_singleton() },
            //{ typeof( CyPhyMLClasses.CoordinateSystemGeometry ).Name,  CreateMethodProxy<CyPhyMLClasses.CoordinateSystemGeometry>.get_singleton() },
            //{ typeof( CyPhyMLClasses.ElectricalPin ).Name,             CreateMethodProxy<CyPhyMLClasses.ElectricalPin>.get_singleton() },
            //{ typeof( CyPhyMLClasses.RotationalFlange ).Name,          CreateMethodProxy<CyPhyMLClasses.RotationalFlange>.get_singleton() },
            //{ typeof( CyPhyMLClasses.FluidPort ).Name,                 CreateMethodProxy<CyPhyMLClasses.FluidPort>.get_singleton() },
            //{ typeof( CyPhyMLClasses.MultibodyFrame ).Name,            CreateMethodProxy<CyPhyMLClasses.MultibodyFrame>.get_singleton() },
            //{ typeof( CyPhyMLClasses.HeatPort ).Name,                  CreateMethodProxy<CyPhyMLClasses.HeatPort>.get_singleton() },
            //{ typeof( CyPhyMLClasses.TranslationalFlange ).Name,       CreateMethodProxy<CyPhyMLClasses.TranslationalFlange>.get_singleton() },
            //{ typeof( CyPhyMLClasses.FlowPort ).Name,                  CreateMethodProxy<CyPhyMLClasses.FlowPort>.get_singleton() },
            //{ typeof( CyPhyMLClasses.AggregatePort ).Name,             CreateMethodProxy<CyPhyMLClasses.AggregatePort>.get_singleton() },
            //{ typeof( CyPhyMLClasses.RealInput ).Name,                 CreateMethodProxy<CyPhyMLClasses.RealInput>.get_singleton() },
            //{ typeof( CyPhyMLClasses.IntegerInput ).Name,              CreateMethodProxy<CyPhyMLClasses.IntegerInput>.get_singleton() },
            //{ typeof( CyPhyMLClasses.BooleanInput ).Name,              CreateMethodProxy<CyPhyMLClasses.BooleanInput>.get_singleton() },
            //{ typeof( CyPhyMLClasses.RealOutput ).Name,                CreateMethodProxy<CyPhyMLClasses.RealOutput>.get_singleton() },
            //{ typeof( CyPhyMLClasses.IntegerOutput ).Name,             CreateMethodProxy<CyPhyMLClasses.IntegerOutput>.get_singleton() },
            //{ typeof( CyPhyMLClasses.BooleanOutput ).Name,             CreateMethodProxy<CyPhyMLClasses.BooleanOutput>.get_singleton() },
            //{ typeof( CyPhyMLClasses.BusPort ).Name,                   CreateMethodProxy<CyPhyMLClasses.BusPort>.get_singleton() },
            //{ typeof( CyPhyMLClasses.ManufacturingModel ).Name,        CreateMethodProxy<CyPhyMLClasses.ManufacturingModel>.get_singleton() },
            //{ typeof(CyPhyMLClasses.ManufacturingModelParameter ).Name,CreateMethodProxy<CyPhyMLClasses.ManufacturingModelParameter>.get_singleton() }
        };

        protected Dictionary<string, CyPhyML.Port> _portNameTypeMap = new Dictionary<string, CyPhyML.Port>();

        protected Dictionary<avm.DataTypeEnum, CyPhyMLClasses.Property.AttributesClass.DataType_enum> _dataTypePropertyEnumMap = new Dictionary<DataTypeEnum, CyPhyMLClasses.Property.AttributesClass.DataType_enum>() {
            { avm.DataTypeEnum.Boolean, CyPhyMLClasses.Property.AttributesClass.DataType_enum.Boolean },
            { avm.DataTypeEnum.Integer, CyPhyMLClasses.Property.AttributesClass.DataType_enum.Integer },
            { avm.DataTypeEnum.Real,    CyPhyMLClasses.Property.AttributesClass.DataType_enum.Float },
            { avm.DataTypeEnum.String,  CyPhyMLClasses.Property.AttributesClass.DataType_enum.String }
        };

        protected Dictionary<avm.DataTypeEnum, CyPhyMLClasses.Parameter.AttributesClass.DataType_enum> _dataTypeParameterEnumMap = new Dictionary<DataTypeEnum, CyPhyMLClasses.Parameter.AttributesClass.DataType_enum>() {
            { avm.DataTypeEnum.Boolean, CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.Boolean },
            { avm.DataTypeEnum.Integer, CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.Integer },
            { avm.DataTypeEnum.Real,    CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.Float },
            { avm.DataTypeEnum.String,  CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.String }
        };

        private static Dictionary<avm.SimpleFormulaOperation, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum> sfOperatorMap
            = new Dictionary<avm.SimpleFormulaOperation, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum>()
            {
                {avm.SimpleFormulaOperation.Addition, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Addition},
                {avm.SimpleFormulaOperation.ArithmeticMean, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.ArithmeticMean},
                {avm.SimpleFormulaOperation.GeometricMean, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.GeometricMean},
                {avm.SimpleFormulaOperation.Maximum, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Maximum},
                {avm.SimpleFormulaOperation.Minimum, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Minimum},
                {avm.SimpleFormulaOperation.Multiplication, CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Multiplication}
            };

        public enum MessageType { OUT, ERROR, WARNING, INFO };

        public object messageConsole
        {
            get
            {
                return _messageConsole;
            }
            set
            {
                _messageConsole = value;
            }
        }

        public void writeMessage(string message, MessageType messageType = MessageType.OUT)
        {
            if (messageConsole == null)
            {
                return;
            }

            TextWriter textWriter = null;

            if (messageConsole is GMEConsole)
            {
                GMEConsole gmeConsole = messageConsole as GMEConsole;
                switch (messageType)
                {
                    case MessageType.INFO:
                        textWriter = gmeConsole.Info;
                        break;
                    case MessageType.WARNING:
                        textWriter = gmeConsole.Warning;
                        break;
                    case MessageType.ERROR:
                        textWriter = gmeConsole.Error;
                        break;
                    default:
                    case MessageType.OUT:
                        textWriter = gmeConsole.Out;
                        break;
                }
            }
            else if (messageConsole is Console)
            {
                switch (messageType)
                {
                    case MessageType.INFO:
                    case MessageType.WARNING:
                    case MessageType.ERROR:
                        textWriter = Console.Error;
                        break;
                    default:
                    case MessageType.OUT:
                        textWriter = Console.Out;
                        break;
                }
            }

            if (textWriter != null)
            {
                textWriter.WriteLine(message);
            }
        }

        public static bool SetLayoutData(object avmObj, IMgaObject cyphyObj)
        {
            if (cyphyObj is IMgaFCO)
            {
                var cyphyFCO = cyphyObj as IMgaFCO;
                if (cyphyFCO.ParentModel == null)
                {
                    return false;
                }
                var objWrapper = new LayoutDataWrapper(avmObj);

                if (objWrapper.hasLayoutData)
                {
                    // Look for all aspects, and set our layout data.
                    foreach (IMgaPart part in cyphyFCO.Parts)
                    {
                        part.SetGmeAttrs(null,
                            System.Convert.ToInt32(objWrapper.xpos),
                            System.Convert.ToInt32(objWrapper.ypos));
                    }
                    return true;
                }
            }
            return false;
        }

        protected void registerPort(avm.PortMapTarget avmPort)
        {
            _avmPortSet.Add(avmPort);
            if (avmPort.ID != null)
            {
                try
                {
                    _avmPortIDMap.Add(avmPort.ID, avmPort);
                }
                catch (System.ArgumentException)
                {
                    throw new ApplicationException(String.Format("Error in component model: duplicate ID '{0}'", avmPort.ID));
                }
            }
        }

        protected void registerValueNode(avm.ValueNode avmValueNode, object owner)
        {
            var keyValuePair = new KeyValuePair<ValueNode, object>(avmValueNode, owner);
            if (false == _avmValueNodeSet.Contains(keyValuePair))
            {
                _avmValueNodeSet.Add(keyValuePair);
            }

            if (String.IsNullOrWhiteSpace(avmValueNode.ID) == false
                && false == _avmValueNodeIDMap.ContainsKey(avmValueNode.ID))
            {
                _avmValueNodeIDMap.Add(avmValueNode.ID, keyValuePair);
            }
        }

        private class CreateMethodProxyAux<Class> where Class : class
        {
            private MethodInfo _createMethodInfo;
            private Object _roleStrDefaultValue;

            public CreateMethodProxyAux(Type parentClass)
            {
                Type classType = typeof(Class);

                MethodInfo[] methodInfoArray = classType.GetMethods(BindingFlags.Static | BindingFlags.Public);

                bool termLoop = false;
                foreach (MethodInfo methodInfo in methodInfoArray)
                {
                    if (methodInfo.Name == "Create")
                    {
                        ParameterInfo[] parameterInfoArray = methodInfo.GetParameters();
                        foreach (ParameterInfo parameterInfo in parameterInfoArray)
                        {
                            if (parameterInfo.ParameterType.Equals(parentClass))
                            {
                                _createMethodInfo = methodInfo;
                                termLoop = true;
                                break;
                            }
                        }
                        if (termLoop)
                        {
                            break;
                        }
                    }
                }

                foreach (ParameterInfo parameterInfo in _createMethodInfo.GetParameters())
                {
                    if (parameterInfo.Name == "roleStr")
                    {
                        _roleStrDefaultValue = parameterInfo.DefaultValue;
                        break;
                    }
                }
            }

            public void call<BaseClass, ParentClass>(ParentClass parent, out BaseClass baseClassObject) where BaseClass : class
            {
                baseClassObject = _createMethodInfo.Invoke(null, new object[] { parent, _roleStrDefaultValue }) as BaseClass;
            }
        }

        protected interface CreateMethodProxyBase
        {
            void call<BaseClass, ParentClass>(ParentClass parent, out BaseClass baseClassObject) where BaseClass : class;
            Type getClass();
        }

        protected class CreateMethodProxy<Class> : CreateMethodProxyBase where Class : class
        {
            private Dictionary<string, CreateMethodProxyAux<Class>> _parentClassCMPADictionary = new Dictionary<string, CreateMethodProxyAux<Class>>();

            protected CreateMethodProxy() { }

            private static CreateMethodProxy<Class> _singleton = new CreateMethodProxy<Class>();

            public static CreateMethodProxy<Class> get_singleton()
            {
                return _singleton;
            }

            public Type getClass()
            {
                return typeof(Class);
            }

            private CreateMethodProxyAux<Class> getCreateMethodProxyAux(Type parentClassType)
            {
                string parentClassName = parentClassType.Name;
                if (_parentClassCMPADictionary.ContainsKey(parentClassName))
                {
                    return _parentClassCMPADictionary[parentClassName];
                }

                CreateMethodProxyAux<Class> createMethodProxyAux = new CreateMethodProxyAux<Class>(parentClassType);
                _parentClassCMPADictionary.Add(parentClassType.Name, createMethodProxyAux);
                return createMethodProxyAux;
            }

            public void call<BaseClass, ParentClass>(ParentClass parent, out BaseClass baseClassObject) where BaseClass : class
            {
                getCreateMethodProxyAux(typeof(ParentClass)).call(parent, out baseClassObject);
            }
        }

        public void setUnitMap(Dictionary<String, CyPhyML.unit> unitSymbolCyPhyMLUnitMap)
        {
            _unitSymbolCyPhyMLUnitMap = unitSymbolCyPhyMLUnitMap;
        }

        public Dictionary<String, CyPhyML.unit> getUnitMap()
        {
            return _unitSymbolCyPhyMLUnitMap;
        }

        private void getCyPhyMLUnits(CyPhyML.RootFolder rootFolder)
        {
            foreach (CyPhyML.TypeSpecifications typeSpecifications in rootFolder.Children.TypeSpecificationsCollection)
            {
                foreach (CyPhyML.Units units in typeSpecifications.Children.UnitsCollection)
                {
                    _cyPhyMLUnitsFolders.Add(units);
                }
            }
        }

        protected PropertyInfo getPropertyInfo(Type type, string propertyName)
        {
            return type.GetProperty(propertyName);
        }

        protected PropertyInfo getPropertyInfo(object object_var, string propertyName)
        {
            return getPropertyInfo(object_var.GetType(), propertyName);
        }

        protected Type getInterfaceType(Type type)
        {
            string typeName = type.UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
            return Type.GetType(typeName);
        }

        protected Type getInterfaceType(object object_var)
        {
            return getInterfaceType(object_var.GetType());
        }


        protected void getCyPhyMLUnits()
        {
            _cyPhyMLUnitsFolders = new List<CyPhyML.Units>();

            // Collect all of the Root Folders in the project.
            // They will be sorted, with the QUDT lib in front, followed by all other libs, then the user's Root Folder.
            var cyPhyMLRootFolderList = new List<CyPhyML.RootFolder>();
            cyPhyMLRootFolderList.AddRange(_cyPhyMLRootFolder.LibraryCollection
                                                             .OrderByDescending(lc => lc.Name.Equals("UnitLibrary QUDT")));
            cyPhyMLRootFolderList.Add(_cyPhyMLRootFolder);

            // Now, for each Root Folder that we gathered, go through and find all units, and add them to our master index.
            if (cyPhyMLRootFolderList.Count > 0)
            {
                cyPhyMLRootFolderList.ForEach(lrf => getCyPhyMLUnits(lrf));
            }
        }

        private static bool isUnitless(CyPhyML.unit cyPhyMLUnit)
        {
            if (cyPhyMLUnit is CyPhyML.si_unit)
            {
                CyPhyML.si_unit si_unit_var = cyPhyMLUnit as CyPhyML.si_unit;
                return
                    si_unit_var.Attributes.amount_of_substance_exponent == 0 &&
                    si_unit_var.Attributes.electric_current_exponent == 0 &&
                    si_unit_var.Attributes.length_exponent == 0 &&
                    si_unit_var.Attributes.luminous_intensity_exponent == 0 &&
                    si_unit_var.Attributes.mass_exponent == 0 &&
                    si_unit_var.Attributes.thermodynamic_temperature_exponent == 0 &&
                    si_unit_var.Attributes.time_exponent == 0;
            }

            if (cyPhyMLUnit is CyPhyML.derived_unit)
            {
                CyPhyML.derived_unit derived_unit_var = cyPhyMLUnit as CyPhyML.derived_unit;
                List<CyPhyML.derived_unit_element> derived_unit_elementList = derived_unit_var.Children.derived_unit_elementCollection.ToList<CyPhyML.derived_unit_element>();
                return derived_unit_elementList.Count == 1 ? isUnitless(derived_unit_elementList.ElementAt(0).Referred.named_unit) : false;
            }

            CyPhyML.conversion_based_unit cyPhyMLconversion_based_unit = cyPhyMLUnit as CyPhyML.conversion_based_unit;
            List<CyPhyML.reference_unit> reference_unitList = cyPhyMLconversion_based_unit.Children.reference_unitCollection.ToList<CyPhyML.reference_unit>();
            if (reference_unitList.Count() == 0)
            {
                return false;
            }

            CyPhyML.reference_unit cyPhyMLreference_unit = reference_unitList.ElementAt(0);
            return reference_unitList.Count == 1 ? isUnitless(cyPhyMLreference_unit.Referred.unit) : false;
        }

        protected void getCyPhyMLNamedUnits(bool resetUnitLibrary = false)
        {
            if (false == _cyPhyMLUnitsFolders.Any())
            {
                return;
            }

            // If the caller has passed in this map already
            if (resetUnitLibrary)
            {
                _unitSymbolCyPhyMLUnitMap.Clear();
            }

            if (_unitSymbolCyPhyMLUnitMap.Count > 0)
            {
                return;
            }

            foreach (CyPhyML.unit cyPhyMLUnit in _cyPhyMLUnitsFolders.SelectMany(uf => uf.Children.unitCollection))
            {
                // Angle-type measures are an exception to this rule.
                /*
				if (cyPhyMLUnit.Attributes.Abbreviation != "rad" &&
					cyPhyMLUnit.Attributes.Abbreviation != "deg" &&
					isUnitless(cyPhyMLUnit))
				{
					continue;
				}*/

                if (!_unitSymbolCyPhyMLUnitMap.ContainsKey(cyPhyMLUnit.Attributes.Abbreviation))
                {
                    _unitSymbolCyPhyMLUnitMap.Add(cyPhyMLUnit.Attributes.Abbreviation, cyPhyMLUnit);
                }
                else if (cyPhyMLUnit is CyPhyML.conversion_based_unit) // Always prefer SI unit
                {
                    _unitSymbolCyPhyMLUnitMap[cyPhyMLUnit.Attributes.Abbreviation] = cyPhyMLUnit;
                }

                if (!_unitSymbolCyPhyMLUnitMap.ContainsKey(cyPhyMLUnit.Attributes.Symbol))
                {
                    _unitSymbolCyPhyMLUnitMap.Add(cyPhyMLUnit.Attributes.Symbol, cyPhyMLUnit);
                }
                else if (cyPhyMLUnit is CyPhyML.conversion_based_unit) // Always prefer SI unit
                {
                    _unitSymbolCyPhyMLUnitMap[cyPhyMLUnit.Attributes.Symbol] = cyPhyMLUnit;
                }

                if (!_unitSymbolCyPhyMLUnitMap.ContainsKey(cyPhyMLUnit.Attributes.FullName))
                {
                    _unitSymbolCyPhyMLUnitMap.Add(cyPhyMLUnit.Attributes.FullName, cyPhyMLUnit);
                }
                else if (cyPhyMLUnit is CyPhyML.conversion_based_unit) // Always prefer SI unit
                {
                    _unitSymbolCyPhyMLUnitMap[cyPhyMLUnit.Attributes.FullName] = cyPhyMLUnit;
                }
            }
        }

        private void getCyPhyMLPorts(CyPhyML.RootFolder rootFolder)
        {
            foreach (CyPhyML.Connectors cyPhyMLConnectors in rootFolder.Children.ConnectorsCollection)
            {
                foreach (CyPhyML.Ports ports in cyPhyMLConnectors.Children.PortsCollection)
                {
                    _cyPhyMLPorts = ports;
                    break;
                }
                if (_cyPhyMLPorts != null)
                {
                    break;
                }
            }
        }

        protected void getCyPhyMLPorts()
        {
            _cyPhyMLPorts = null;

            List<CyPhyML.RootFolder> cyPhyMLRootFolderList = _cyPhyMLRootFolder.LibraryCollection.ToList<CyPhyML.RootFolder>();
            if (cyPhyMLRootFolderList.Count > 0)
            {
                foreach (CyPhyML.RootFolder libraryRootFolder in _cyPhyMLRootFolder.LibraryCollection)
                {
                    getCyPhyMLPorts(libraryRootFolder);
                    if (_cyPhyMLPorts != null)
                    {
                        break;
                    }
                }
            }

            if (_cyPhyMLPorts == null)
            {
                getCyPhyMLPorts(_cyPhyMLRootFolder);
            }
        }

        private void getCyPhyMLModelPorts(CyPhyML.Ports cyPhyMLPorts)
        {
            if (cyPhyMLPorts.Children.PortCollection.Count() != 0)
            {
                foreach (CyPhyML.Port cyPhyMLPort in cyPhyMLPorts.Children.PortCollection)
                {
                    PropertyInfo attributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLPort), "Attributes");
                    PropertyInfo classPropertyInfo = attributesPropertyInfo != null ? attributesPropertyInfo.PropertyType.GetProperty("Class") : null;

                    string key = cyPhyMLPort.Name;

                    if (classPropertyInfo != null)
                    {
                        object bar = classPropertyInfo.GetValue(attributesPropertyInfo.GetValue(cyPhyMLPort, null), null);
                        key = classPropertyInfo.GetValue(attributesPropertyInfo.GetValue(cyPhyMLPort, null), null) as string;
                    }
                    _portNameTypeMap.Add(key, cyPhyMLPort);
                }
            }

            foreach (CyPhyML.Ports cyPhyMLChildPorts in cyPhyMLPorts.Children.PortsCollection)
            {
                getCyPhyMLModelPorts(cyPhyMLChildPorts);
            }
        }

        protected void getCyPhyMLModelPorts(bool resetPorts = false)
        {
            if (_cyPhyMLPorts == null)
            {
                return;
            }

            // If the caller has passed in this map already
            if (resetPorts)
            {
                _portNameTypeMap.Clear();
            }

            if (_portNameTypeMap.Count > 0)
            {
                return;
            }

            getCyPhyMLModelPorts(_cyPhyMLPorts);
        }

        protected void process<T>(T parent, avm.Property avmProperty, string compoundPropertyPath = "")
        {
            if (avmProperty is avm.CompoundProperty)
            {
                avm.CompoundProperty avmCompoundProperty = avmProperty as avm.CompoundProperty;

                if (string.IsNullOrEmpty(compoundPropertyPath))
                {
                    compoundPropertyPath = avmProperty.Name;
                }
                else
                {
                    compoundPropertyPath = String.Format("{0}__{1}", compoundPropertyPath, avmProperty.Name);
                }

                foreach (CompoundProperty childAVMProperty in avmCompoundProperty.CompoundProperty1)
                {
                    process(parent, childAVMProperty, compoundPropertyPath);
                }
                foreach (PrimitiveProperty childAVMProperty in avmCompoundProperty.PrimitiveProperty)
                {
                    process(parent, childAVMProperty, compoundPropertyPath);
                }
                return;
            }


            avm.PrimitiveProperty avmPrimitiveProperty = avmProperty as avm.PrimitiveProperty;
            avm.Value avmValue = avmPrimitiveProperty.Value;

            registerValueNode(avmValue, avmPrimitiveProperty);


            CyPhyML.unit cyPhyMLUnit = null;
            if (!String.IsNullOrWhiteSpace(avmValue.Unit))
            {
                if (_unitSymbolCyPhyMLUnitMap.ContainsKey(avmValue.Unit))
                {
                    cyPhyMLUnit = _unitSymbolCyPhyMLUnitMap[avmValue.Unit];
                }
                else
                {
                    writeMessage(String.Format("WARNING: No unit lib match found for: {0}", avmValue.Unit), MessageType.WARNING);
                }
            }

            avm.ValueExpressionType avmValueExpressionType = avmValue.ValueExpression;
            if (avmValueExpressionType is avm.ParametricValue)
            {
                var avmParametricValue = avmValueExpressionType as avm.ParametricValue;

                CyPhyML.Parameter cyPhyMLParameter = null;
                CreateMethodProxy<CyPhyMLClasses.Parameter>.get_singleton().call(parent, out cyPhyMLParameter);

                _avmCyPhyMLObjectMap.Add(avmProperty, cyPhyMLParameter);

                cyPhyMLParameter.Name = (string.IsNullOrEmpty(compoundPropertyPath)) ? avmPrimitiveProperty.Name : String.Format("{0}__{1}", compoundPropertyPath, avmPrimitiveProperty.Name);

                cyPhyMLParameter.Attributes.ID = avmValue.ID;
                cyPhyMLParameter.Attributes.DataType = _dataTypeParameterEnumMap[avmValue.DataType];
                cyPhyMLParameter.Attributes.Dimension = avmValue.Dimensions;
                cyPhyMLParameter.Referred.unit = cyPhyMLUnit;


                // Set Default value (required)
                var def = avmParametricValue.Default;
                if (def is avm.FixedValue)
                {
                    var def_FV = def as avm.FixedValue;
                    cyPhyMLParameter.Attributes.DefaultValue = def_FV.Value;
                }


                // Set min and max, and default either end to inf if not provided
                var min = avmParametricValue.Minimum;
                var max = avmParametricValue.Maximum;
                bool rangeSet = false;
                string sMin = "-inf";
                string sMax = "inf";
                if (min is avm.FixedValue)
                {
                    var min_FV = min as avm.FixedValue;
                    if (!String.IsNullOrWhiteSpace(min_FV.Value))
                    {
                        sMin = min_FV.Value;
                        rangeSet = true;
                    }
                }
                if (max is avm.FixedValue)
                {
                    var max_FV = max as avm.FixedValue;
                    if (!String.IsNullOrWhiteSpace(max_FV.Value))
                    {
                        sMax = max_FV.Value;
                        rangeSet = true;
                    }
                }
                //-inf..inf
                if (rangeSet)
                {
                    cyPhyMLParameter.Attributes.Range = String.Format("{0}..{1}", sMin, sMax);
                }

                // If no AssignedValue is provided, use the DefaultValue
                if (avmParametricValue.AssignedValue is avm.FixedValue)
                {
                    var av_FV = avmParametricValue.AssignedValue as avm.FixedValue;
                    cyPhyMLParameter.Attributes.Value = av_FV.Value;
                }
                else
                {
                    cyPhyMLParameter.Attributes.Value = cyPhyMLParameter.Attributes.DefaultValue;
                }
                //registerValueNode(avmParametricValue
            }
            else
            {
                CyPhyML.Property cyPhyMLProperty = null;
                CreateMethodProxy<CyPhyMLClasses.Property>.get_singleton().call(parent, out cyPhyMLProperty);

                _avmCyPhyMLObjectMap.Add(avmProperty, cyPhyMLProperty);

                cyPhyMLProperty.Name = (string.IsNullOrEmpty(compoundPropertyPath)) ? avmPrimitiveProperty.Name : String.Format("{0}__{1}", compoundPropertyPath, avmPrimitiveProperty.Name);

                cyPhyMLProperty.Attributes.ID = avmValue.ID;
                cyPhyMLProperty.Attributes.DataType = _dataTypePropertyEnumMap[avmValue.DataType];
                //            cyPhyMLProperty.Attributes.Dimension = avmValue.DimensionType.ToString();
                cyPhyMLProperty.Attributes.Dimension = avmValue.Dimensions;
                if (avmProperty.OnDataSheetSpecified)
                {
                    cyPhyMLProperty.Attributes.IsProminent = avmProperty.OnDataSheet;
                }

                cyPhyMLProperty.Referred.unit = cyPhyMLUnit;
            }
        }

        protected void process(SimpleFormula avmSimpleFormula, CyPhyML.SimpleFormula cyphySimpleFormula)
        {
            cyphySimpleFormula.Attributes.Method = sfOperatorMap[avmSimpleFormula.Operation];
            cyphySimpleFormula.Attributes.ID = avmSimpleFormula.ID;
            cyphySimpleFormula.Name = avmSimpleFormula.Name;
            registerValueNode(avmSimpleFormula, avmSimpleFormula);

            _avmCyPhyMLObjectMap.Add(avmSimpleFormula, cyphySimpleFormula);
        }

        private static Dictionary<string, Func<IMgaObject, ISIS.GME.Common.Interfaces.Base>> dsmlCasts = new Dictionary<string, Func<IMgaObject, ISIS.GME.Common.Interfaces.Base>>()
        {
            {typeof(CyPhyMLClasses.ValueFlow).Name, CyPhyMLClasses.ValueFlow.Cast},
            {typeof(CyPhyMLClasses.ModelicaParameterPortMap).Name, CyPhyMLClasses.ModelicaParameterPortMap.Cast},
            {typeof(CyPhyMLClasses.ConnectorComposition).Name, CyPhyMLClasses.ConnectorComposition.Cast},
            {typeof(CyPhyMLClasses.CADMetricPortMap).Name, CyPhyMLClasses.CADMetricPortMap.Cast},
            {typeof(CyPhyMLClasses.CADParameterPortMap).Name, CyPhyMLClasses.CADParameterPortMap.Cast},
            {typeof(CyPhyMLClasses.ManufacturingParameterPortMap).Name, CyPhyMLClasses.ManufacturingParameterPortMap.Cast},
            {typeof(CyPhyMLClasses.PortComposition).Name, CyPhyMLClasses.PortComposition.Cast},
            {typeof(CyPhyMLClasses.CarParameterPortMap).Name, CyPhyMLClasses.CarParameterPortMap.Cast},
            // TODO more kinds
        };

        private Dictionary<avm.modelica.RedeclareTypeEnum, CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum> _modelicaRedeclareTypeEnumMap =
            new Dictionary<avm.modelica.RedeclareTypeEnum, CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum>() {
                { avm.modelica.RedeclareTypeEnum.Block,     CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Block },
                { avm.modelica.RedeclareTypeEnum.Class,     CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Class },
                { avm.modelica.RedeclareTypeEnum.Connector, CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Connector },
                { avm.modelica.RedeclareTypeEnum.Function,  CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Function },
                { avm.modelica.RedeclareTypeEnum.Model,     CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Model },
                { avm.modelica.RedeclareTypeEnum.Package,   CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Package },
                { avm.modelica.RedeclareTypeEnum.Record,    CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Record }
            };

        protected object makeConnection(object cyPhyMLObjectSrc, object cyPhyMLObjectDst, string kind, bool createAsPortConnection = false)
        {
            IMgaFCO src = GetFCOObject(cyPhyMLObjectSrc);
            IMgaReference srcReference = GetFCOObjectReference(cyPhyMLObjectSrc);
            IMgaFCO dst = GetFCOObject(cyPhyMLObjectDst);
            IMgaReference dstReference = GetFCOObjectReference(cyPhyMLObjectDst);

            IMgaModel srcparent = (srcReference != null ? srcReference : src).ParentModel;
            IMgaModel dstparent = (dstReference != null ? dstReference : dst).ParentModel;
            IMgaModel parent = srcparent;
            if (srcparent != dstparent) // src or dst is a port
            {
                if (srcparent == dstparent.ParentModel)
                {
                    parent = srcparent;
                }
                else if (srcparent.ParentModel == dstparent.ParentModel)
                {
                    parent = dstparent.ParentModel;
                }
                else if (dstparent == srcparent.ParentModel)
                {
                    parent = dstparent;
                }
            }
            else if (createAsPortConnection)
            {
                parent = srcparent.ParentModel;
            }

            var role = (MgaMetaRole)((MgaMetaModel)parent.Meta).RoleByName[kind];
            var conn = parent.CreateSimpleConnDisp(role, (MgaFCO)src, (MgaFCO)dst, (MgaFCO)srcReference, (MgaFCO)dstReference);

            return dsmlCasts[kind](conn);
        }

        public static IMgaFCO GetFCOObject(object cyPhyMLObjectSrc)
        {
            IMgaFCO source;
            if (cyPhyMLObjectSrc is ISIS.GME.Common.Interfaces.FCO)
            {
                source = (IMgaFCO)((ISIS.GME.Common.Interfaces.FCO)cyPhyMLObjectSrc).Impl;
            }
            else
            {
                source = (IMgaFCO)((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyPhyMLObjectSrc).Value.Impl;
            }

            return source;
        }
        public static IMgaReference GetFCOObjectReference(object cyPhyMLObjectSrc)
        {
            IMgaReference source_rp;
            if (cyPhyMLObjectSrc is ISIS.GME.Common.Interfaces.FCO)
            {
                source_rp = null;
            }
            else
            {
                source_rp = (IMgaReference)((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyPhyMLObjectSrc).Key.Impl;
            }

            return source_rp;
        }

        protected void processValues()
        {
            foreach (var avmSimpleFormulaWithOwner in _avmValueNodeSet.Where(t => t.Key is avm.SimpleFormula))
            {
                var avmSimpleFormula = avmSimpleFormulaWithOwner.Key as avm.SimpleFormula;
                var cyPhyMLSimpleFormula = _avmCyPhyMLObjectMap[avmSimpleFormula];

                foreach (var operandID in avmSimpleFormula.Operand)
                {
                    // Look up other AVM Value & its AVM Owner by ID.
                    KeyValuePair<avm.ValueNode, object> avmSourceValueNodeWithOwner;
                    if (_avmValueNodeIDMap.TryGetValue(operandID, out avmSourceValueNodeWithOwner))
                    {
                        var avmSourceValueNodeOwner = avmSourceValueNodeWithOwner.Value;

                        // Find the CyPhy object corresponding with that AVM Value's Owner
                        object cyPhyMLSourceObject;
                        if (_avmCyPhyMLObjectMap.TryGetValue(avmSourceValueNodeOwner, out cyPhyMLSourceObject))
                        {
                            makeConnection(cyPhyMLSourceObject, cyPhyMLSimpleFormula, typeof(CyPhyML.ValueFlow).Name);
                        }
                    }
                    else
                    {
                        if (Debugger.IsAttached)
                        {
                            Debugger.Break();
                        }
                    }
                }
            }

            foreach (var avmComplexFormulaWithOwner in _avmValueNodeSet.Where(t => t.Key is avm.ComplexFormula))
            {
                var avmComplexFormula = avmComplexFormulaWithOwner.Key as avm.ComplexFormula;
                var cyPhyMLCustomFormula = _avmCyPhyMLObjectMap[avmComplexFormula];

                foreach (var operand in avmComplexFormula.Operand)
                {
                    // Get the source operand & its owner by ID
                    KeyValuePair<avm.ValueNode, object> avmSourceValueNodeWithOwner;
                    if (_avmValueNodeIDMap.TryGetValue(operand.ValueSource, out avmSourceValueNodeWithOwner))
                    {
                        var avmSourceValueNodeOwner = avmSourceValueNodeWithOwner.Value;

                        // Find the CyPhy object corresponding with that AVM Value's Ownder
                        object cyPhyMLSourceObject;
                        if (_avmCyPhyMLObjectMap.TryGetValue(avmSourceValueNodeOwner, out cyPhyMLSourceObject))
                        {
                            CyPhyML.ValueFlow valueFlow = makeConnection(cyPhyMLSourceObject, cyPhyMLCustomFormula, typeof(CyPhyML.ValueFlow).Name) as CyPhyML.ValueFlow;

                            var cyPhyMLSourceObject_ValueFlowTarget = cyPhyMLSourceObject as CyPhyML.ValueFlowTarget;
                            if (cyPhyMLSourceObject_ValueFlowTarget == null)
                            {
                                cyPhyMLSourceObject_ValueFlowTarget = ((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyPhyMLSourceObject).Value as CyPhyML.ValueFlowTarget;
                            }
                            if (cyPhyMLSourceObject is CyPhyML.ValueFormula)
                            {
                                // Set symbol
                                valueFlow.Attributes.FormulaVariableName = operand.Symbol;
                            }
                            else if (operand.Symbol == cyPhyMLSourceObject_ValueFlowTarget.Name)
                            {
                                // The source object has the same name as the symbol.
                                // No need to set an override here.
                            }
                            else
                            {
                                // Override symbol
                                valueFlow.Attributes.FormulaVariableName = operand.Symbol;
                            }
                        }
                    }
                    else if (Debugger.IsAttached)
                    {
                        Debugger.Break();
                    }
                }
            }

            foreach (var avmValueNodeWithOwner in _avmValueNodeSet.Where(t => t.Key is avm.Value))
            {
                var avmValue = avmValueNodeWithOwner.Key as avm.Value;
                object avmValueOwner = avmValueNodeWithOwner.Value;
                avm.ValueExpressionType avmValueExpression = avmValue.ValueExpression;

                if (avmValueExpression is avm.FixedValue)
                {
                    object cyPhyMLParentObject = null;
                    if (!_avmCyPhyMLObjectMap.TryGetValue(avmValueOwner, out cyPhyMLParentObject))
                    {
                        continue;
                    }

                    string objectTypeName = cyPhyMLParentObject.GetType().UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
                    Type objectType = Type.GetType(objectTypeName);

                    PropertyInfo attributesPropertyInfo = getPropertyInfo(objectType, "Attributes");
                    if (attributesPropertyInfo == null)
                    {
                        String sOutput = String.Format("WARNING: could not assign avm.FixedValue to cyPhyMLObject of type {0}", objectType.FullName);
                        Console.Out.WriteLine(sOutput);
                        continue;
                    }

                    PropertyInfo assignToProperty = getPropertyInfo(attributesPropertyInfo.PropertyType, "Value");
                    if (assignToProperty == null)
                    {
                        String sOutput = String.Format("WARNING: could not assign avm.FixedValue to cyPhyMLObject of type {0}", objectType.FullName);
                        Console.Out.WriteLine(sOutput);
                        continue;
                    }

                    assignToProperty.SetValue(attributesPropertyInfo.GetValue(cyPhyMLParentObject, null), (avmValueExpression as avm.FixedValue).Value, null);
                    continue;
                }
                else if (avmValueExpression is avm.ParametricValue)
                {
                    var avmPV = avmValueExpression as avm.ParametricValue;

                    object cyPhyMLParentObject = null;
                    if (!_avmCyPhyMLObjectMap.TryGetValue(avmValueOwner, out cyPhyMLParentObject))
                    {
                        continue;
                    }

                    // Get underlying CyPhy Interface class for the object that was created in CyPhy
                    string objectTypeName = cyPhyMLParentObject.GetType().UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
                    Type objectType = Type.GetType(objectTypeName);

                    // Get this thing's attributes. If we can't, show an error and break.
                    PropertyInfo attributesPropertyInfo = getPropertyInfo(objectType, "Attributes");
                    if (attributesPropertyInfo == null)
                    {
                        String sOutput = String.Format("WARNING: could not assign avm.ParametricValue to cyPhyMLObject of type {0}", objectType.FullName);
                        Console.Out.WriteLine(sOutput);
                        continue;
                    }

                    /// First, get these two key properties. Then we'll figure out what to do based on what we get.
                    PropertyInfo cyPhyAssignedValueAttribute = getPropertyInfo(attributesPropertyInfo.PropertyType, "Value");
                    if (cyPhyAssignedValueAttribute == null)
                    {
                        String sOutput = String.Format("WARNING: could not assign AssignedValue of avm.ParametricValue to cyPhyMLObject of type {0}", objectType.FullName);
                        Console.Out.WriteLine(sOutput);
                        continue;
                    }
                    PropertyInfo cyphyDefaultValueAttribute = getPropertyInfo(attributesPropertyInfo.PropertyType, "DefaultValue");
                    if (cyphyDefaultValueAttribute == null)
                    {
                        String sOutput = String.Format("WARNING: could not assign DefaultValue of avm.ParametricValue to cyPhyMLObject of type {0}", objectType.FullName);
                        Console.Out.WriteLine(sOutput);
                        continue;
                    }

                    ///// If incoming model has AssignedValue, use it. /////
                    if (avmPV.AssignedValue != null)
                    {
                        var av = avmPV.AssignedValue;
                        if (av is avm.FixedValue)
                        {
                            var avFV = av as avm.FixedValue;
                            cyPhyAssignedValueAttribute.SetValue(attributesPropertyInfo.GetValue(cyPhyMLParentObject, null), avFV.Value, null);
                        }
                        else if (av is avm.DerivedValue)
                        {
                            KeyValuePair<ValueNode, object> avmOtherValueWithOwner;
                            if (_avmValueNodeIDMap.TryGetValue(((avm.DerivedValue)av).ValueSource, out avmOtherValueWithOwner))
                            {
                                object avmOtherValueOwner = avmOtherValueWithOwner.Value;

                                if (avmOtherValueOwner is avm.ValueFlowMux)
                                {
                                    object cyPhyMLObjectDst = null;
                                    var hasValueParent = _avmCyPhyMLObjectMap.TryGetValue(avmValueOwner, out cyPhyMLObjectDst);
                                    ConnectMuxSources((avm.ValueFlowMux)avmOtherValueOwner, (CyPhyML.ValueFlowTarget)cyPhyMLObjectDst);
                                }
                                else
                                {
                                    object cyPhyMLObjectSrc = null;
                                    if (!_avmCyPhyMLObjectMap.TryGetValue(avmOtherValueOwner, out cyPhyMLObjectSrc))
                                    {
                                        continue;
                                    }

                                    object cyPhyMLObjectDst = null;
                                    var hasValueParent = _avmCyPhyMLObjectMap.TryGetValue(avmValueOwner, out cyPhyMLObjectDst);

                                    makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.ValueFlow).Name);
                                }
                            }
                            else if (Debugger.IsAttached)
                            {
                                Debugger.Break();
                            }
                        }
                    }

                    ///// If incoming model has DefaultValue, use it. /////
                    if (avmPV.Default != null)
                    {
                        if (avmPV.Default is avm.FixedValue)
                        {
                            var avmDefaultVal = (avmPV.Default as avm.FixedValue).Value;
                            cyphyDefaultValueAttribute.SetValue(attributesPropertyInfo.GetValue(cyPhyMLParentObject, null), avmDefaultVal, null);

                            // If AssignedValue was null, let's use the default here
                            if (avmPV.AssignedValue == null)
                            {
                                cyPhyAssignedValueAttribute.SetValue(attributesPropertyInfo.GetValue(cyPhyMLParentObject, null), avmDefaultVal, null);
                            }
                        }
                        else
                        {
                            String sOutput = String.Format("WARNING: could not assign DefaultValue (using non-FixedValue) of avm.ParametricValue to cyPhyMLObject of type {0}", objectType.FullName);
                            Console.Out.WriteLine(sOutput);
                            continue;
                        }
                    }
                }
                else if (avmValueExpression is avm.DerivedValue)
                {
                    string otherValueID = (avmValueExpression as avm.DerivedValue).ValueSource;

                    object cyPhyMLObjectDst = null;
                    var hasValueParent = _avmCyPhyMLObjectMap.TryGetValue(avmValueOwner, out cyPhyMLObjectDst);

                    if (hasValueParent && !(cyPhyMLObjectDst is KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>))
                    {
                        var attributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObjectDst), "Attributes");
                        var idProperty = getPropertyInfo(attributesPropertyInfo.PropertyType, "ID");
                        var unitsProperty = getPropertyInfo(attributesPropertyInfo.PropertyType, "Unit");
                        if (idProperty != null)
                        {
                            var attr = attributesPropertyInfo.GetValue(cyPhyMLObjectDst, null);
                            idProperty.SetValue(attr, avmValue.ID, null);
                        }
                        if (unitsProperty != null)
                        {
                            var attr = attributesPropertyInfo.GetValue(cyPhyMLObjectDst, null);
                            unitsProperty.SetValue(attr, avmValue.Unit, null);
                        }
                    }

                    if (_avmValueNodeIDMap.ContainsKey(otherValueID) && hasValueParent)
                    {
                        var avmOtherValueWithOwner = _avmValueNodeIDMap[otherValueID];
                        object avmOtherValueOwner = avmOtherValueWithOwner.Value;


                        if (avmOtherValueOwner is avm.ValueFlowMux)
                        {
                            ConnectMuxSources((avm.ValueFlowMux)avmOtherValueOwner, (CyPhyML.ValueFlowTarget)cyPhyMLObjectDst);
                        }
                        else
                        {
                            object cyPhyMLObjectSrc = null;
                            if (!_avmCyPhyMLObjectMap.TryGetValue(avmOtherValueOwner, out cyPhyMLObjectSrc))
                            {
                                continue;
                            }

                            if (cyPhyMLObjectDst is CyPhyML.CarParameter)
                            {
                                // CyPhyML.ModelicaParameter.SrcConnections.ModelicaParameterPortMapCollection
                                makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.CarParameterPortMap).Name);
                            }
                            else if (cyPhyMLObjectDst is CyPhyML.ModelicaParameter)
                            {
                                // CyPhyML.ModelicaParameter.SrcConnections.ModelicaParameterPortMapCollection
                                makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.ModelicaParameterPortMap).Name);
                            }
                            else if (cyPhyMLObjectSrc is CyPhyML.CADMetric)
                            {
                                // CyPhyML.CADMetric.DstConnections.CADMetricPortMapCollection
                                makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.CADMetricPortMap).Name);
                            }
                            else if (cyPhyMLObjectDst is CyPhyML.CADParameter)
                            {
                                // CyPhyML.CADParameter.SrcConnections.CADParameterPortMapCollection
                                makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.CADParameterPortMap).Name);
                            }
                            // else if (cyPhyMLObjectSrc is CyPhyML.ManufacturingModelMetric) TODO???
                            //{
                            //    CyPhyML.ManufacturingModelMetric x; x.DstConnections.
                            //}
                            else if (cyPhyMLObjectDst is CyPhyML.ManufacturingModelParameter)
                            {
                                // CyPhyML.ManufacturingModelParameter x; x.SrcConnections.ManufacturingParameterPortMapCollection
                                makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.ManufacturingParameterPortMap).Name);
                            }
                            else
                            {
                                makeConnection(cyPhyMLObjectSrc, cyPhyMLObjectDst, typeof(CyPhyML.ValueFlow).Name);
                            }

                            bool haveValue = false;
                            object value = null;
                            PropertyInfo cyPhyMLObjectSrcAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObjectSrc), "Attributes");
                            if (cyPhyMLObjectSrcAttributesPropertyInfo != null)
                            {
                                PropertyInfo cyPhyMLObjectSrcValuePropertyInfo = getPropertyInfo(cyPhyMLObjectSrcAttributesPropertyInfo.PropertyType, "Value");
                                if (cyPhyMLObjectSrcValuePropertyInfo != null)
                                {
                                    haveValue = true;
                                    value = (string)cyPhyMLObjectSrcValuePropertyInfo.GetValue(cyPhyMLObjectSrcAttributesPropertyInfo.GetValue(cyPhyMLObjectSrc, null), null);
                                }
                            }

                            if (haveValue)
                            {
                                PropertyInfo cyPhyMLObjectDstAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObjectDst), "Attributes");
                                if (cyPhyMLObjectDstAttributesPropertyInfo != null)
                                {
                                    PropertyInfo cyPhyMLObjectDstValuePropertyInfo = getPropertyInfo(cyPhyMLObjectDstAttributesPropertyInfo.PropertyType, "Value");
                                    if (cyPhyMLObjectDstValuePropertyInfo != null)
                                    {
                                        cyPhyMLObjectDstValuePropertyInfo.SetValue(cyPhyMLObjectDstAttributesPropertyInfo.GetValue(cyPhyMLObjectDst, null), value, null);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        private void ConnectMuxSources(avm.ValueFlowMux ad_mux, CyPhyML.ValueFlowTarget target)
        {
            var incomingCyPhys = ad_mux.Source.Select(x =>
            {
                var avmSourceValueNodeWithOwner = _avmValueNodeIDMap[x];
                var avmValue = avmSourceValueNodeWithOwner.Key;
                var avmSourceValueNodeOwner = avmSourceValueNodeWithOwner.Value;

                // Find the CyPhy object corresponding with that AVM Value's Owner
                object cyPhyMLSourceObject;
                if (_avmCyPhyMLObjectMap.TryGetValue(avmSourceValueNodeOwner, out cyPhyMLSourceObject))
                {
                    return cyPhyMLSourceObject;
                }
                return null;
            });

            foreach (var source in incomingCyPhys)
            {
                makeConnection(source, target, typeof(CyPhyML.ValueFlow).Name);
            }
        }


        protected void processComplexFormula(ComplexFormula avmComplexFormula, CyPhyML.CustomFormula cyphyCustomFormula)
        {
            cyphyCustomFormula.Name = avmComplexFormula.Name;
            cyphyCustomFormula.Attributes.ID = avmComplexFormula.ID;
            cyphyCustomFormula.Attributes.Expression = avmComplexFormula.Expression;
            registerValueNode(avmComplexFormula, avmComplexFormula);

            _avmCyPhyMLObjectMap.Add(avmComplexFormula, cyphyCustomFormula);
        }

        protected void init(bool resetUnitLibrary = false)
        {
            getCyPhyMLUnits();
            getCyPhyMLNamedUnits(resetUnitLibrary);

            getCyPhyMLPorts();
            getCyPhyMLModelPorts();
        }

        protected void DoLayout()
        {
            // If a member had layout information, lay everything out.
            // If not, fall back and use an auto-layout method.
            foreach (var kvp in _avmCyPhyMLObjectMap)
            {
                var avmObj = kvp.Key;
                var cyphyObj = kvp.Value;

                //var propInfo = getPropertyInfo(cyphyObj, "Impl");
                if (cyphyObj is ISIS.GME.Common.Interfaces.Base)
                {
                    var cyphyImpl = ((ISIS.GME.Common.Interfaces.Base)cyphyObj).Impl;

                    this.aMemberHasLayoutData = SetLayoutData(avmObj, cyphyImpl) || aMemberHasLayoutData;
                }
            }
        }

        protected void process<T>(T parent, avm.Port avmPort, List<avm.ConnectorFeature> connectorFeatures = null)
        {
            string avmPortTypeName = avmPort.GetType().ToString();
            if (!_cyPhyMLNameCreateMethodMap.ContainsKey(avmPortTypeName))
            {
                writeMessage("WARNING:  No way to create CyPhyML object from \"" + avmPortTypeName + "\" avm port.", MessageType.WARNING);
                return;
            }

            CyPhyML.DomainModelPort cyPhyMLDomainModelPort = null;

            PropertyInfo avmClassPropertyInfo = getPropertyInfo(avmPort, "Class");
            string avmClass = "";

            if (avmClassPropertyInfo != null)
            {
                PropertyInfo implPropertyInfo = getPropertyInfo(parent, "Impl");
                object parentImpl = implPropertyInfo.GetValue(parent, null);

                avmClass = avmClassPropertyInfo.GetValue(avmPort, null) as string;
                // META-1984 don't create instances, just create a copy
                MgaFCO portInstance = getPortCopyFromLibrary(parentImpl as IMgaModel, avmClass);

                if (portInstance != null)
                {
                    cyPhyMLDomainModelPort = CyPhyMLClasses.DomainModelPort.Cast(portInstance);
                }
            }

            if (cyPhyMLDomainModelPort == null)
            {
                _cyPhyMLNameCreateMethodMap[avmPortTypeName].call(parent, out cyPhyMLDomainModelPort);
            }

            if (avmClassPropertyInfo != null)
            {
                PropertyInfo cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLDomainModelPort), "Attributes");
                if (cyPhyMLAttributesPropertyInfo != null)
                {
                    PropertyInfo cyPhyMLClassPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("Class");
                    if (cyPhyMLClassPropertyInfo != null)
                    {
                        cyPhyMLClassPropertyInfo.SetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLDomainModelPort, null), avmClass, null);
                    }
                }
            }

            if (connectorFeatures != null)
            {
                foreach (var connectorFeature in connectorFeatures.OfType<avm.cad.GuideDatum>())
                {
                    if (avmPort.ID == connectorFeature.Datum)
                    {
                        ((MgaFCO)cyPhyMLDomainModelPort.Impl).set_BoolAttrByName("IsGuide", true);
                        break;
                    }
                }
            }

            if (avmPort is avm.modelica.Connector)
            {
                var avmModelicaConnector = avmPort as avm.modelica.Connector;
                var cyphyModelicaConnector = CyPhyMLClasses.ModelicaConnector.Cast(cyPhyMLDomainModelPort.Impl);
                foreach (var avmModelicaRedeclare in avmModelicaConnector.Redeclare)
                {
                    // See if this Redeclare already exists. If not, create one.
                    CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare = cyphyModelicaConnector.Children
                        .ModelicaRedeclareCollection
                        .FirstOrDefault(n => n.Name == avmModelicaRedeclare.Locator);
                    if (cyPhyMLModelicaRedeclare == null)
                    {
                        cyPhyMLModelicaRedeclare = CyPhyMLClasses.ModelicaRedeclare.Create(cyphyModelicaConnector);
                    }

                    populateModelicaRedeclare(avmModelicaRedeclare, cyPhyMLModelicaRedeclare);
                }
                foreach (var avmModelicaParameter in avmModelicaConnector.Parameter)
                {
                    // See if this Parameter already exists. If not, create one.
                    CyPhyML.ModelicaParameter cyPhyMLModelicaParameter = cyphyModelicaConnector.Children
                        .ModelicaParameterCollection
                        .FirstOrDefault(n => n.Name == avmModelicaParameter.Locator);
                    if (cyPhyMLModelicaParameter == null)
                    {
                        cyPhyMLModelicaParameter = CyPhyMLClasses.ModelicaParameter.Create(cyphyModelicaConnector);
                    }

                    _avmCyPhyMLObjectMap.Add(avmModelicaParameter, cyPhyMLModelicaParameter);
                    registerValueNode(avmModelicaParameter.Value, avmModelicaParameter);
                    cyPhyMLModelicaParameter.Name = avmModelicaParameter.Locator;
                }
            }

            _avmCyPhyMLObjectMap.Add(avmPort, cyPhyMLDomainModelPort);
            registerPort(avmPort);

            cyPhyMLDomainModelPort.Name = avmPort.Name;
            cyPhyMLDomainModelPort.Attributes.ID = avmPort.ID;
            cyPhyMLDomainModelPort.Attributes.Definition = avmPort.Definition;
            cyPhyMLDomainModelPort.Attributes.InstanceNotes = avmPort.Notes;

            // [ZL 2014-06-19] META-3356 use ALIGN as default
            if (cyPhyMLDomainModelPort is CyPhyML.Surface)
            {
                (cyPhyMLDomainModelPort as CyPhyML.Surface).Attributes.Alignment = CyPhyMLClasses.Surface.AttributesClass.Alignment_enum.ALIGN;
            }
        }

        protected MgaFCO getPortCopyFromLibrary(IMgaModel parent, string className)
        {
            if (!_portNameTypeMap.ContainsKey(className))
            {
                return null;
            }

            MgaFCO archetype = _portNameTypeMap[className].Impl as MgaFCO;
            MgaMetaRole role = (parent.Meta as MgaMetaModel).RoleByName[archetype.Meta.Name];
            if (role == null)
            {
                return null;
            }

            return parent.CopyFCODisp(archetype, role);
        }

        protected void populateModelicaRedeclare(avm.modelica.Redeclare avmModelicaRedeclare, CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare)
        {
            _avmCyPhyMLObjectMap.Add(avmModelicaRedeclare, cyPhyMLModelicaRedeclare);

            cyPhyMLModelicaRedeclare.Attributes.Notes = avmModelicaRedeclare.Notes;
            cyPhyMLModelicaRedeclare.Name = avmModelicaRedeclare.Locator;
            if (avmModelicaRedeclare.TypeSpecified)
            {
                cyPhyMLModelicaRedeclare.Attributes.ModelicaRedeclareType = _modelicaRedeclareTypeEnumMap[avmModelicaRedeclare.Type];
            }

            registerValueNode(avmModelicaRedeclare.Value, avmModelicaRedeclare);
        }

        protected Dictionary<string, avm.ConnectorCompositionTarget> _idConnectorMap = new Dictionary<string, ConnectorCompositionTarget>();

        protected void processConnector(Connector avmConnector, CyPhyML.Connector cyPhyMLConnector)
        {
            _avmCyPhyMLObjectMap.Add(avmConnector, cyPhyMLConnector);
            _idConnectorMap.Add(avmConnector.ID, avmConnector);

            cyPhyMLConnector.Name = avmConnector.Name;
            cyPhyMLConnector.Attributes.Definition = avmConnector.Definition;
            cyPhyMLConnector.Attributes.InstanceNotes = avmConnector.Notes;
            cyPhyMLConnector.Attributes.ID = avmConnector.ID;

            foreach (avm.Port avmPort in avmConnector.Role)
            {
                process(cyPhyMLConnector, avmPort, avmConnector.ConnectorFeature);
            }

            foreach (avm.Property avmProperty in avmConnector.Property)
            {
                process(cyPhyMLConnector, avmProperty);
            }

            foreach (var assemblyDetail in avmConnector.DefaultJoin)
            {
                JoinDataTransform.TransformJoinData(assemblyDetail, cyPhyMLConnector);
            }

            foreach (var kinematic in avmConnector.ConnectorFeature.OfType<avm.cad.RevoluteJointSpec>())
            {
                processRevoluteJoint(avmConnector, cyPhyMLConnector, kinematic);
            }
            foreach (var kinematic in avmConnector.ConnectorFeature.OfType<avm.cad.TranslationalJointSpec>())
            {
                processTranslationalJoint(avmConnector, cyPhyMLConnector, kinematic);
            }
        }

        private void processTranslationalJoint(Connector avmConnector, CyPhyML.Connector cyPhyMLConnector, avm.cad.TranslationalJointSpec kinematic)
        {
            CyPhyML.KinematicTranslationalJoint joint = CyPhyMLClasses.KinematicTranslationalJoint.Create(cyPhyMLConnector);
            joint.Name = "TranslationalJoint";
            PortMapTarget avmDatum;
            object cyphyDatum;
            if (_avmPortIDMap.TryGetValue(kinematic.AlignmentPlane, out avmDatum) && _avmCyPhyMLObjectMap.TryGetValue(avmDatum, out cyphyDatum))
            {
                CyPhyMLClasses.KinematicJointDefinition.Connect(joint, (CyPhyML.CADDatum)cyphyDatum);
            }
            if (_avmPortIDMap.TryGetValue(kinematic.AlignmentAxis, out avmDatum) && _avmCyPhyMLObjectMap.TryGetValue(avmDatum, out cyphyDatum))
            {
                CyPhyMLClasses.KinematicJointDefinition.Connect(joint, (CyPhyML.CADDatum)cyphyDatum);
            }
            if (kinematic.TranslationLimitReference != null && _avmPortIDMap.TryGetValue(kinematic.TranslationLimitReference, out avmDatum) && _avmCyPhyMLObjectMap.TryGetValue(avmDatum, out cyphyDatum))
            {
                CyPhyMLClasses.KinematicTranslationalLimitReference.Connect(joint, (CyPhyML.Surface)cyphyDatum);
            }
            CreateKinematicTranslationLimit(avmConnector, joint, CyPhyMLClasses.KinematicTranslationallLimit.AttributesClass.LimitType_enum.Default, kinematic.DefaultTranslation);
            CreateKinematicTranslationLimit(avmConnector, joint, CyPhyMLClasses.KinematicTranslationallLimit.AttributesClass.LimitType_enum.Max, kinematic.MaximumTranslation);
            CreateKinematicTranslationLimit(avmConnector, joint, CyPhyMLClasses.KinematicTranslationallLimit.AttributesClass.LimitType_enum.Min, kinematic.MinimumTranslation);
        }

        private void processRevoluteJoint(Connector avmConnector, CyPhyML.Connector cyPhyMLConnector, avm.cad.RevoluteJointSpec kinematic)
        {
            CyPhyML.KinematicRevoluteJoint joint = CyPhyMLClasses.KinematicRevoluteJoint.Create(cyPhyMLConnector);
            joint.Name = "RevoluteJoint";
            PortMapTarget avmDatum;
            object cyphyDatum;
            if (_avmPortIDMap.TryGetValue(kinematic.AlignmentPlane, out avmDatum) && _avmCyPhyMLObjectMap.TryGetValue(avmDatum, out cyphyDatum))
            {
                CyPhyMLClasses.KinematicJointDefinition.Connect(joint, (CyPhyML.CADDatum)cyphyDatum);
            }
            if (_avmPortIDMap.TryGetValue(kinematic.AlignmentAxis, out avmDatum) && _avmCyPhyMLObjectMap.TryGetValue(avmDatum, out cyphyDatum))
            {
                CyPhyMLClasses.KinematicJointDefinition.Connect(joint, (CyPhyML.CADDatum)cyphyDatum);
            }
            if (kinematic.RotationLimitReference != null && _avmPortIDMap.TryGetValue(kinematic.RotationLimitReference, out avmDatum) && _avmCyPhyMLObjectMap.TryGetValue(avmDatum, out cyphyDatum))
            {
                CyPhyMLClasses.KinematicRotationalLimitReference.Connect(joint, (CyPhyML.Surface)cyphyDatum);
            }
            CreateKinematicRotationalLimit(avmConnector, joint, CyPhyMLClasses.KinematicRotationalLimit.AttributesClass.LimitType_enum.Default, kinematic.DefaultRotation);
            CreateKinematicRotationalLimit(avmConnector, joint, CyPhyMLClasses.KinematicRotationalLimit.AttributesClass.LimitType_enum.Max, kinematic.MaximumRotation);
            CreateKinematicRotationalLimit(avmConnector, joint, CyPhyMLClasses.KinematicRotationalLimit.AttributesClass.LimitType_enum.Min, kinematic.MinimumRotation);
        }

        private void CreateKinematicRotationalLimit(Connector avmConnector, CyPhyML.KinematicRevoluteJoint joint, CyPhyMLClasses.KinematicRotationalLimit.AttributesClass.LimitType_enum limitType, Value value)
        {
            CreateKinematicLimit(avmConnector, joint, cyphyParam =>
            {
                var limit = CyPhyMLClasses.KinematicRotationalLimit.Connect((CyPhyML.Parameter)cyphyParam, joint);
                limit.Attributes.LimitType = limitType;
            }, value);
        }

        private void CreateKinematicTranslationLimit(Connector avmConnector, CyPhyML.KinematicTranslationalJoint joint, CyPhyMLClasses.KinematicTranslationallLimit.AttributesClass.LimitType_enum limitType, Value value)
        {
            CreateKinematicLimit(avmConnector, joint, cyphyParam =>
            {
                var limit = CyPhyMLClasses.KinematicTranslationallLimit.Connect(joint, (CyPhyML.Parameter)cyphyParam);
                limit.Attributes.LimitType = limitType;
            }, value);
        }

        private void CreateKinematicLimit(Connector avmConnector, CyPhyML.KinematicJoint joint, Action<CyPhyML.Parameter> act, Value value)
        {
            if (value != null && value.ValueExpression is avm.DerivedValue)
            {
                var valueSource = (value.ValueExpression as avm.DerivedValue).ValueSource;
                KeyValuePair<ValueNode, object> avmValueSource;
                if (!_avmValueNodeIDMap.TryGetValue(valueSource, out avmValueSource))
                {
                    throw new ApplicationException(String.Format("Value source '{0}' is not defined for Kinematic joint in Connector '{1}'",
                        valueSource, avmConnector.Name));
                }
                object cyphyParam;
                if (_avmCyPhyMLObjectMap.TryGetValue(avmValueSource.Value, out cyphyParam))
                {
                    act((CyPhyML.Parameter)cyphyParam);
                }
            }
        }

        protected void processPorts()
        {
            foreach (avm.PortMapTarget iteratorAVMPort in _avmPortSet)
            {
                avm.PortMapTarget avmPort = iteratorAVMPort; // avmPort CAN BE ASSIGNED TO

                object cyPhyMLObjectSrc = null;
                if (!_avmCyPhyMLObjectMap.TryGetValue(avmPort, out cyPhyMLObjectSrc))
                {
                    continue;
                }

                foreach (string iteratorAVMPortID in avmPort.PortMap)
                {
                    string otherAvmPortID = iteratorAVMPortID;

                    if (!_avmPortIDMap.ContainsKey(otherAvmPortID))
                    {
                        continue;
                    }

                    avm.PortMapTarget otherAVMPort = _avmPortIDMap[otherAvmPortID];

                    object cyPhyMLObjectDst = null;
                    if (!_avmCyPhyMLObjectMap.TryGetValue(otherAVMPort, out cyPhyMLObjectDst))
                    {
                        continue;
                    }

                    if (otherAVMPort.PortMap.Contains(avmPort.ID) && otherAVMPort.ID.CompareTo(avmPort.ID) < 0)
                    {
                        continue;
                    }

                    makeConnection(cyPhyMLObjectDst, cyPhyMLObjectSrc, typeof(CyPhyML.PortComposition).Name);
                }

                if (iteratorAVMPort is avm.cad.Plane)
                {
                    avm.cad.Plane plane = iteratorAVMPort as avm.cad.Plane;

                    foreach (string iteratorAVMPortID in plane.SurfaceReverseMap)
                    {
                        string avmPortID = iteratorAVMPortID;

                        avm.cad.Plane otherAVMPort = null;

                        if (!_avmPortIDMap.ContainsKey(avmPortID))
                        {
                            continue;
                        }

                        otherAVMPort = _avmPortIDMap[avmPortID] as avm.cad.Plane;

                        object cyPhyMLObjectDst = null;
                        if (!_avmCyPhyMLObjectMap.TryGetValue(otherAVMPort, out cyPhyMLObjectDst))
                        {
                            continue;
                        }

                        CyPhyMLClasses.SurfaceReverseMap.Connect(
                            cyPhyMLObjectDst as CyPhyML.Surface,// FIXME could be a refport in DesignImporter
                            cyPhyMLObjectSrc as CyPhyML.Surface);
                    }
                }
            }
        }
    }

    public class CyPhyMLComponentBuilder : AVM2CyPhyMLBuilder
    {
        private HashSet<avm.DomainModel> _avmDomainModelSet = new HashSet<avm.DomainModel>();


        private Dictionary<avm.DoDDistributionStatementEnum, CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum> _dodDistStatementEnumMap =
            new Dictionary<DoDDistributionStatementEnum, CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum>()
        {
            { avm.DoDDistributionStatementEnum.StatementA,      CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementA},
            { avm.DoDDistributionStatementEnum.StatementB,      CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementB},
            { avm.DoDDistributionStatementEnum.StatementC,      CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementC},
            { avm.DoDDistributionStatementEnum.StatementD,      CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementD},
            { avm.DoDDistributionStatementEnum.StatementE,      CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementE},
        };


        private bool avmCyPhyMLInteritance(string portType, Type ancestorClass)
        {
            if (!_cyPhyMLNameCreateMethodMap.ContainsKey(portType))
            {
                return false;
            }

            Type cyPhyMLClass = _cyPhyMLNameCreateMethodMap[portType].getClass();
            Type cyPhyMLInterface = cyPhyMLClass.GetInterface(cyPhyMLClass.Name);
            return cyPhyMLInterface.GetInterfaces().Contains(ancestorClass);
        }

        public CyPhyMLComponentBuilder(CyPhyML.RootFolder rootFolder, bool resetUnitLibrary = false, object messageConsoleParameter = null)
            : base(rootFolder, messageConsoleParameter: messageConsoleParameter)
        {
            init(resetUnitLibrary);
        }

        public CyPhyMLComponentBuilder(CyPhyML.Components cyPhyMLComponentParent, bool resetUnitLibrary = false, object messageConsoleParameter = null)
            : base(CyPhyMLClasses.RootFolder.GetRootFolder(cyPhyMLComponentParent.Impl.Project), messageConsoleParameter: messageConsoleParameter)
        {
            _cyPhyMLComponent = CyPhyMLClasses.Component.Create(cyPhyMLComponentParent);
            init(resetUnitLibrary);
        }

        private CyPhyMLComponentBuilder(CyPhyML.ComponentAssembly cyPhyMLComponentParent, bool resetUnitLibrary = false, object messageConsoleParameter = null)
            : base(CyPhyMLClasses.RootFolder.GetRootFolder(cyPhyMLComponentParent.Impl.Project), messageConsoleParameter: messageConsoleParameter)
        {
            _cyPhyMLComponent = CyPhyMLClasses.Component.Create(cyPhyMLComponentParent);
            init(resetUnitLibrary);
        }

        private CyPhyML.Component getComponent()
        {
            return _cyPhyMLComponent;
        }

        private void SetComponentName(string componentName)
        {
            _cyPhyMLComponent.Name = componentName;
        }

        private void SetComponentId(string avmId)
        {
            _cyPhyMLComponent.Attributes.AVMID = avmId;
        }

        private void setClassifications(List<string> categories)
        {
            foreach (string category in categories)
            {
                if (_cyPhyMLComponent.Attributes.Classifications != "")
                {
                    _cyPhyMLComponent.Attributes.Classifications += "\n";
                }

                _cyPhyMLComponent.Attributes.Classifications += category;
            }
        }

        protected CyPhyML.Component _cyPhyMLComponent;

        private void process(avm.SimpleFormula avmSimpleFormula)
        {
            CyPhyML.SimpleFormula cyphySimpleFormula = CyPhyMLClasses.SimpleFormula.Create(_cyPhyMLComponent);
            process(avmSimpleFormula, cyphySimpleFormula);
        }

        private void process(avm.ComplexFormula avmComplexFormula)
        {
            CyPhyML.CustomFormula cyphyCustomFormula = CyPhyMLClasses.CustomFormula.Create(_cyPhyMLComponent);
            processComplexFormula(avmComplexFormula, cyphyCustomFormula);
        }

        private void process(avm.Property avmProperty)
        {
            process(_cyPhyMLComponent, avmProperty);
        }

        private void process(avm.Port avmPort)
        {
            process(_cyPhyMLComponent, avmPort);
        }

        private void process(avm.Resource avmResource)
        {
            CyPhyML.Resource cyPhyMLResource = CyPhyMLClasses.Resource.Create(_cyPhyMLComponent);

            _avmCyPhyMLObjectMap.Add(avmResource, cyPhyMLResource);
            _avmResourceIDMap.Add(avmResource.ID, avmResource);

            cyPhyMLResource.Name = avmResource.Name;
            cyPhyMLResource.Attributes.Hash = avmResource.Hash;
            cyPhyMLResource.Attributes.ID = avmResource.ID;
            cyPhyMLResource.Attributes.Notes = avmResource.Notes;

            cyPhyMLResource.Attributes.Path = avmResource.Path;
            // META-3490 special-case CAD files: CyPhy resource should not contain .1
            string path = GetCreoFileWithoutVersion(avmResource.Path);
            cyPhyMLResource.Attributes.Path = path;
        }

        public static string GetCreoFileWithoutVersion(string resourcePath)
        {
            var path = resourcePath;
            Match m = cadResourceRegex.Match(resourcePath);
            if (m.Success)
            {
                path = m.Groups[1].Value + m.Groups[2].Value;
            }

            return path;
        }

        private void process(avm.Connector avmConnector)
        {
            CyPhyML.Connector cyPhyMLConnector = CyPhyMLClasses.Connector.Create(_cyPhyMLComponent);

            processConnector(avmConnector, cyPhyMLConnector);
        }

        private void process(avm.DistributionRestriction avmDistributionRestriction)
        {
            string avmDistributionRestrictionTypeName = avmDistributionRestriction.GetType().ToString();
            if (!_cyPhyMLNameCreateMethodMap.ContainsKey(avmDistributionRestrictionTypeName))
            {
                writeMessage("WARNING:  No way to create CyPhyML object from \"" + avmDistributionRestrictionTypeName + "\" avm distribution restriction.", MessageType.WARNING);
                return;
            }

            CyPhyML.DistributionRestriction cyPhyMLDistributionRestriction = null;
            _cyPhyMLNameCreateMethodMap[avmDistributionRestrictionTypeName].call(_cyPhyMLComponent, out cyPhyMLDistributionRestriction);

            _avmCyPhyMLObjectMap.Add(avmDistributionRestriction, cyPhyMLDistributionRestriction);

            cyPhyMLDistributionRestriction.Attributes.Notes = avmDistributionRestriction.Notes;

            if (avmDistributionRestriction is avm.SecurityClassification)
            {
                (cyPhyMLDistributionRestriction as CyPhyML.SecurityClassification).Attributes.Level = (avmDistributionRestriction as avm.SecurityClassification).Level;
            }
            else if (avmDistributionRestriction is avm.Proprietary)
            {
                (cyPhyMLDistributionRestriction as CyPhyML.Proprietary).Attributes.Organization = (avmDistributionRestriction as avm.Proprietary).Organization;
            }
            else if (avmDistributionRestriction is avm.ITAR)
            {
                // If there's an avm.ITAR tag in the input file, then it means the Component is ITAR.
                // We can use a constant here.
                (cyPhyMLDistributionRestriction as CyPhyML.ITAR).Attributes.RestrictionLevel = CyPhyMLClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITAR;
                cyPhyMLDistributionRestriction.Name = "ITAR";
            }
            else if (avmDistributionRestriction is avm.DoDDistributionStatement)
            {
                var avmDistStatement = avmDistributionRestriction as avm.DoDDistributionStatement;
                var cyphyMLDistStatement = cyPhyMLDistributionRestriction as CyPhyML.DoDDistributionStatement;

                cyphyMLDistStatement.Attributes.DoDDistributionStatementEnum = _dodDistStatementEnumMap[avmDistStatement.Type];
                cyphyMLDistStatement.Attributes.Notes = avmDistStatement.Notes;

                cyPhyMLDistributionRestriction.Name = "DoDDistribution_" + cyphyMLDistStatement.Attributes.DoDDistributionStatementEnum.ToString();
            }
        }

        private MgaFCO getPortInstance(IMgaModel parent, string className)
        {
            if (!_portNameTypeMap.ContainsKey(className))
            {
                return null;
            }

            MgaFCO archetype = _portNameTypeMap[className].Impl as MgaFCO;
            MgaMetaRole role = (parent.Meta as MgaMetaModel).RoleByName[archetype.Meta.Name];
            if (role == null)
            {
                return null;
            }

            return parent.DeriveChildObject(archetype, role, instance: true);
        }

        private Dictionary<avm.cyber.ModelType, CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum> d_CyberModelTypeMap = new Dictionary<avm.cyber.ModelType, CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum>()
        {
            { avm.cyber.ModelType.ESMoL, CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum.ESMoL },
            { avm.cyber.ModelType.SignalFlow, CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum.SignalFlow },
            { avm.cyber.ModelType.Simulink, CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum.Simulink }
        };

        public CyPhyML.CyberModel process(avm.cyber.CyberModel avmCyberModel, CyPhyML.Component parentComponent = null)
        {
            CyPhyML.CyberModel cyPhyMLCyberModel;
            if (parentComponent == null)
            {
                cyPhyMLCyberModel = CyPhyMLClasses.CyberModel.Create(_cyPhyMLComponent);
            }
            else
            {
                cyPhyMLCyberModel = CyPhyMLClasses.CyberModel.Create(parentComponent);
            }

            _avmCyPhyMLObjectMap.Add(avmCyberModel, cyPhyMLCyberModel);

            cyPhyMLCyberModel.Name = avmCyberModel.Name;
            cyPhyMLCyberModel.Attributes.Author = avmCyberModel.Author;
            cyPhyMLCyberModel.Attributes.Notes = avmCyberModel.Notes;
            cyPhyMLCyberModel.Attributes.Class = avmCyberModel.Class;
            cyPhyMLCyberModel.Attributes.FilePathWithinResource = avmCyberModel.Locator;
            cyPhyMLCyberModel.Attributes.ModelType = d_CyberModelTypeMap[avmCyberModel.Type];

            string fileResource = avmCyberModel.UsesResource;
            if (fileResource != null && fileResource != "")
            {
                avm.Resource res = null;
                _avmResourceIDMap.TryGetValue(fileResource, out res);
                if (res != null)
                {
                    cyPhyMLCyberModel.Attributes.FileRef = res.Path;
                }
            }
            else
            {
                writeMessage(String.Format("WARNING: No Cyber resource files found for: {0}", avmCyberModel.Name), MessageType.WARNING);
            }

            foreach (avm.modelica.Connector avmModelicaConnector in avmCyberModel.Connector)
            {
                processModelicaConnector(cyPhyMLCyberModel, avmModelicaConnector);
            }

            foreach (avm.modelica.Parameter avmModelicaParameter in avmCyberModel.Parameter)
            {
                processModelicaParameter(cyPhyMLCyberModel, avmModelicaParameter);
            }

            try
            {
                CyPhy2ComponentModel.CyPhyComponentAutoLayout.LayoutChildrenByName(cyPhyMLCyberModel);
            }
            catch (Exception)
            {
            }

            return cyPhyMLCyberModel;
        }

        private void process(avm.adamsCar.AdamsCarModel avmCarModel)
        {
            CyPhyML.CarModel cyphyMLCarModel = CyPhyMLClasses.CarModel.Create(_cyPhyMLComponent);
            _avmCyPhyMLObjectMap.Add(avmCarModel, cyphyMLCarModel);
            cyphyMLCarModel.Name = avmCarModel.Name;

            Dictionary<String, ISIS.GME.Dsml.CyPhyML.Interfaces.CarParameter> carparams = new Dictionary<string, CyPhyML.CarParameter>();
            Dictionary<String, ISIS.GME.Dsml.CyPhyML.Interfaces.CarResource> carresources = new Dictionary<string, CyPhyML.CarResource>();


            foreach (var p in avmCarModel.Parameter)
            {
                CyPhyML.CarParameter cyPhyMLCarParameter = CyPhyMLClasses.CarParameter.Create(cyphyMLCarModel);

                _avmCyPhyMLObjectMap.Add(p, cyPhyMLCarParameter);

                registerValueNode(p.Value, p);

                cyPhyMLCarParameter.Name = p.Name;
                if (p.Value != null)
                {
                    cyPhyMLCarParameter.Attributes.Value = p.Value.ValueExpression.ToString();
                }
                carparams.Add(p.ID, cyPhyMLCarParameter);
            }

            foreach (var f in avmCarModel.FileReference)
            {
                CyPhyML.CarResource cyPhyMLCarResource = CyPhyMLClasses.CarResource.Create(cyphyMLCarModel);

                _avmCyPhyMLObjectMap.Add(f, cyPhyMLCarResource);

                cyPhyMLCarResource.Attributes.ResourcePath = f.FilePath;
                cyPhyMLCarResource.Name = f.Name;

                carresources.Add(f.ID, cyPhyMLCarResource);
            }

            foreach (var f in avmCarModel.FileReference)
            {
                foreach (var conn in f.ParameterSwap)
                {
                    if (conn.Length > 1)
                    {
                        CyPhyMLClasses.CarResourceParameter.Connect(carparams[conn], carresources[f.ID]);
                    }
                }
                foreach (var conn in f.FileReferenceSwap)
                {
                    if (conn.Length > 1)
                    {
                        CyPhyMLClasses.ReferenceSwap.Connect(carresources[conn], carresources[f.ID]);
                    }
                }
            }
        }

        private void process(avm.modelica.ModelicaModel avmModelicaModel)
        {
            CyPhyML.ModelicaModel cyPhyMLModelicaModel = CyPhyMLClasses.ModelicaModel.Create(_cyPhyMLComponent);

            _avmCyPhyMLObjectMap.Add(avmModelicaModel, cyPhyMLModelicaModel);

            cyPhyMLModelicaModel.Name = string.IsNullOrWhiteSpace(avmModelicaModel.Name) ? "ModelicaModel" : avmModelicaModel.Name;
            cyPhyMLModelicaModel.Attributes.Author = avmModelicaModel.Author;
            cyPhyMLModelicaModel.Attributes.Class = avmModelicaModel.Class;
            cyPhyMLModelicaModel.Attributes.Notes = avmModelicaModel.Notes;

            foreach (avm.modelica.Redeclare avmModelicaRedeclare in avmModelicaModel.Redeclare)
            {
                CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare = CyPhyMLClasses.ModelicaRedeclare.Create(cyPhyMLModelicaModel);

                populateModelicaRedeclare(avmModelicaRedeclare, cyPhyMLModelicaRedeclare);
            }

            foreach (avm.modelica.Connector avmModelicaConnector in avmModelicaModel.Connector)
            {
                processModelicaConnector(cyPhyMLModelicaModel, avmModelicaConnector);
            }

            foreach (avm.modelica.Parameter avmModelicaParameter in avmModelicaModel.Parameter)
            {
                processModelicaParameter(cyPhyMLModelicaModel, avmModelicaParameter);
            }

            CyPhy2ComponentModel.CyPhyComponentAutoLayout.LayoutChildrenByName(cyPhyMLModelicaModel);
        }

        private void processModelicaParameter(CyPhyML.ModelicaModel cyPhyMLModelicaModel, avm.modelica.Parameter avmModelicaParameter)
        {
            CyPhyML.ModelicaParameter cyPhyMLModelicaParameter = CyPhyMLClasses.ModelicaParameter.Create(cyPhyMLModelicaModel);

            _avmCyPhyMLObjectMap.Add(avmModelicaParameter, cyPhyMLModelicaParameter);

            registerValueNode(avmModelicaParameter.Value, avmModelicaParameter);

            cyPhyMLModelicaParameter.Name = avmModelicaParameter.Locator;
            if (avmModelicaParameter.Value != null)
            {
                cyPhyMLModelicaParameter.Attributes.ID = avmModelicaParameter.Value.ID;

                if (!String.IsNullOrWhiteSpace(avmModelicaParameter.Value.Unit))
                {
                    if (_unitSymbolCyPhyMLUnitMap.ContainsKey(avmModelicaParameter.Value.Unit))
                    {
                        cyPhyMLModelicaParameter.Referred.unit = _unitSymbolCyPhyMLUnitMap[avmModelicaParameter.Value.Unit];
                    }
                    else
                    {
                        writeMessage(String.Format("WARNING: No unit lib match found for: {0}", avmModelicaParameter.Value.Unit), MessageType.WARNING);
                    }
                }
            }
        }

        private void processModelicaConnector(CyPhyML.ModelicaModel cyPhyMLModelicaModel, avm.modelica.Connector avmModelicaConnector)
        {
            CyPhyML.ModelicaConnector cyPhyMLModelicaConnector = null;

            // First, try to see if we should create an instance from the port library.
            // If so, the function getPortInstance(...) will return a new instance of the port.

            // META-1984 don't instantiate, just create a copy
            MgaFCO portInstance = getPortCopyFromLibrary(cyPhyMLModelicaModel.Impl as IMgaModel, avmModelicaConnector.Class);

            bool isInstance = (portInstance != null);
            if (isInstance)
            {
                cyPhyMLModelicaConnector = CyPhyMLClasses.ModelicaConnector.Cast(portInstance);
            }
            // If not an instance, or the connector is missing (library version of port is wrong)
            if (isInstance == false || cyPhyMLModelicaConnector == null)
            {
                cyPhyMLModelicaConnector = CyPhyMLClasses.ModelicaConnector.Create(cyPhyMLModelicaModel);
            }

            _avmCyPhyMLObjectMap.Add(avmModelicaConnector, cyPhyMLModelicaConnector);

            registerPort(avmModelicaConnector);

            cyPhyMLModelicaConnector.Name = avmModelicaConnector.Name;
            cyPhyMLModelicaConnector.Attributes.Definition = avmModelicaConnector.Definition;
            cyPhyMLModelicaConnector.Attributes.ID = avmModelicaConnector.ID;
            cyPhyMLModelicaConnector.Attributes.InstanceNotes = avmModelicaConnector.Notes;
            cyPhyMLModelicaConnector.Attributes.Locator = avmModelicaConnector.Locator;
            cyPhyMLModelicaConnector.Attributes.Class = avmModelicaConnector.Class;

            foreach (avm.modelica.Redeclare avmModelicaRedeclare in avmModelicaConnector.Redeclare)
            {
                CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare = null;
                if (isInstance)
                {
                    // We cannot create a new Redeclare; look for the existing one.
                    cyPhyMLModelicaRedeclare = cyPhyMLModelicaConnector.Children
                                                                       .ModelicaRedeclareCollection
                                                                       .Where(n => n.Name == avmModelicaRedeclare.Locator)
                                                                       .FirstOrDefault();
                }
                // If not an instance, or the redeclare is missing (library version of port is wrong)
                if (isInstance == false || cyPhyMLModelicaRedeclare == null)
                {
                    cyPhyMLModelicaRedeclare = CyPhyMLClasses.ModelicaRedeclare.Create(cyPhyMLModelicaConnector);
                }

                populateModelicaRedeclare(avmModelicaRedeclare, cyPhyMLModelicaRedeclare);
            }

            foreach (avm.modelica.Parameter avmModelicaParameter in avmModelicaConnector.Parameter)
            {
                CyPhyML.ModelicaParameter cyPhyMLModelicaParameter = null;
                if (isInstance)
                {
                    cyPhyMLModelicaParameter = cyPhyMLModelicaConnector.Children
                                                                       .ModelicaParameterCollection
                                                                       .Where(n => n.Name == avmModelicaParameter.Locator)
                                                                       .FirstOrDefault();
                }

                // If not an instance, or the parameter is missing (library version of port is wrong)
                if (isInstance == false || cyPhyMLModelicaParameter == null)
                {
                    cyPhyMLModelicaParameter = CyPhyMLClasses.ModelicaParameter.Create(cyPhyMLModelicaConnector);
                }

                _avmCyPhyMLObjectMap.Add(avmModelicaParameter, cyPhyMLModelicaParameter);
                registerValueNode(avmModelicaParameter.Value, avmModelicaParameter);
                cyPhyMLModelicaParameter.Name = avmModelicaParameter.Locator;
                if (avmModelicaParameter.Value != null)
                {
                    cyPhyMLModelicaParameter.Attributes.ID = avmModelicaParameter.Value.ID;

                    if (!String.IsNullOrWhiteSpace(avmModelicaParameter.Value.Unit))
                    {
                        if (_unitSymbolCyPhyMLUnitMap.ContainsKey(avmModelicaParameter.Value.Unit))
                        {
                            cyPhyMLModelicaParameter.Referred.unit = _unitSymbolCyPhyMLUnitMap[avmModelicaParameter.Value.Unit];
                        }
                        else
                        {
                            writeMessage(String.Format("WARNING: No unit lib match found for: {0}", avmModelicaParameter.Value.Unit), MessageType.WARNING);
                        }
                    }
                }
            }
        }


        public CyPhyML.CADModel process(avm.cad.CADModel avmCADModel, CyPhyML.Component parentComponent = null)
        {
            if (parentComponent == null)
            {
                parentComponent = _cyPhyMLComponent;
            }

            CyPhyML.CADModel cyPhyMLCADModel = CyPhyMLClasses.CADModel.Create(parentComponent);

            _avmCyPhyMLObjectMap.Add(avmCADModel, cyPhyMLCADModel);

            cyPhyMLCADModel.Name = string.IsNullOrWhiteSpace(avmCADModel.Name) ? "CADModel" : avmCADModel.Name;
            cyPhyMLCADModel.Attributes.Author = avmCADModel.Author;
            cyPhyMLCADModel.Attributes.Notes = avmCADModel.Notes;

            foreach (avm.cad.Metric avmCADMetric in avmCADModel.ModelMetric)
            {
                CyPhyML.CADMetric cyPhyMLCADMetric = CyPhyMLClasses.CADMetric.Create(cyPhyMLCADModel);
                _avmCyPhyMLObjectMap.Add(avmCADMetric, cyPhyMLCADMetric);
                cyPhyMLCADMetric.Name = avmCADMetric.Name;
                cyPhyMLCADMetric.Attributes.ParameterName = avmCADMetric.Name;

                try
                {
                    if (avmCADMetric.Value.ValueExpression is avm.FixedValue)
                    {
                        cyPhyMLCADMetric.Attributes.Value = ((avm.FixedValue)avmCADMetric.Value.ValueExpression).Value;
                    }
                    else if (avmCADMetric.Value.ValueExpression is avm.ParametricValue)
                    {
                        // This case doesn't really make sense, but Ricardo provides models with this structure.
                        // In this case, take the Fixed Value of the thing and use it as the value.
                        var avmParametricValue = avmCADMetric.Value.ValueExpression as avm.ParametricValue;
                        cyPhyMLCADMetric.Attributes.Value = (avmParametricValue.Default as avm.FixedValue).Value;
                    }
                    else
                    {
                        writeMessage("Could not parse value of CAD Metric: " + cyPhyMLCADMetric.Name, MessageType.WARNING);
                    }
                }
                catch
                {
                    writeMessage("Exception parsing the value of CAD Metric: " + cyPhyMLCADMetric.Name, MessageType.WARNING);
                }

                registerValueNode(avmCADMetric.Value, avmCADMetric);
            }

            foreach (avm.cad.Parameter avmCADParameter in avmCADModel.Parameter)
            {
                CyPhyML.CADParameter cyPhyMLCADParameter = CyPhyMLClasses.CADParameter.Create(cyPhyMLCADModel);
                _avmCyPhyMLObjectMap.Add(avmCADParameter, cyPhyMLCADParameter);
                cyPhyMLCADParameter.Name = avmCADParameter.Name;
                cyPhyMLCADParameter.Attributes.ParameterName = avmCADParameter.Name;
                cyPhyMLCADParameter.Attributes.Unit = avmCADParameter.Value.Unit;
                if (avmCADParameter.Value != null)
                {
                    Dictionary<avm.DataTypeEnum, CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum> d_AVMParamType_to_CADParamType = new Dictionary<DataTypeEnum, CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum>()
                    {
                        {avm.DataTypeEnum.Boolean, CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.Boolean },
                        {avm.DataTypeEnum.Integer, CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.Integer },
                        {avm.DataTypeEnum.Real, CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.Float},
                        {avm.DataTypeEnum.String, CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.String }
                    };
                    DataTypeEnum? avmDataType = avmCADParameter.Value.DataType;
                    if (avmCADParameter.Value.DataTypeSpecified == false)
                    {
                        avmDataType = null;
                        var avmValue = avmCADParameter.Value;
                        while (avmValue.ValueExpression != null && avmValue.ValueExpression is DerivedValue)
                        {
                            DerivedValue avmDerivedValue = (DerivedValue)avmValue.ValueExpression;
                            KeyValuePair<ValueNode, object> avmValueSource;
                            if (!_avmValueNodeIDMap.TryGetValue(avmDerivedValue.ValueSource, out avmValueSource))
                            {
                                throw new ApplicationException(String.Format("Value source '{0}' is not defined for CADParameter '{1}'",
                                    avmDerivedValue.ValueSource, avmCADParameter.Name));
                            }
                            // If the ValueSource is a Value object, we'll take its DataType
                            if (avmValueSource.Key is avm.Value)
                            {
                                var avmValueSourceTarget = avmValueSource.Key as avm.Value;
                                if (avmValueSourceTarget.DataTypeSpecified)
                                {
                                    avmDataType = avmValueSourceTarget.DataType;
                                    break;
                                }
                                avmValue = avmValueSourceTarget;
                            }
                        }
                        if (avmDataType == null)
                        {
                            throw new ApplicationException(String.Format("DataType is not specified for {0}", avmCADParameter.Name));
                        }
                    }
                    cyPhyMLCADParameter.Attributes.CADParameterType = d_AVMParamType_to_CADParamType[avmDataType.Value];

                    if (!String.IsNullOrWhiteSpace(avmCADParameter.Value.Unit))
                    {
                        if (_unitSymbolCyPhyMLUnitMap.ContainsKey(avmCADParameter.Value.Unit))
                        {
                            cyPhyMLCADParameter.Referred.unit = _unitSymbolCyPhyMLUnitMap[avmCADParameter.Value.Unit];
                        }
                        else
                        {
                            writeMessage(String.Format("WARNING: No unit lib match found for: {0}", avmCADParameter.Value.Unit), MessageType.WARNING);
                        }
                    }
                }
                if (avmCADParameter.Value.ValueExpression is avm.FixedValue)
                {
                    cyPhyMLCADParameter.Attributes.Value = ((avm.FixedValue)avmCADParameter.Value.ValueExpression).Value;
                }

                registerValueNode(avmCADParameter.Value, avmCADParameter);
            }

            foreach (avm.cad.Datum avmCADDatum in avmCADModel.Datum)
            {
                string avmDatumTypeName = avmCADDatum.GetType().ToString();
                if (!_cyPhyMLNameCreateMethodMap.ContainsKey(avmDatumTypeName))
                {
                    writeMessage("WARNING:  No way to create CyPhyML object from \"" + avmDatumTypeName + "\" avm datum.", MessageType.WARNING);
                    continue;
                }

                CyPhyML.CADDatum cyPhyMLCADDatum = null;
                _cyPhyMLNameCreateMethodMap[avmDatumTypeName].call(cyPhyMLCADModel, out cyPhyMLCADDatum);

                _avmCyPhyMLObjectMap.Add(avmCADDatum, cyPhyMLCADDatum);

                registerPort(avmCADDatum);

                if (avmCADDatum is avm.cad.Plane)
                {
                    //cyPhyMLCADDatum
                }

                // If the XML has a name, use that. Otherwise, use the datum name.
                if (!String.IsNullOrWhiteSpace(avmCADDatum.Name))
                {
                    cyPhyMLCADDatum.Name = avmCADDatum.Name;
                }
                else
                {
                    cyPhyMLCADDatum.Name = avmCADDatum.DatumName;
                }

                cyPhyMLCADDatum.Attributes.DatumName = avmCADDatum.DatumName;
                cyPhyMLCADDatum.Attributes.Definition = avmCADDatum.Definition;
                cyPhyMLCADDatum.Attributes.DefinitionNotes = avmCADDatum.Notes;
                cyPhyMLCADDatum.Attributes.InstanceNotes = avmCADDatum.Notes;
                cyPhyMLCADDatum.Attributes.ID = avmCADDatum.ID;

                foreach (avm.cad.Metric avmCADMetric in avmCADDatum.DatumMetric)
                {
                    registerValueNode(avmCADMetric.Value, avmCADMetric);
                }
            }

            try
            {
                CyPhy2ComponentModel.CyPhyComponentAutoLayout.LayoutChildrenByName(cyPhyMLCADModel);
            }
            catch (Exception)
            {
                // snyako: this call fails
                // ...
            }

            return cyPhyMLCADModel;
        }


        private void process(avm.manufacturing.ManufacturingModel avmManufacturingModel)
        {
            CyPhyML.ManufacturingModel cyPhyMLManufacturingModel = CyPhyMLClasses.ManufacturingModel.Create(_cyPhyMLComponent);

            _avmCyPhyMLObjectMap.Add(avmManufacturingModel, cyPhyMLManufacturingModel);

            cyPhyMLManufacturingModel.Name = string.IsNullOrWhiteSpace(avmManufacturingModel.Name) ? "ManufacturingModel" : avmManufacturingModel.Name;
            cyPhyMLManufacturingModel.Attributes.Author = avmManufacturingModel.Author;
            cyPhyMLManufacturingModel.Attributes.Notes = avmManufacturingModel.Notes;

            foreach (avm.manufacturing.Parameter avmManufacturingModelParameter in avmManufacturingModel.Parameter)
            {
                CyPhyML.ManufacturingModelParameter cyPhyMLManufacturingModelParameter = CyPhyMLClasses.ManufacturingModelParameter.Create(cyPhyMLManufacturingModel);
                cyPhyMLManufacturingModelParameter.Name = avmManufacturingModelParameter.Name;
                cyPhyMLManufacturingModelParameter.Attributes.Notes = avmManufacturingModelParameter.Notes;

                _avmCyPhyMLObjectMap.Add(avmManufacturingModelParameter, cyPhyMLManufacturingModelParameter);
                registerValueNode(avmManufacturingModelParameter.Value, avmManufacturingModelParameter);

                if (avmManufacturingModelParameter.Value != null)
                {
                    if (!String.IsNullOrWhiteSpace(avmManufacturingModelParameter.Value.Unit))
                    {
                        if (_unitSymbolCyPhyMLUnitMap.ContainsKey(avmManufacturingModelParameter.Value.Unit))
                        {
                            cyPhyMLManufacturingModelParameter.Referred.unit = _unitSymbolCyPhyMLUnitMap[avmManufacturingModelParameter.Value.Unit];
                        }
                        else
                        {
                            writeMessage(String.Format("WARNING: No unit lib match found for: {0}", avmManufacturingModelParameter.Value.Unit), MessageType.WARNING);
                        }
                    }
                }
            }

            CyPhy2ComponentModel.CyPhyComponentAutoLayout.LayoutChildrenByName(cyPhyMLManufacturingModel);
        }

        private void processResources()
        {
            foreach (avm.DomainModel avmDomainModel in _avmDomainModelSet)
            {
                if (_avmCyPhyMLObjectMap.ContainsKey(avmDomainModel))
                {
                    CyPhyML.DomainModel cyPhyMLDomainModel = _avmCyPhyMLObjectMap[avmDomainModel] as CyPhyML.DomainModel;
                    string usesResource = avmDomainModel.UsesResource;
                    if (!String.IsNullOrWhiteSpace(usesResource) && _avmResourceIDMap.ContainsKey(usesResource))
                    {
                        avm.Resource avmResource = _avmResourceIDMap[usesResource];
                        if (_avmCyPhyMLObjectMap.ContainsKey(avmResource))
                        {
                            CyPhyMLClasses.UsesResource.Connect(cyPhyMLDomainModel, _avmCyPhyMLObjectMap[avmResource] as CyPhyML.Resource);
                        }
                    }
                }
            }
        }

        private void setSupercedes(List<string> supercedes)
        {
            if (supercedes == null)
            {
                return;
            }

            String s_Supercedes = "";
            foreach (var id in supercedes)
            {
                if (String.IsNullOrWhiteSpace(s_Supercedes))
                {
                    s_Supercedes = id;
                }
                else
                {
                    s_Supercedes += '\n' + id;
                }
            }

            _cyPhyMLComponent.Attributes.Supercedes = s_Supercedes;
        }

        private void TellCyPhyAddonDontAssignIds()
        {
            var cyPhyAddon = project.AddOnComponents.Cast<IMgaComponentEx>().Where(x => x.ComponentName.ToLowerInvariant() == "CyPhyAddOn".ToLowerInvariant()).FirstOrDefault();
            if (cyPhyAddon != null)
            {
                cyPhyAddon.ComponentParameter["DontAssignGuidsOnNextTransaction".ToLowerInvariant()] = true;
            }
        }

        public CyPhyML.Component AVM2CyPhyMLNonStatic(avm.Component avmComponent)
        {
            TellCyPhyAddonDontAssignIds();

            SetComponentName(avmComponent.Name);
            SetComponentId(avmComponent.ID);
            if (!String.IsNullOrEmpty(avmComponent.Version))
            {
                _cyPhyMLComponent.Attributes.Version = avmComponent.Version;
            }

            setClassifications(avmComponent.Classifications);
            setSupercedes(avmComponent.Supercedes);

            foreach (avm.Property avmProperty in avmComponent.Property)
            {
                process(avmProperty);
            }

            foreach (avm.Port avmPort in avmComponent.Port)
            {
                process(avmPort);
            }

            foreach (avm.Resource avmResource in avmComponent.ResourceDependency)
            {
                process(avmResource);
            }

            foreach (avm.Connector avmConnector in avmComponent.Connector)
            {
                process(avmConnector);
            }

            foreach (avm.DistributionRestriction avmDistributionRestriction in avmComponent.DistributionRestriction)
            {
                process(avmDistributionRestriction);
            }

            foreach (avm.DomainModel avmDomainModel in avmComponent.DomainModel)
            {
                _avmDomainModelSet.Add(avmDomainModel);
            }

            foreach (avm.modelica.ModelicaModel avmModelicaModel in avmComponent.DomainModel.OfType<avm.modelica.ModelicaModel>())
            {
                process(avmModelicaModel);
            }

            foreach (avm.adamsCar.AdamsCarModel avmCarModel in avmComponent.DomainModel.OfType<avm.adamsCar.AdamsCarModel>())
            {
                process(avmCarModel);
            }


            foreach (avm.cyber.CyberModel avmCyberModel in avmComponent.DomainModel.OfType<avm.cyber.CyberModel>())
            {
                process(avmCyberModel);
            }

            bool hasCADModel = false;
            foreach (avm.cad.CADModel avmCADModel in avmComponent.DomainModel.OfType<avm.cad.CADModel>())
            {
                process(avmCADModel);
                hasCADModel = true;
            }
            if (hasCADModel)
            {
                CyPhyML.ReferenceCoordinateSystem coordSys = CyPhyMLClasses.ReferenceCoordinateSystem.Create(_cyPhyMLComponent);
                coordSys.Name = "AssemblyRoot";
            }

            foreach (avm.manufacturing.ManufacturingModel avmManufacturingModel in avmComponent.DomainModel.OfType<avm.manufacturing.ManufacturingModel>())
            {
                process(avmManufacturingModel);
            }

            foreach (var simpleFormula in avmComponent.Formula.OfType<avm.SimpleFormula>())
            {
                process(simpleFormula);
            }

            foreach (var complexFormula in avmComponent.Formula.OfType<avm.ComplexFormula>())
            {
                process(complexFormula);
            }

            processPorts();
            processValues();
            processResources();

            foreach (var analysisConstruct in avmComponent.AnalysisConstruct)
            {
                process(analysisConstruct);
            }

            DoLayout();
            if (!this.aMemberHasLayoutData)
            {
                CyPhy2ComponentModel.CyPhyComponentAutoLayout.LayoutComponent(_cyPhyMLComponent);
            }

            _cyPhyMLComponent.RunFormulaEvaluator();

            return _cyPhyMLComponent;
        }

        private void process(AnalysisConstruct analysisConstruct)
        {
            if (analysisConstruct is avm.cad.ExtrudedGeometry)
            {
                Action<List<string>, CyPhyML.Point> createPortCompositionInComponent = (referredPoints, fromPoint) =>
                {
                    foreach (string otherAvmPortID in referredPoints)
                    {
                        avm.PortMapTarget otherAVMPort = _avmPortIDMap[otherAvmPortID];

                        object cyPhyMLObjectDst = null;
                        if (!_avmCyPhyMLObjectMap.TryGetValue(otherAVMPort, out cyPhyMLObjectDst))
                        {
                            continue;
                        }

                        CyPhyMLClasses.PortComposition.Connect(fromPoint, (CyPhyML.Port)cyPhyMLObjectDst, parent: this._cyPhyMLComponent);
                    }
                };

                var extrusion = (avm.cad.ExtrudedGeometry)analysisConstruct;
                var cyphyExtrusion = CyPhyMLClasses.Extrusion.Create(_cyPhyMLComponent);
                // FIXME cyphyExtrustion.Name = extrusion.Name;
                cyphyExtrusion.Name = "Extrusion";
                if (extrusion.GeometryQualifierSpecified)
                {
                    cyphyExtrusion.Attributes.BoundaryQualifier =
                        CyPhyML2AVM.AVMComponentBuilder.extrusionBoundaryQualifierMap.First(ent => ent.Value == extrusion.GeometryQualifier).Key;
                }
                if (extrusion.PartIntersectionModifierSpecified)
                {
                    cyphyExtrusion.Attributes.GeometryModifier =
                        CyPhyML2AVM.AVMComponentBuilder.extrusionIntersectionMap.First(ent => ent.Value == extrusion.PartIntersectionModifier).Key;
                }
                if (extrusion.ExtrusionHeight != null)
                {
                    var cyphyExtrusionHeight = CyPhyMLClasses.Point.Create(cyphyExtrusion, CyPhyMLClasses.Point.Roles.Extrusion.ExtrusionHeight);
                    cyphyExtrusionHeight.Name = "ExtrusionHeight";
                    cyphyExtrusionHeight.Attributes.ID = "id-" + cyphyExtrusionHeight.Guid.ToString();
                    createPortCompositionInComponent(extrusion.ExtrusionHeight.ReferredPoint, cyphyExtrusionHeight);
                }
                if (extrusion.DirectionReferencePoint != null)
                {
                    var cyphyDirectionReference = CyPhyMLClasses.Point.Create(cyphyExtrusion, ISIS.GME.Dsml.CyPhyML.Classes.Point.Roles.Extrusion.Direction_Reference_Point);
                    if (((MgaFCO)cyphyDirectionReference.Impl).MetaRole.Name != "Direction_Reference_Point")
                    {
                        throw new ApplicationException(((MgaFCO)cyphyDirectionReference.Impl).MetaRole.Name);
                    }
                    cyphyDirectionReference.Name = "DirectionReferencePoint";
                    cyphyDirectionReference.Attributes.ID = "id-" + cyphyDirectionReference.Guid.ToString();
                    createPortCompositionInComponent(extrusion.DirectionReferencePoint.ReferredPoint, cyphyDirectionReference);
                }

                var extrusionSurface = extrusion.ExtrusionSurface;
                if (extrusionSurface is avm.cad.Polygon)
                {
                    var cyphyPolygon = CyPhyMLClasses.Polygon.Create(cyphyExtrusion);
                    cyphyPolygon.Name = "Polygon";
                    var extrusionPolygon = (avm.cad.Polygon)extrusionSurface;
                    int ordinal = 0;
                    foreach (var point in extrusionPolygon.PolygonPoint)
                    {
                        var cyphyPoint = CyPhyMLClasses.OrdinalPoint.Create(cyphyPolygon);
                        cyphyPoint.Name = String.Format("Point_{0}", ordinal);
                        cyphyPoint.Attributes.PolygonOrdinalPosition = ordinal++;
                        var cyphyPointForwarder = CyPhyMLClasses.Point.Create(cyphyExtrusion, CyPhyMLClasses.Point.Roles.Extrusion.PointDatumForwarder);
                        cyphyPointForwarder.Name = cyphyPoint.Name + "_Forwarder";

                        CyPhyMLClasses.PortComposition.Connect(cyphyPoint, cyphyPointForwarder, parent: cyphyExtrusion);

                        createPortCompositionInComponent(point.ReferredPoint, cyphyPointForwarder);
                    }
                }
                else
                {
                    // FIXME handle other shapes
                }
            }
        }

        public static CyPhyML.Component AVM2CyPhyML(CyPhyML.ComponentAssembly cyPhyMLComponentParent, avm.Component avmComponent, bool resetUnitLib = true, object messageConsole = null)
        {
            CyPhyMLComponentBuilder cyPhyMLComponentBuilder = new CyPhyMLComponentBuilder(cyPhyMLComponentParent, resetUnitLib, messageConsole);
            return cyPhyMLComponentBuilder.AVM2CyPhyMLNonStatic(avmComponent);
        }

        public static CyPhyML.Component AVM2CyPhyML(CyPhyML.Components cyPhyMLComponentParent, avm.Component avmComponent, bool resetUnitLib = true, object messageConsole = null)
        {
            CyPhyMLComponentBuilder cyPhyMLComponentBuilder = new CyPhyMLComponentBuilder(cyPhyMLComponentParent, resetUnitLib, messageConsole);
            return cyPhyMLComponentBuilder.AVM2CyPhyMLNonStatic(avmComponent);
        }
    }
}
