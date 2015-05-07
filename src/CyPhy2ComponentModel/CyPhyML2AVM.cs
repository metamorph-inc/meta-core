using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using avm;
using CyPhy2ComponentModel;
// using domain specific interfaces
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;

namespace CyPhyML2AVM {
    
    public interface CreatorBase {
        void call<BaseClass>(out BaseClass baseClassObject) where BaseClass : class;
    }

    public class Creator<Class> : CreatorBase where Class : class {

        public void call<BaseClass>(out BaseClass baseClassObject) where BaseClass : class {
            baseClassObject = Activator.CreateInstance(  typeof( Class )  ) as BaseClass;
        }

        private static Creator< Class > _singleton = new Creator<Class>();

        public static Creator< Class > get_singleton() { return _singleton; }
    }
        
    public class AVMComponentBuilder {
        
        private static void SetLayoutData(object avmObj, IMgaObject cyphyObj)
        {
            if (cyphyObj is IMgaFCO)
            {
                var cyphyFCO = cyphyObj as IMgaFCO;

                var objWrapper = new LayoutDataWrapper(avmObj);

                // Look for layout data associated with the All aspect.
                // If we find that aspect, set all the values.
                // Otherwise, we expect that avmObj's positional values will remain unset.
                Boolean hasAllAspect = false;
                foreach (IMgaPart part in cyphyFCO.Parts)
                {
                    if (part.MetaAspect.Name == "All")
                        hasAllAspect = true;
                }
                
                foreach (IMgaPart part in cyphyFCO.Parts)
                {
                    if (part.MetaAspect.Name == "All" || hasAllAspect == false)
                    {
                        String icon;
                        int xpos = 0;
                        int ypos = 0;
                        part.GetGmeAttrs(out icon, out xpos, out ypos);

                        if (xpos >= 0 && ypos >= 0)
                        {
                            objWrapper.xpos = System.Convert.ToUInt32(xpos);
                            objWrapper.ypos = System.Convert.ToUInt32(ypos);
                        }

                        return;
                    }
                }
            }
        }

        private Dictionary<CyPhyMLClasses.Property.AttributesClass.DataType_enum, avm.DataTypeEnum> _dataTypePropertyEnumMap = new Dictionary<CyPhyMLClasses.Property.AttributesClass.DataType_enum, avm.DataTypeEnum>() {
            { CyPhyMLClasses.Property.AttributesClass.DataType_enum.Boolean, avm.DataTypeEnum.Boolean },
            { CyPhyMLClasses.Property.AttributesClass.DataType_enum.Integer, avm.DataTypeEnum.Integer },
            { CyPhyMLClasses.Property.AttributesClass.DataType_enum.Float,   avm.DataTypeEnum.Real    },
            { CyPhyMLClasses.Property.AttributesClass.DataType_enum.String,  avm.DataTypeEnum.String  }
        };

        

        public static Dictionary<string, CreatorBase> _avmNameCreateMethodMap = new Dictionary<String, CreatorBase>() {
            { typeof(CyPhyMLClasses.Axis).ToString(),                       Creator<avm.cad.Axis>.get_singleton()                   },
            { typeof(CyPhyMLClasses.CoordinateSystem).ToString(),           Creator<avm.cad.CoordinateSystem>.get_singleton()       },
            { typeof(CyPhyMLClasses.Surface).ToString(),                    Creator<avm.cad.Plane>.get_singleton()                  },
            { typeof(CyPhyMLClasses.Point).ToString(),                      Creator<avm.cad.Point>.get_singleton()                  },
            { typeof(CyPhyMLClasses.AbstractPort).ToString(),               Creator<avm.AbstractPort>.get_singleton()               },
            { typeof(CyPhyMLClasses.ModelicaConnector).ToString(),          Creator<avm.modelica.Connector>.get_singleton()         },
            { typeof(CyPhyMLClasses.SecurityClassification).ToString(),     Creator<avm.SecurityClassification>.get_singleton()     },
            { typeof(CyPhyMLClasses.Proprietary).ToString(),                Creator<avm.Proprietary>.get_singleton()                },
            { typeof(CyPhyMLClasses.ITAR).ToString(),                       Creator<avm.ITAR>.get_singleton()                       },
            { typeof(CyPhyMLClasses.DoDDistributionStatement).ToString(),   Creator<avm.DoDDistributionStatement>.get_singleton()   }
        };

        private Dictionary<CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum, avm.DoDDistributionStatementEnum> _dodDistStatementEnumMap = 
            new Dictionary<CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum, DoDDistributionStatementEnum>()
        {
            { CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementA,     avm.DoDDistributionStatementEnum.StatementA },
            { CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementB,     avm.DoDDistributionStatementEnum.StatementB },
            { CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementC,     avm.DoDDistributionStatementEnum.StatementC },
            { CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementD,     avm.DoDDistributionStatementEnum.StatementD },
            { CyPhyMLClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementE,     avm.DoDDistributionStatementEnum.StatementE }
        };
                
        private Dictionary<CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum, avm.modelica.RedeclareTypeEnum> _modelicaRedeclareTypeEnumMap =
         new Dictionary<CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum, avm.modelica.RedeclareTypeEnum>() {
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Block,     avm.modelica.RedeclareTypeEnum.Block     },
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Class,     avm.modelica.RedeclareTypeEnum.Class     },
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Connector, avm.modelica.RedeclareTypeEnum.Connector },
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Function,  avm.modelica.RedeclareTypeEnum.Function  },
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Model,     avm.modelica.RedeclareTypeEnum.Model     },
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Package,   avm.modelica.RedeclareTypeEnum.Package   },
            { CyPhyMLClasses.ModelicaRedeclare.AttributesClass.ModelicaRedeclareType_enum.Record,    avm.modelica.RedeclareTypeEnum.Record    }
        };



        private static PropertyInfo getPropertyInfo(Type type, string propertyName) {
            return type.GetProperty(propertyName);
        }

        private static PropertyInfo getPropertyInfo(object object_var, string propertyName) {
            return getPropertyInfo(object_var.GetType(), propertyName);
        }

        private static Type getInterfaceType(Type type) {
            string typeName = type.UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
            return Type.GetType(typeName);
        }

        private static Type getInterfaceType(object object_var) {
            return getInterfaceType(object_var.GetType());
        }


        static private Regex _doubleRegex = new Regex(@"(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[Ee][+-][0-9]{1,3})?");
        static private string extractNumber( string input ) {
            Match m = _doubleRegex.Match( input );
            string retval = m.Value;
            if( retval == "" ) retval = "0";
            return retval;
        }

        private Dictionary<object, object> _cyPhyMLAVMObjectMap = new Dictionary<object, object>();
        public Dictionary<object, object> CyPhyMLToAVMObjectMap { get { return _cyPhyMLAVMObjectMap; } }

        private Dictionary<string, object> _idCyPhyMLObjectMap = new Dictionary<string, object>();
        private HashSet<CyPhyML.DomainModel> _cyPhyMLDomainModelSet = new HashSet<CyPhyML.DomainModel>();

        private class SourceTargetIDGroup {
            public SourceTargetIDGroup(object source, object target, string id) {
                this.source = source; this.target = target; this.id = id;
            }
            public object source { get; set; }
            public object target { get; set; }
            public string id { get; set; }
        }

        private HashSet<string> _newIDs = new HashSet<string>();

        private avm.Component _avmComponent = new avm.Component();

        private string getUnit(CyPhyML.unit cyPhyMLUnit) {

            if (cyPhyMLUnit == null) return "";

            string unitName = cyPhyMLUnit.Attributes.Symbol;
            if (unitName == "") {
                unitName = cyPhyMLUnit.Attributes.Abbreviation;
                if (unitName == "") {
                    unitName = cyPhyMLUnit.Attributes.FullName;
                }
            }

            return unitName;
        }

        private void setComponentName( string componentName ) {
            _avmComponent.Name = componentName;
        }

        private avm.Component getComponent() {
            return _avmComponent;
        }

        private int _idNo = 0;

        private String getUniqueId() {
            string newID = "ID" + _idNo++.ToString();
            while(  _idCyPhyMLObjectMap.ContainsKey( newID )  ) {
                newID = "ID" + _idNo++.ToString();
            }
            _newIDs.Add( newID );
            return newID;
        }

        private static string getIDAttribute(ISIS.GME.Common.Interfaces.Base cyPhyMLObject) {

            string id = "";

            PropertyInfo cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null) {
                PropertyInfo cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null) {
                    id = cyPhyMLIDPropertyInfo.GetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLObject, null), null) as string;
                }
            }

            return id;
        }

        private static void setIDAttribute(object cyPhyMLObject, string id)
        {
            PropertyInfo cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                PropertyInfo cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null)
                {
                    cyPhyMLIDPropertyInfo.SetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLObject, null), id, null);
                }
            }
        }

        private Dictionary<string, ISIS.GME.Common.Interfaces.Base> IDMap = new Dictionary<string, ISIS.GME.Common.Interfaces.Base>();
        private string ensureIDAttribute(ISIS.GME.Common.Interfaces.Base cyPhyMLObject)
        {
            string id = getIDAttribute(cyPhyMLObject);

            if (string.IsNullOrWhiteSpace(id)
                || !isValidXMLID(id))
            {
                id = AssignID(cyPhyMLObject);
            }
            ISIS.GME.Common.Interfaces.Base dup;
            if (IDMap.TryGetValue(id, out dup) && dup.ID != cyPhyMLObject.ID)
            {
                id = AssignID(cyPhyMLObject);
                if (getIDAttribute(dup) == id)
                {
                    id = "id-" + Guid.NewGuid().ToString("D");
                    setIDAttribute(cyPhyMLObject, id);
                }
            }
            IDMap[id] = cyPhyMLObject;
            return id;
        }

        private static string AssignID(ISIS.GME.Common.Interfaces.Base cyPhyMLObject)
        {
            string newId = "id-" + cyPhyMLObject.Guid.ToString("D");

            setIDAttribute(cyPhyMLObject, newId);
            return newId;
        }

        private static bool isValidXMLID(String id)
        {
            // FIXME this is wrong
            // NameStartChar	   ::=   	":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
            // NameChar	   ::=   	 NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
            // NCNameChar	   ::=   	NameChar - ':'
            // NCNameStartChar	   ::=   	NameStartChar - ':'
            string pattern = @"^[a-zA-Z_][\w.-]*$";
            return Regex.IsMatch(id, pattern);
        }
        
        public void createAVMProperty(CyPhyML.Property cyPhyMLProperty)
        {
            createAVMProperty(cyPhyMLProperty, _avmComponent);
        }

        public void createAVMProperty(CyPhyML.Property cyPhyMLProperty, object parent) {
            var avmPrimitiveProperty = new avm.PrimitiveProperty();

            SetLayoutData(avmPrimitiveProperty, cyPhyMLProperty.Impl);

            dynamic pParent = parent;
            pParent.Property.Add(avmPrimitiveProperty);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLProperty, avmPrimitiveProperty);

            avmPrimitiveProperty.Name = cyPhyMLProperty.Name;
            avmPrimitiveProperty.ID = "property." + ensureIDAttribute(cyPhyMLProperty);
            avmPrimitiveProperty.OnDataSheet = cyPhyMLProperty.Attributes.IsProminent;
            avmPrimitiveProperty.OnDataSheetSpecified = true;

            avm.Value avmValue = new avm.Value();
            avmPrimitiveProperty.Value = avmValue;
            avmValue.ID = ensureIDAttribute(cyPhyMLProperty);

            avmValue.DimensionType = avm.DimensionTypeEnum.Scalar;
            avmValue.DimensionTypeSpecified = true;

            avmValue.DataType = _dataTypePropertyEnumMap[cyPhyMLProperty.Attributes.DataType];
            avmValue.DataTypeSpecified = true;

            avmValue.Dimensions = cyPhyMLProperty.Attributes.Dimension;

            if (cyPhyMLProperty.Referred.unit != null)
                avmValue.Unit = cyPhyMLProperty.Referred.unit.Attributes.Symbol;

            // ValueFlow connections
            if (cyPhyMLProperty.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false).Count() > 0) {

                avm.DerivedValue avmDerivedValue = new avm.DerivedValue();
                avmValue.ValueExpression = avmDerivedValue;

                CyPhyML.ValueFlowTarget cyPhyMLValueFlowTarget = cyPhyMLProperty.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false).First().SrcEnds.ValueFlowTarget;
                string id = ensureIDAttribute(cyPhyMLValueFlowTarget);
                avmDerivedValue.ValueSource = id;
            }
            else if (cyPhyMLProperty.SrcConnections.CADMetricPortMapCollection.Where(c => c.IsRefportConnection() == false).Any()) {

                avm.DerivedValue avmDerivedValue = new DerivedValue();
                avmValue.ValueExpression = avmDerivedValue;

                CyPhyML.CADMetric cyPhyMLCadMetric = cyPhyMLProperty.SrcConnections.CADMetricPortMapCollection.Where(c => c.IsRefportConnection() == false).First().SrcEnds.CADMetric;
                String id = ensureIDAttribute(cyPhyMLCadMetric);
                avmDerivedValue.ValueSource = id;
            }
            else
            {
                avm.FixedValue avmFixedValue = new avm.FixedValue();
                avmValue.ValueExpression = avmFixedValue;
                avmFixedValue.Value = cyPhyMLProperty.Attributes.Value;
            }
        }

        private static Dictionary<CyPhyMLClasses.Parameter.AttributesClass.DataType_enum, avm.DataTypeEnum> _dataTypeParameterEnumMap = new Dictionary<CyPhyMLClasses.Parameter.AttributesClass.DataType_enum, avm.DataTypeEnum>() {
            { CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.Boolean, avm.DataTypeEnum.Boolean },
            { CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.Integer, avm.DataTypeEnum.Integer },
            { CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.Float,   avm.DataTypeEnum.Real    },
            { CyPhyMLClasses.Parameter.AttributesClass.DataType_enum.String,  avm.DataTypeEnum.String  }
        };

        public static PrimitiveProperty convertParameter(ISIS.GME.Dsml.CyPhyML.Interfaces.Parameter cyPhyMLParameter, string idParam)
        {
            avm.PrimitiveProperty avmPrimitiveProperty = new avm.PrimitiveProperty
            {
                Name = cyPhyMLParameter.Name,
                ID = "param." + idParam
            };
            SetLayoutData(avmPrimitiveProperty, cyPhyMLParameter.Impl);

            avm.Value avmValue = new avm.Value()
            {
                ID = idParam,
                DimensionType = avm.DimensionTypeEnum.Scalar,
                Dimensions = cyPhyMLParameter.Attributes.Dimension,
                DimensionTypeSpecified = true,
                DataType = _dataTypeParameterEnumMap[cyPhyMLParameter.Attributes.DataType],
                DataTypeSpecified = true
            };
            avmPrimitiveProperty.Value = avmValue;

            if (cyPhyMLParameter.Referred.unit != null)
                avmValue.Unit = cyPhyMLParameter.Referred.unit.Attributes.Symbol;

            var avmParametricValue = new avm.ParametricValue()
            {
                AssignedValue = new FixedValue()
                {
                    Value = cyPhyMLParameter.Attributes.Value
                }
            };
            avmValue.ValueExpression = avmParametricValue;

            // Export default value.
            if (!String.IsNullOrWhiteSpace(cyPhyMLParameter.Attributes.DefaultValue))
            {
                avmParametricValue.Default = new avm.FixedValue() { Value = cyPhyMLParameter.Attributes.DefaultValue };
            }
            /*else
            {
                avmParametricValue.Default = new avm.FixedValue() { Value = cyPhyMLParameter.Attributes.Value };
            }*/

            // Export range. If it's empty, skip it.
            var range = cyPhyMLParameter.Attributes.Range;
            if (!String.IsNullOrWhiteSpace(range) && (range.Contains(',') || range.Contains("..")))
            {
                foreach (var d in new string[] { "[", "(", "]", ")", " " })
                {
                    range = range.Replace(d, "");
                }
                string[] split = null;
                if (range.Contains(","))
                    split = range.Split(',');
                else if (range.Contains(".."))
                    split = Regex.Split(range, @"\.\.");

                string min = split[0];
                string max = split[1];

                if (min != "-inf")
                {
                    avmParametricValue.Minimum = new avm.FixedValue()
                    {
                        Value = min
                    };
                }
                if (max != "inf")
                {
                    avmParametricValue.Maximum = new avm.FixedValue()
                    {
                        Value = max
                    };
                }
            }

            return avmPrimitiveProperty;
        }

        public void createAVMParameter( CyPhyML.Parameter cyPhyMLParameter ) {
            avm.PrimitiveProperty avmPrimitiveProperty = convertParameter(cyPhyMLParameter, ensureIDAttribute(cyPhyMLParameter));

            var incomingVF = cyPhyMLParameter.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false).FirstOrDefault();
            var incomingCADMetric = cyPhyMLParameter.SrcConnections.CADMetricPortMapCollection.Where(c => c.IsRefportConnection() == false).FirstOrDefault();
            var valueExpression = avmPrimitiveProperty.Value.ValueExpression as avm.ParametricValue;
            if (incomingVF != null || incomingCADMetric != null)
            {
                throw new ApplicationException("Error: component '" + this._avmComponent.Name + "' has Parameter '" + cyPhyMLParameter.Name +
                        "' with incoming ValueFlow connections. Value assignments for Component Parameters must come from outside the Component itself. " +
                        "Alternatively, a Property may have incoming ValueFlow connections");
            }

            _cyPhyMLAVMObjectMap.Add(cyPhyMLParameter, avmPrimitiveProperty);
            _avmComponent.Property.Add(avmPrimitiveProperty);
        }        

        public void createAVMPort(List<avm.Port> avmPortList, CyPhyML.DomainModelPort cyPhyMLDomainModelPort, List<avm.ConnectorFeature> connectorFeatures = null) {

            string cyPhyMLDomainModelPortTypeName = cyPhyMLDomainModelPort.GetType().ToString();

            if (!_avmNameCreateMethodMap.ContainsKey(cyPhyMLDomainModelPortTypeName)) {
//                writeMessage("WARNING:  No way to create AVM object from \"" + avmPortTypeName + "\" CyPhyML DomainModelPort.", MessageType.WARNING);
                return;
            }

            avm.DomainModelPort avmDomainModelPort = null;

            bool hasBeenPortMapped = false;
            if (cyPhyMLDomainModelPort is CyPhyML.ModelicaConnector)
            {
                avm.modelica.Connector avmModelicaConnector;
                createAVMModelicaConnector(out avmModelicaConnector, cyPhyMLDomainModelPort as CyPhyML.ModelicaConnector);
                avmDomainModelPort = avmModelicaConnector;
                hasBeenPortMapped = true;
            }
            else
            {
                _avmNameCreateMethodMap[cyPhyMLDomainModelPortTypeName].call(out avmDomainModelPort);
            }

            SetLayoutData(avmDomainModelPort, cyPhyMLDomainModelPort.Impl);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLDomainModelPort, avmDomainModelPort);
            avmPortList.Add( avmDomainModelPort );

            PropertyInfo avmClassPropertyInfo = getPropertyInfo(avmDomainModelPort, "Class");

            if (avmClassPropertyInfo != null) {
                PropertyInfo cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLDomainModelPort), "Attributes");
                if (cyPhyMLAttributesPropertyInfo != null) {
                    PropertyInfo cyPhyMLClassPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("Class");
                    if (cyPhyMLClassPropertyInfo != null) {
                        avmClassPropertyInfo.SetValue(avmDomainModelPort, cyPhyMLClassPropertyInfo.GetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLDomainModelPort, null), null), null);
                    }
                }
            }

            avmDomainModelPort.Name = cyPhyMLDomainModelPort.Name;
            avmDomainModelPort.ID = ensureIDAttribute(cyPhyMLDomainModelPort);
            avmDomainModelPort.Definition = cyPhyMLDomainModelPort.Attributes.Definition;
            avmDomainModelPort.Notes = cyPhyMLDomainModelPort.Attributes.InstanceNotes;

            #region IsGuide

            if (cyPhyMLDomainModelPort is CyPhyML.CADDatum && connectorFeatures!=null)
            {
                var isGuide = ((MgaFCO)cyPhyMLDomainModelPort.Impl).get_BoolAttrByName("IsGuide");
                if (isGuide)
                {
                    connectorFeatures.Add(new avm.cad.GuideDatum
                    {
                        Datum = avmDomainModelPort.ID
                    });
                }
            }
            
            #endregion

            if (!hasBeenPortMapped)
            {
                foreach (CyPhyML.PortComposition cyPhyMLPortComposition in cyPhyMLDomainModelPort.SrcConnections.PortCompositionCollection.Where(c => c.IsRefportConnection() == false))
                {
                    CyPhyML.DomainModelPort otherCyPhyMLDomainModelPort = cyPhyMLPortComposition.SrcEnds.DomainModelPort;

                    string id = ensureIDAttribute(otherCyPhyMLDomainModelPort);
                    if (id != "")
                    {
                        avmDomainModelPort.PortMap.Add(id);
                    }
                }
                if (cyPhyMLDomainModelPort is CyPhyML.Surface)
                {
                    var surface = cyPhyMLDomainModelPort as CyPhyML.Surface;
                    var avmPlane = avmDomainModelPort as avm.cad.Plane;

                    foreach (var reverseMap in surface.SrcConnections.SurfaceReverseMapCollection.Where(c => c.IsRefportConnection() == false))
                    {
                        var otherSurface = reverseMap.SrcEnds.Surface;

                        string id = ensureIDAttribute(otherSurface);
                        if (!String.IsNullOrWhiteSpace(id))
                        {
                            avmPlane.SurfaceReverseMap.Add(id);
                        }
                        else
                        {
                            Console.Out.WriteLine("Other side has no ID");
                        }
                    }

                    // Find targets of FeatureMap connections, where the other end is a Surface, and where the ID value is not empty.
                    foreach (var targetSurfaceID in surface.SrcConnections.FeatureMapCollection
                                                                            .Where(c => c.IsRefportConnection() == false)
                                                                            .Where(featureMap => featureMap.SrcEnds.Surface != null)
                                                                            .Select(featureMap => featureMap.SrcEnds.Surface)
                                                                            .Select(targetSurface => ensureIDAttribute(targetSurface))
                                                                            .Where(targetSurfaceID => !String.IsNullOrWhiteSpace(targetSurfaceID)))
                    {
                        avmPlane.PortMap.Add(targetSurfaceID);
                    }

                    // Find targets of FeatureMap connections, where the other end is a SurfaceGeometry, andwhere the ID value is not empty.
                    foreach (var targetSurfaceID in surface.SrcConnections.FeatureMapCollection
                                                                            .Where(c => c.IsRefportConnection() == false)
                                                                            .Where(featureMap => featureMap.SrcEnds.SurfaceGeometry != null)
                                                                            .Select(featureMap => featureMap.SrcEnds.SurfaceGeometry)
                                                                            .Select(targetSurfaceGeometry => ensureIDAttribute(targetSurfaceGeometry))
                                                                            .Where(targetSurfaceGeometryID => !String.IsNullOrWhiteSpace(targetSurfaceGeometryID)))
                    {
                        avmPlane.PortMap.Add(targetSurfaceID);
                    }
                }
            }
        }

        public void createAVMPort(CyPhyML.DomainModelPort cyPhyMLDomainModelPort) {
            createAVMPort(_avmComponent.Port, cyPhyMLDomainModelPort);
        }

        public void createAVMResource(CyPhyML.Resource cyPhyMLResource) {

            avm.Resource avmResource = new avm.Resource();
            _avmComponent.ResourceDependency.Add(avmResource);

            SetLayoutData(avmResource, cyPhyMLResource.Impl);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLResource,avmResource);

            avmResource.Name = cyPhyMLResource.Name;
            avmResource.Path = cyPhyMLResource.Attributes.Path;
            avmResource.Hash = cyPhyMLResource.Attributes.Hash;
            avmResource.ID = ensureIDAttribute(cyPhyMLResource);
            avmResource.Notes = cyPhyMLResource.Attributes.Notes;
        }

        public void createAVMConnector(CyPhyML.Connector cyPhyMLConnector) {

            avm.Connector avmConnector = new avm.Connector();
            _cyPhyMLAVMObjectMap.Add(cyPhyMLConnector, avmConnector);

            SetLayoutData(avmConnector, cyPhyMLConnector.Impl);

            _avmComponent.Connector.Add(avmConnector);

            avmConnector.Name = cyPhyMLConnector.Name;
            avmConnector.Definition = cyPhyMLConnector.Attributes.Definition;
            avmConnector.Notes = cyPhyMLConnector.Attributes.InstanceNotes;
            avmConnector.ID = ensureIDAttribute(cyPhyMLConnector);

            foreach (CyPhyML.DomainModelPort cyPhyMLDomainModelPort in cyPhyMLConnector.Children.DomainModelPortCollection) {
                createAVMPort(avmConnector.Role, cyPhyMLDomainModelPort, avmConnector.ConnectorFeature);
            }
            
            
            foreach (CyPhyML.Property cyPhyMLProperty in cyPhyMLConnector.Children.PropertyCollection)
            {
                createAVMProperty(cyPhyMLProperty, avmConnector);
            }
            /*
            foreach (CyPhyML.Parameter cyPhyMLParameter in cyPhyMLConnector.Children.ParameterCollection)
            {
                createAVMParameter(cyPhyMLParameter);
            }*/
            foreach (CyPhyML.JoinData jd in cyPhyMLConnector.Children.JoinDataCollection)
            {
                var ad = JoinDataTransform.TransformJoinData(jd);
                avmConnector.DefaultJoin.Add(ad);
            }
        }

        public void createAVMDistributionRestriction(CyPhyML.DistributionRestriction cyPhyMLDistributionRestriction) {

            string cyPhyMLDistributionRestrictionTypeName = cyPhyMLDistributionRestriction.GetType().ToString();
            if (!_avmNameCreateMethodMap.ContainsKey(cyPhyMLDistributionRestrictionTypeName)) {
//                writeMessage("WARNING:  No way to create CyPhyML object from \"" + avmDistributionRestrictionTypeName + "\" avm distribution restriction.", MessageType.WARNING);
                return;
            }

            avm.DistributionRestriction avmDistributionRestriction = null;
            _avmNameCreateMethodMap[cyPhyMLDistributionRestrictionTypeName].call(out avmDistributionRestriction);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLDistributionRestriction, avmDistributionRestriction);

            avmDistributionRestriction.Notes = cyPhyMLDistributionRestriction.Attributes.Notes;

            if (cyPhyMLDistributionRestriction is CyPhyML.SecurityClassification) {
                (avmDistributionRestriction as avm.SecurityClassification).Level = (cyPhyMLDistributionRestriction as CyPhyML.SecurityClassification).Attributes.Level;
            } else if (cyPhyMLDistributionRestriction is CyPhyML.Proprietary) {
                (avmDistributionRestriction as avm.Proprietary).Organization = (cyPhyMLDistributionRestriction as CyPhyML.Proprietary).Attributes.Organization;
            }
            else if (cyPhyMLDistributionRestriction is CyPhyML.ITAR)
            {
                /* In recent revisions of the ACM spec, the mere presence of avm.ITAR means that the component is ITAR.
                 * Thus, in this code, we must check that the attribute value is an ITAR label, in which case we'll
                 * produce an avm.ITAR tag in the output. Otherwise, we should skip it.
                 */ 

                var cyPhyMLITAR = cyPhyMLDistributionRestriction as CyPhyML.ITAR;
                switch (cyPhyMLITAR.Attributes.RestrictionLevel)
                {
                    case CyPhyMLClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITAR:
                        // This is ITAR.
                        // The block should get created and added as normal.
                        break;

                    case CyPhyMLClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITARDistributionD:
                        // This is ITAR.
                        // Since it's DistributionD, we should create a DoDDistributionStatement object as well.
                        var avmDistStatement = new avm.DoDDistributionStatement() 
                        { 
                            Type = DoDDistributionStatementEnum.StatementD 
                        };
                        _avmComponent.DistributionRestriction.Add(avmDistStatement);
                        break;

                    case CyPhyMLClasses.ITAR.AttributesClass.RestrictionLevel_enum.NotITAR:
                    default:
                        // This is NOT really ITAR. 
                        // We need to delete this block and not add it to the output component.
                        // To avoid adding it, return early.
                        avmDistributionRestriction = null;
                        return;
                }
            }
            else if (cyPhyMLDistributionRestriction is CyPhyML.DoDDistributionStatement)
            {
                var cyPhyML_DistStatement = cyPhyMLDistributionRestriction as CyPhyML.DoDDistributionStatement;
                var avm_DistStatement = avmDistributionRestriction as avm.DoDDistributionStatement;

                avm_DistStatement.Notes = cyPhyML_DistStatement.Attributes.Notes;
                avm_DistStatement.Type = _dodDistStatementEnumMap[cyPhyML_DistStatement.Attributes.DoDDistributionStatementEnum];
            }

            _avmComponent.DistributionRestriction.Add(avmDistributionRestriction);
        }

        public void createAVMModelicaRedeclare(List<avm.modelica.Redeclare> avmModelicaRedeclareList, CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare) {
            avm.modelica.Redeclare avmModelicaRedeclare = new avm.modelica.Redeclare();
            avmModelicaRedeclareList.Add(avmModelicaRedeclare);

            SetLayoutData(avmModelicaRedeclare, cyPhyMLModelicaRedeclare.Impl);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLModelicaRedeclare, avmModelicaRedeclare);

            avmModelicaRedeclare.Notes = cyPhyMLModelicaRedeclare.Attributes.Notes;
            avmModelicaRedeclare.Locator = cyPhyMLModelicaRedeclare.Name;

            avmModelicaRedeclare.Type = _modelicaRedeclareTypeEnumMap[cyPhyMLModelicaRedeclare.Attributes.ModelicaRedeclareType];
            avmModelicaRedeclare.TypeSpecified = true;

            avm.Value avmValue = new avm.Value();
            avmModelicaRedeclare.Value = avmValue;

            avm.FixedValue avmFixedValue = new avm.FixedValue();
            avmValue.ValueExpression = avmFixedValue;

            avmFixedValue.Value = cyPhyMLModelicaRedeclare.Attributes.Value;
        }

        public void createAVMModelicaParameter(List<avm.modelica.Parameter> avmModelicaParameterList, CyPhyML.ModelicaParameter cyPhyMLModelicaParameter) {

            avm.modelica.Parameter avmModelicaParameter = new avm.modelica.Parameter();
            avmModelicaParameterList.Add(avmModelicaParameter);

            SetLayoutData(avmModelicaParameter, cyPhyMLModelicaParameter.Impl);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLModelicaParameter, avmModelicaParameter);

            avmModelicaParameter.Locator = cyPhyMLModelicaParameter.Name;

            avmModelicaParameter.Value = new avm.Value();


            if (cyPhyMLModelicaParameter.SrcConnections.ModelicaParameterPortMapCollection.Where(c => c.IsRefportConnection() == false).Count() > 0)
            {

                avm.DerivedValue avmDerivedValue = new avm.DerivedValue();
                avmModelicaParameter.Value.ValueExpression = avmDerivedValue;

                CyPhyML.ValueFlowTarget cyPhyMLValueFlowTarget = cyPhyMLModelicaParameter.SrcConnections.ModelicaParameterPortMapCollection.Where(c => c.IsRefportConnection() == false).First().SrcEnds.ValueFlowTarget;
                string id = ensureIDAttribute(cyPhyMLValueFlowTarget);
                avmDerivedValue.ValueSource = id;

            } else {
                avm.FixedValue avmFixedValue = new avm.FixedValue();
                avmModelicaParameter.Value.ValueExpression = avmFixedValue;
                avmFixedValue.Value = cyPhyMLModelicaParameter.Attributes.Value;
            }
            if (cyPhyMLModelicaParameter.Referred.unit != null)
                avmModelicaParameter.Value.Unit = cyPhyMLModelicaParameter.Referred.unit.Attributes.Symbol;

            avmModelicaParameter.Value.ID = ensureIDAttribute(cyPhyMLModelicaParameter);
        }

        private Dictionary<CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum, avm.cyber.ModelType> d_CyberModelTypeMap = new Dictionary<CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum, avm.cyber.ModelType>()
        {
            { CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum.ESMoL, avm.cyber.ModelType.ESMoL },
            { CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum.SignalFlow, avm.cyber.ModelType.SignalFlow },
            { CyPhyMLClasses.CyberModel.AttributesClass.ModelType_enum.Simulink, avm.cyber.ModelType.Simulink }
        };

        public void createAVMCyberModel(CyPhyML.CyberModel cyPhyMLCyberModel)
        {
            var avmCyberModel = new avm.cyber.CyberModel()
            {
                Locator = cyPhyMLCyberModel.Attributes.FilePathWithinResource,
                Type = d_CyberModelTypeMap[cyPhyMLCyberModel.Attributes.ModelType],
                Class = cyPhyMLCyberModel.Attributes.Class,
                Name = cyPhyMLCyberModel.Name,
                Notes = cyPhyMLCyberModel.Attributes.Notes,
                Author = cyPhyMLCyberModel.Attributes.Author
            };

            SetLayoutData(avmCyberModel, cyPhyMLCyberModel.Impl);
            _avmComponent.DomainModel.Add(avmCyberModel);
            _cyPhyMLAVMObjectMap.Add(cyPhyMLCyberModel, avmCyberModel);

            foreach (CyPhyML.ModelicaParameter cyPhyMLModelicaParameter in cyPhyMLCyberModel.Children.ModelicaParameterCollection)
            {
                createAVMModelicaParameter(avmCyberModel.Parameter, cyPhyMLModelicaParameter);
            }

            foreach (CyPhyML.ModelicaConnector cyPhyMLModelicaConnector in cyPhyMLCyberModel.Children.ModelicaConnectorCollection)
            {
                avm.modelica.Connector avmModelicaConnector;
                createAVMModelicaConnector(out avmModelicaConnector, cyPhyMLModelicaConnector);
                avmCyberModel.Connector.Add(avmModelicaConnector);
                _cyPhyMLAVMObjectMap.Add(cyPhyMLModelicaConnector, avmModelicaConnector);
            }
        }

        public void createAVMModelicaModel(CyPhyML.ModelicaModel cyPhyMLModelicaModel) {

            avm.modelica.ModelicaModel avmModelicaModel = new avm.modelica.ModelicaModel();

            avmModelicaModel.Name = cyPhyMLModelicaModel.Name;
            _avmComponent.DomainModel.Add(avmModelicaModel);

            SetLayoutData(avmModelicaModel, cyPhyMLModelicaModel.Impl);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLModelicaModel, avmModelicaModel);

            avmModelicaModel.Author = cyPhyMLModelicaModel.Attributes.Author;
            avmModelicaModel.Class = cyPhyMLModelicaModel.Attributes.Class;
            avmModelicaModel.Notes = cyPhyMLModelicaModel.Attributes.Notes;

            foreach (CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare in cyPhyMLModelicaModel.Children.ModelicaRedeclareCollection) {
                createAVMModelicaRedeclare(avmModelicaModel.Redeclare, cyPhyMLModelicaRedeclare);
            }

            foreach (CyPhyML.ModelicaParameter cyPhyMLModelicaParameter in cyPhyMLModelicaModel.Children.ModelicaParameterCollection) {
                createAVMModelicaParameter(avmModelicaModel.Parameter, cyPhyMLModelicaParameter);
            }

            foreach (CyPhyML.ModelicaConnector cyPhyMLModelicaConnector in cyPhyMLModelicaModel.Children.ModelicaConnectorCollection) {
                avm.modelica.Connector avmModelicaConnector;
                createAVMModelicaConnector(out avmModelicaConnector, cyPhyMLModelicaConnector);
                avmModelicaModel.Connector.Add(avmModelicaConnector);
                _cyPhyMLAVMObjectMap.Add(cyPhyMLModelicaConnector, avmModelicaConnector);
            }

        }

        private void createAVMModelicaConnector(out avm.modelica.Connector avmModelicaConnector, CyPhyML.ModelicaConnector cyPhyMLModelicaConnector)
        {
            avmModelicaConnector = new avm.modelica.Connector();

            SetLayoutData(avmModelicaConnector, cyPhyMLModelicaConnector.Impl);

            avmModelicaConnector.Name = cyPhyMLModelicaConnector.Name;
            avmModelicaConnector.Definition = cyPhyMLModelicaConnector.Attributes.Definition;
            avmModelicaConnector.ID = ensureIDAttribute(cyPhyMLModelicaConnector);
            avmModelicaConnector.Notes = cyPhyMLModelicaConnector.Attributes.InstanceNotes;
            avmModelicaConnector.Locator = cyPhyMLModelicaConnector.Attributes.Locator;
            avmModelicaConnector.Class = cyPhyMLModelicaConnector.Attributes.Class;

            foreach (CyPhyML.ModelicaRedeclare cyPhyMLModelicaRedeclare in cyPhyMLModelicaConnector.Children.ModelicaRedeclareCollection)
            {
                createAVMModelicaRedeclare(avmModelicaConnector.Redeclare, cyPhyMLModelicaRedeclare);
            }

            foreach (CyPhyML.ModelicaParameter cyPhyMLModelicaParameter in cyPhyMLModelicaConnector.Children.ModelicaParameterCollection)
            {
                createAVMModelicaParameter(avmModelicaConnector.Parameter, cyPhyMLModelicaParameter);
            }

            foreach (CyPhyML.PortComposition cyPhyMLPortComposition in cyPhyMLModelicaConnector.SrcConnections.PortCompositionCollection.Where(c => c.IsRefportConnection() == false))
            {
                CyPhyML.DomainModelPort cyPhyMLDomainModelPort = cyPhyMLPortComposition.SrcEnds.DomainModelPort;
                string id = ensureIDAttribute(cyPhyMLDomainModelPort);
                avmModelicaConnector.PortMap.Add(id);
            }
        }

        private void createAVMCADModel(CyPhyML.CADModel cyPhyMLCADModel) {

            avm.cad.CADModel avmCADModel = new avm.cad.CADModel()
            {
                Name = cyPhyMLCADModel.Name,
                Author = cyPhyMLCADModel.Attributes.Author,
                Notes = cyPhyMLCADModel.Attributes.Notes
            };

            _avmComponent.DomainModel.Add(avmCADModel);
            SetLayoutData(avmCADModel, cyPhyMLCADModel.Impl);
            _cyPhyMLAVMObjectMap.Add(cyPhyMLCADModel, avmCADModel);
            
            foreach (CyPhyML.CADMetric cyPhyMLCADMetric in cyPhyMLCADModel.Children.CADMetricCollection) {

                avm.cad.Metric avmCADMetric = new avm.cad.Metric()
                {
                    Name = String.IsNullOrWhiteSpace(cyPhyMLCADMetric.Attributes.ParameterName) 
                           ? cyPhyMLCADMetric.Name
                           : cyPhyMLCADMetric.Attributes.ParameterName,
                    ID = "cadmetric." + ensureIDAttribute(cyPhyMLCADMetric),
                    Value = new avm.Value()
                    {
                        ID = ensureIDAttribute(cyPhyMLCADMetric),
                        ValueExpression = new avm.FixedValue()
                        {
                            Value = cyPhyMLCADMetric.Attributes.Value
                        }
                    }
                };

                avmCADModel.ModelMetric.Add(avmCADMetric);
                SetLayoutData(avmCADMetric, cyPhyMLCADMetric.Impl);
                _cyPhyMLAVMObjectMap.Add(cyPhyMLCADMetric, avmCADMetric);
            }

            foreach (CyPhyML.CADParameter cyPhyMLCADParameter in cyPhyMLCADModel.Children.CADParameterCollection) {

                avm.cad.Parameter avmCADParameter = new avm.cad.Parameter();
                avmCADModel.Parameter.Add(avmCADParameter);

                SetLayoutData(avmCADParameter, cyPhyMLCADParameter.Impl);

                _cyPhyMLAVMObjectMap.Add(cyPhyMLCADParameter, avmCADParameter);
                avmCADParameter.Name = cyPhyMLCADParameter.Name;

                Dictionary<CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum, avm.DataTypeEnum> d_CADParamType_to_AVMParamType = new Dictionary<CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum, avm.DataTypeEnum>() 
                {
                    {CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.Boolean,avm.DataTypeEnum.Boolean },
                    {CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.Integer,avm.DataTypeEnum.Integer },
                    {CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.Float,avm.DataTypeEnum.Real},
                    {CyPhyMLClasses.CADParameter.AttributesClass.CADParameterType_enum.String,avm.DataTypeEnum.String }
                };
                avmCADParameter.Value = new avm.Value()
                {
                    Unit = cyPhyMLCADParameter.Attributes.Unit,
                    DataType = d_CADParamType_to_AVMParamType[cyPhyMLCADParameter.Attributes.CADParameterType],
                    DataTypeSpecified = true
                };

                if (cyPhyMLCADParameter.SrcConnections.CADParameterPortMapCollection.Where(c => c.IsRefportConnection() == false).Any())
                {

                    avm.DerivedValue avmDerivedValue = new avm.DerivedValue();
                    avmCADParameter.Value.ValueExpression = avmDerivedValue;
                    avmCADParameter.Value.ID = ensureIDAttribute(cyPhyMLCADParameter);

                    CyPhyML.ValueFlowTarget cyPhyMLValueFlowTarget = cyPhyMLCADParameter.SrcConnections.CADParameterPortMapCollection.Where(c => c.IsRefportConnection() == false).First().SrcEnds.ValueFlowTarget;

                    string id = ensureIDAttribute(cyPhyMLValueFlowTarget);
                    avmDerivedValue.ValueSource = id;
                } else {
                    avm.FixedValue avmFixedValue = new avm.FixedValue();
                    avmCADParameter.Value.ValueExpression = avmFixedValue;
                    avmFixedValue.Value = cyPhyMLCADParameter.Attributes.Value;
                }
                if (cyPhyMLCADParameter.Referred.unit != null)
                    avmCADParameter.Value.Unit = cyPhyMLCADParameter.Referred.unit.Attributes.Symbol;
            }

            foreach (CyPhyML.CADDatum cyPhyMLCADDatum in cyPhyMLCADModel.Children.CADDatumCollection) {

                string cyPhyMLCADDatumTypeName = cyPhyMLCADDatum.GetType().ToString();
                if (!_avmNameCreateMethodMap.ContainsKey(cyPhyMLCADDatumTypeName)) {
//                writeMessage("WARNING:  No way to create CyPhyML object from \"" + avmDistributionRestrictionTypeName + "\" avm distribution restriction.", MessageType.WARNING);
                    return;
                }

                avm.cad.Datum avmCADDatum = null;
                _avmNameCreateMethodMap[cyPhyMLCADDatumTypeName].call(out avmCADDatum);

                SetLayoutData(avmCADDatum, cyPhyMLCADDatum.Impl);

                avmCADDatum.Name = cyPhyMLCADDatum.Name;

                avmCADDatum.DatumName = cyPhyMLCADDatum.Attributes.DatumName;
                avmCADDatum.Definition = cyPhyMLCADDatum.Attributes.Definition;
                avmCADDatum.Notes = cyPhyMLCADDatum.Attributes.DefinitionNotes;
                avmCADDatum.Notes = cyPhyMLCADDatum.Attributes.InstanceNotes;
                avmCADDatum.ID = ensureIDAttribute(cyPhyMLCADDatum);

                foreach (CyPhyML.PortComposition cyPhyMLPortComposition in cyPhyMLCADDatum.SrcConnections.PortCompositionCollection.Where(c => c.IsRefportConnection() == false))
                {
                    CyPhyML.DomainModelPort cyPhyMLDomainModelPort = cyPhyMLPortComposition.SrcEnds.DomainModelPort;
                    string id = getIDAttribute(cyPhyMLDomainModelPort);
                    if (id != "") {
                        avmCADDatum.PortMap.Add(id);
                    }
                }

                if (cyPhyMLCADDatum is CyPhyML.Surface)
                {
                    var cyPhyMLSurface = cyPhyMLCADDatum as CyPhyML.Surface;
                    var avmPlane = avmCADDatum as avm.cad.Plane;

                    // Find targets of FeatureMap connections, where the other end is a Surface, and where the ID value is not empty.
                    foreach (var targetSurfaceID in cyPhyMLSurface.SrcConnections.FeatureMapCollection
                                                                                    .Where(c => c.IsRefportConnection() == false)
                                                                                    .Where(featureMap => featureMap.SrcEnds.Surface != null)
                                                                                    .Select(featureMap => featureMap.SrcEnds.Surface)
                                                                                    .Select(targetSurface => ensureIDAttribute(targetSurface))
                                                                                    .Where(targetSurfaceID => !String.IsNullOrWhiteSpace(targetSurfaceID)))
                    {
                        avmPlane.PortMap.Add(targetSurfaceID);
                    }

                    // Find targets of FeatureMap connections, where the other end is a SurfaceGeometry, and where the ID value is not empty.
                    foreach (var targetSurfaceID in cyPhyMLSurface.SrcConnections.FeatureMapCollection
                                                                                    .Where(c => c.IsRefportConnection() == false)
                                                                                    .Where(featureMap => featureMap.SrcEnds.SurfaceGeometry != null)
                                                                                    .Select(featureMap => featureMap.SrcEnds.SurfaceGeometry)
                                                                                    .Select(targetSurfaceGeometry => ensureIDAttribute(targetSurfaceGeometry))
                                                                                    .Where(targetSurfaceGeometryID => !String.IsNullOrWhiteSpace(targetSurfaceGeometryID)))
                    {
                        avmPlane.PortMap.Add(targetSurfaceID);
                    }

                    // Find targets of SurfaceReverseMap connections, where the other end is a Surface, and where the ID value is not empty.
                    foreach (var targetSurfaceID in cyPhyMLSurface.SrcConnections
                                                                    .SurfaceReverseMapCollection
                                                                    .Where(c => c.IsRefportConnection() == false)
                                                                    .Where(surfaceReverseMap => surfaceReverseMap.SrcEnds.Surface != null)
                                                                    .Select(surfaceReverseMap => surfaceReverseMap.SrcEnds.Surface)
                                                                    .Select(targetSurface => ensureIDAttribute(targetSurface))
                                                                    .Where(targetSurfaceID => !String.IsNullOrWhiteSpace(targetSurfaceID)))
                    {
                        avmPlane.SurfaceReverseMap.Add(targetSurfaceID);
                    }
                }

                avmCADModel.Datum.Add(avmCADDatum);
            }
        }


        private void createAVMManufacturingModel(CyPhyML.ManufacturingModel cyPhyMLManufacturingModel) {

            avm.manufacturing.ManufacturingModel avmManufacturingModel = new avm.manufacturing.ManufacturingModel();
            avmManufacturingModel.Name = cyPhyMLManufacturingModel.Name;
            _avmComponent.DomainModel.Add( avmManufacturingModel );

            SetLayoutData(avmManufacturingModel, cyPhyMLManufacturingModel.Impl);

            _cyPhyMLAVMObjectMap.Add(cyPhyMLManufacturingModel, avmManufacturingModel);

            avmManufacturingModel.Author = cyPhyMLManufacturingModel.Attributes.Author;
            avmManufacturingModel.Notes = cyPhyMLManufacturingModel.Attributes.Notes;

            foreach (CyPhyML.ManufacturingModelParameter cyPhyMLManufacturingModelParameter in cyPhyMLManufacturingModel.Children.ManufacturingModelParameterCollection) {
                avm.manufacturing.Parameter avmManufacturingModelParameter = new avm.manufacturing.Parameter();
                avmManufacturingModel.Parameter.Add(avmManufacturingModelParameter);

                SetLayoutData(avmManufacturingModelParameter, cyPhyMLManufacturingModelParameter.Impl);

                _cyPhyMLAVMObjectMap.Add(cyPhyMLManufacturingModelParameter, avmManufacturingModelParameter);

                avmManufacturingModelParameter.Name = cyPhyMLManufacturingModelParameter.Name;
                avmManufacturingModelParameter.Notes = cyPhyMLManufacturingModelParameter.Attributes.Notes;
                avmManufacturingModelParameter.Value = new avm.Value();

                if (cyPhyMLManufacturingModelParameter.SrcConnections.ManufacturingParameterPortMapCollection.Where(c => c.IsRefportConnection() == false).Count() > 0)
                {

                    avm.DerivedValue avmDerivedValue = new avm.DerivedValue();
                    avmManufacturingModelParameter.Value.ValueExpression = avmDerivedValue;

                    CyPhyML.ValueFlowTarget cyPhyMLValueFlowTarget = cyPhyMLManufacturingModelParameter.SrcConnections.ManufacturingParameterPortMapCollection.Where(c => c.IsRefportConnection() == false).First().SrcEnds.ValueFlowTarget;
                    string id = getIDAttribute(cyPhyMLValueFlowTarget);
                    if (id != "") {
                        avmDerivedValue.ValueSource = id;
                    }

                } else {
                    avm.FixedValue avmFixedValue = new avm.FixedValue();
                    avmManufacturingModelParameter.Value.ValueExpression = avmFixedValue;
                    avmFixedValue.Value = cyPhyMLManufacturingModelParameter.Attributes.Value;
                }
                if (cyPhyMLManufacturingModelParameter.Referred.unit != null)
                    avmManufacturingModelParameter.Value.Unit = cyPhyMLManufacturingModelParameter.Referred.unit.Attributes.Symbol;
            }

        }


        private avm.Value getValueAttribute(object avmObject) {

            avm.Value avmValue = null;

            PropertyInfo avmValuePropertyInfo = getPropertyInfo(getInterfaceType(avmObject), "Value");
            if (avmValuePropertyInfo != null) {
                avmValue = avmValuePropertyInfo.GetValue(avmObject, null) as avm.Value;
            }

            return avmValue;
        }

        private void setSupercedes(String supercedes)
        {
            if (String.IsNullOrWhiteSpace(supercedes))
                _avmComponent.Supercedes = null;
            else
                _avmComponent.Supercedes = supercedes.Split('\n').ToList<String>();
        }

        public avm.Component CyPhyML2AVMNonStatic(CyPhyML.Component cyPhyMLComponent) {
            cyPhyMLComponent.RunFormulaEvaluator();
            setComponentName( cyPhyMLComponent.Name );
            setSupercedes(cyPhyMLComponent.Attributes.Supercedes);

            if (string.IsNullOrWhiteSpace(cyPhyMLComponent.Attributes.AVMID))
            {
                cyPhyMLComponent.Attributes.AVMID = cyPhyMLComponent.Guid.ToString("D");
            }

            _avmComponent.ID = cyPhyMLComponent.Attributes.AVMID;
            
            _avmComponent.Version = cyPhyMLComponent.Attributes.Version;

            // Set Classifications
            _avmComponent.Classifications.AddRange(cyPhyMLComponent.Attributes.Classifications.Split('\n'));

            foreach( CyPhyML.Property cyPhyMLProperty in cyPhyMLComponent.Children.PropertyCollection ) {
                createAVMProperty( cyPhyMLProperty );
            }

            foreach( CyPhyML.Parameter cyPhyMLParameter in cyPhyMLComponent.Children.ParameterCollection ) {
                createAVMParameter( cyPhyMLParameter );
            }

            foreach (CyPhyML.DomainModelPort cyPhyMLDomainModelPort in cyPhyMLComponent.Children.DomainModelPortCollection) {
                createAVMPort(cyPhyMLDomainModelPort);
            }

            foreach (CyPhyML.Resource cyPhyMLResource in cyPhyMLComponent.Children.ResourceCollection) {
                createAVMResource(cyPhyMLResource);
            }

            foreach (CyPhyML.Connector cyPhyMLConnector in cyPhyMLComponent.Children.ConnectorCollection) {
                createAVMConnector(cyPhyMLConnector);
            }

            foreach (CyPhyML.DistributionRestriction cyPhyMLDistributionRestriction in cyPhyMLComponent.Children.DistributionRestrictionCollection) {
                createAVMDistributionRestriction(cyPhyMLDistributionRestriction);
            }

            foreach (CyPhyML.ModelicaModel cyPhyMLModelicaModel in cyPhyMLComponent.Children.ModelicaModelCollection) {
                _cyPhyMLDomainModelSet.Add(cyPhyMLModelicaModel);

                if (cyPhyMLModelicaModel is CyPhyML.CyberModel)
                {
                    createAVMCyberModel(cyPhyMLModelicaModel as CyPhyML.CyberModel);
                }
                else
                {
                    createAVMModelicaModel(cyPhyMLModelicaModel);
                }
            }

            foreach (CyPhyML.CADModel cyPhyMLCADModel in cyPhyMLComponent.Children.CADModelCollection) {
                _cyPhyMLDomainModelSet.Add(cyPhyMLCADModel);
                createAVMCADModel(cyPhyMLCADModel);
            }

            foreach (CyPhyML.ManufacturingModel cyPhyMLManufacturingModel in cyPhyMLComponent.Children.ManufacturingModelCollection) {
                _cyPhyMLDomainModelSet.Add(cyPhyMLManufacturingModel);
                createAVMManufacturingModel(cyPhyMLManufacturingModel);
            }

            foreach (CyPhyML.SimpleFormula cyPhyMLSimpleFormula in cyPhyMLComponent.Children.SimpleFormulaCollection)
            {
                createAVMSimpleFormula(cyPhyMLSimpleFormula);
            }

            foreach (CyPhyML.CustomFormula cyPhyMLCustomFormula in cyPhyMLComponent.Children.CustomFormulaCollection)
            {
                createAVMComplexFormula(cyPhyMLCustomFormula);
            }

            foreach( CyPhyML.DomainModel cyPhyMLDomainModel in _cyPhyMLDomainModelSet ) {
                foreach (CyPhyML.UsesResource cyPhyMLUsesResource in cyPhyMLDomainModel.DstConnections.UsesResourceCollection.Where(c => c.IsRefportConnection() == false))
                {
                    CyPhyML.Resource cyPhyMLResource = cyPhyMLUsesResource.DstEnds.Resource;
                    avm.DomainModel avmDomainModel = _cyPhyMLAVMObjectMap[cyPhyMLDomainModel] as avm.DomainModel;
                    avmDomainModel.UsesResource = ensureIDAttribute(cyPhyMLResource);
                }
                foreach (CyPhyML.UsesResource cyPhyMLUsesResource in cyPhyMLDomainModel.SrcConnections.UsesResourceCollection.Where(c => c.IsRefportConnection() == false))
                {
                    CyPhyML.Resource cyPhyMLResource = cyPhyMLUsesResource.SrcEnds.Resource;
                    avm.DomainModel avmDomainModel = _cyPhyMLAVMObjectMap[cyPhyMLDomainModel] as avm.DomainModel;
                    avmDomainModel.UsesResource = ensureIDAttribute(cyPhyMLResource);
                }
            }

            return _avmComponent;
        }

        private void createAVMComplexFormula(CyPhyML.CustomFormula cyPhyMLCustomFormula)
        {
            avm.ComplexFormula a_cf = new avm.ComplexFormula()
            {
                Expression = cyPhyMLCustomFormula.Attributes.Expression,
                ID = ensureIDAttribute(cyPhyMLCustomFormula),
                Name = cyPhyMLCustomFormula.Name
            };
            _avmComponent.Formula.Add(a_cf);

            SetLayoutData(a_cf, cyPhyMLCustomFormula.Impl);

            // Find operands
            foreach (var src in cyPhyMLCustomFormula.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false))
            {
                var target = src.SrcEnds.ValueFlowTarget;
                string id = ensureIDAttribute(target);

                var operand = new Operand()
                {
                    ValueSource = id
                };
                if (false == String.IsNullOrWhiteSpace(src.Attributes.FormulaVariableName))
                    operand.Symbol = src.Attributes.FormulaVariableName;
                else if (String.IsNullOrWhiteSpace(operand.Symbol))
                    operand.Symbol = target.Name;
                else
                {
                    // ERROR!!!
                }

                a_cf.Operand.Add(operand);
            }
        }

        private static Dictionary<CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum, avm.SimpleFormulaOperation> sfOperatorMap 
                = new Dictionary<CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum, avm.SimpleFormulaOperation>()
                    {
                        {CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Addition,avm.SimpleFormulaOperation.Addition},
                        {CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.ArithmeticMean,avm.SimpleFormulaOperation.ArithmeticMean},
                        {CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.GeometricMean,avm.SimpleFormulaOperation.GeometricMean},
                        {CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Maximum,avm.SimpleFormulaOperation.Maximum},
                        {CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Minimum,avm.SimpleFormulaOperation.Minimum},
                        {CyPhyMLClasses.SimpleFormula.AttributesClass.Method_enum.Multiplication,avm.SimpleFormulaOperation.Multiplication}
                    };

        private void createAVMSimpleFormula(CyPhyML.SimpleFormula c_sf)
        {
            avm.SimpleFormula a_sf = new SimpleFormula()
            {
                ID = ensureIDAttribute(c_sf),
                OperationSpecified = true,
                Operation = sfOperatorMap[c_sf.Attributes.Method],
                Name = c_sf.Name
            };
            _avmComponent.Formula.Add(a_sf);

            SetLayoutData(a_sf, c_sf.Impl);

            // Find operands
            foreach (var src in c_sf.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false))
            {
                var target = src.SrcEnds.ValueFlowTarget;
                string id = ensureIDAttribute(target);
                a_sf.Operand.Add(id);
            }
        }
 
        public static avm.Component CyPhyML2AVM( CyPhyML.Component cyPhyMLComponent ) {

            AVMComponentBuilder avmComponentBuilder = new AVMComponentBuilder();
            return avmComponentBuilder.CyPhyML2AVMNonStatic( cyPhyMLComponent );
        }
    }

    public static class RefportExtension
    {
        public static bool IsRefportConnection(this ISIS.GME.Common.Interfaces.Connection connection)
        {
            IMgaSimpleConnection conn = ((IMgaSimpleConnection)connection.Impl);
            var srcRefs = conn.SrcReferences;
            if (srcRefs != null && srcRefs.Count > 0)
                return true;
            var dstRefs = conn.DstReferences;
            if (dstRefs != null && dstRefs.Count > 0)
                return true;
            return false;
        }
    }
}
