import sys
sys.path.insert(0, '../../lib/python')

import avm
from avm import modelica, cad
import xml.dom.minidom
from lxml import etree

if __name__ == "__main__":
    c = avm.Component()
    c.Name = "Example Component"
    c.Version = "1.2"
    
    ### Test objects from the iFAB package
    con = avm.Connector()
    con.Name = "BaseSeamTest"

    part1 = iFAB.part()
    part2 = iFAB.part()

    part1.id = "16106CF7-2643-46A1-970C-75DB50F6C76B"
    part2.id = "26106CF7-2643-46A1-970C-75DB50F6C76B"

    mechanical1 = iFAB.mechanical()
    mechanical1.fasteningMethod = "Bolted"
    mechanical1.part1 = part1
    mechanical1.part2 = part2

    mechanical1.description = "BaseSeamTest1"
    mechanical1.id = "36106CF7-2643-46A1-970C-75DB50F6C76B"

    mechanical2 = iFAB.mechanical()
    mechanical2.fasteningMethod = "Press Fit"
    mechanical2.part1 = part1
    mechanical2.part2 = part2

    mechanical2.description = "BaseSeamTest2"
    mechanical2.id = "46106CF7-2643-46A1-970C-75DB50F6C76B"

    con.DefaultJoin.append(mechanical1)
    con.DefaultJoin.append(mechanical2)
    c.Connector.append(con)
    ### End of iFAB test

    ### Build a NamedValue that represents a property of the component.
    ### With "OnDataSheet" set to true, it will not be hidden from the user
    ### while they are browsing components.
    ### The unit will be meters, and will be given as an integer.
    ### By using a FixedValue ValueExpression, we are saying that this
    ### is unchangeable.
    p1 = avm.PrimitiveProperty()
    p1.Name = "Property1"
    p1.OnDataSheet = 1
    c.Property.append(p1)

    v = avm.Value()
    v.Unit = "m"
    v.ID = "ID4"
    v.DataType = avm.DataTypeEnum.Integer
    p1.Value = v

    fv = avm.FixedValue()
    fv.Value = "34513"
    v.ValueExpression = fv

    ### Build a NamedValue that will derive its value from the previous one.
    ### This is accomplished by using a DerivedValue object for the ValueExpression,
    ### and pointing back to the first one's ID.
    p2 = avm.PrimitiveProperty()
    p2.Name = "ADerivedValue"
    p2.OnDataSheet = 0
    c.Property.append(p2)

    v2 = avm.Value()
    v2.Unit = "m"
    v2.ID = "ID5"
    v2.DataType = avm.DataTypeEnum.Integer
    p2.Value = v2

    dv2 = avm.DerivedValue()
    dv2.ValueSource = v.ID
    v2.ValueExpression = dv2

    ### Build a NamedValue that is parametric -- the user will be able to go in
    ### and assign a value for it.
    ### We do this by using a ParametricValue ValueExpression. Specification
    ### of the allowed minimum, maximum, default, and assigned values are done
    ### using ValueExpressions themselves.
    ### In this case, we will say that the Default value is derived from the
    ### value of the first NamedValue object we chose.
    ### We will also use an alternative constructor technique in building objects.
    p3 = avm.PrimitiveProperty(Name = "AParametricValue",OnDataSheet=0)
    c.Property.append(p3)

    v3 = avm.Value(Unit = "m", ID = "ID20",DataType = avm.DataTypeEnum.Real)
    p3.Value = v3

    pv3 = avm.ParametricValue()
    v3.ValueExpression = pv3

    ve3_2 = avm.DerivedValue(ValueSource = v.ID)
    pv3.Default = ve3_2

    ve3_3 = avm.FixedValue(Value = "1.0")
    pv3.Minimum = ve3_3


    ### Build a Component Connector, and give it one role.
    i = avm.Connector()
    i.Name = "BogusInterface"
    c.Connector.append(i)

    r1 = modelica.Connector()
    r1.Name = "Role1 for Modelica Connector"
    r1.Class = "Modelica.Mechanics.Rotational.Interfaces.Flange_a"
    i.Role.append(r1)

    r2 = cad.CoordinateSystem(Name = "Role2 for CAD CoordinateSystem")
    i.Role.append(r2)


    ### Create a DomainModel of type ModelicaModel.
    ### Then add one Metric and two Connectors, of different types.
    mm = modelica.ModelicaModel()
    mm.Class = "path.to.class"
    c.DomainModel.append(mm)

    mm_metric = modelica.Metric()
    mm_metric.Locator = "subsystem.max_force"
    mm_metric.ID = "ID3"
    mm_metric.Notes = "maximum force exerted over simulation"
    mm.Metric.append(mm_metric)

    mm_connector = modelica.Connector()
    mm_connector.Class = "Modelica.Mechanics.Rotational.Interfaces.Flange_a"
    mm_connector.Locator = "RotationalOut"
    mm_connector.ID = "ID1"
    mm.Connector.append(mm_connector)

    mm_translational = modelica.Connector()
    mm_translational.ID = "ID10"
    mm_translational.Class = "Modelica.Mechanics.Translational.Interfaces.Flange_a"
    mm_translational.Locator = "TranslationalInput"
    mm.Connector.append(mm_translational)

    ### Create a DomainModel of type CADModel.
    ### It has one parameter, the value of which will be derived from
    ### the first NamedValue of the Component.
    cm = cad.CADModel(Notes="A detailed CAD representation of the component")
    c.DomainModel.append(cm)

    csys = cad.CoordinateSystem(Name="MATING_POINT",ID="ID22")
    cm.Datum.append(csys)

    cparam = cad.Parameter(Name="LENGTH")
    cparam.Value = avm.Value(Unit="m",ID="ID21",DataType=avm.DataTypeEnum.Real,DimensionType=avm.DimensionTypeEnum.Scalar)
    cparam.Value.ValueExpression = avm.DerivedValue(ValueSource=v.ID)
    cm.Parameter.append(cparam)


    ### Map the roles on the Component Interface to Domain Model Ports.
    r1.DomainPort = mm_connector.ID
    r2.DomainPort = csys.ID


    ### Create a Resource object for our CADModel in order to indicate the dependent artifacts.
    resource = avm.Resource()
    resource.Name = "CAD Model"
    resource.Path = "cad/Model.prt"
    resource.ID = "ID30"
    resource.Notes = "Creo model representing this component"

    c.ResourceDependency.append(resource)
    cm.UsesResource = resource.ID

    from lxml import etree
    root = etree.fromstring(c.toxml("utf-8"))
    print etree.tostring(root, pretty_print = True)

    # ### Use minidom to pretty-print the xml
    # dom_c = xml.dom.minidom.parseString(c.toxml("utf-8"))
    # xml_c = dom_c.toprettyxml()
    # print xml_c
    #
    # f_xml = open("python_test_out.xml","w")
    # f_xml.write(xml_c)
    # f_xml.close()
    #
    #
    # ### Deserialize the java version component and print its details
    # f_javaxml = open("../java/java_test_out.xml","r")
    # xml_fromjava = f_javaxml.read()
    # dom_c_ = xml.dom.minidom.parseString(xml_fromjava)
    # c_deserialized = avm.Component.createFromDOM(dom_c_)
    #
    # print "------------------------------------------"
    # print "Deserialized Component (from java example)"
    # print "------------------------------------------"
    # print dom_c.toprettyxml()
