from xml.etree.ElementTree import Element, SubElement, ElementTree, Comment, tostring
import sys
import os
import logging
from time import gmtime, strftime
from xml.dom import minidom
import cad_library.allowable_stress_levels as allowable_stress_levels
import cad_library.material_properties as material_properties

gVersion = '1.0.0.2'
gConfigurationID = ''
gStructuralMetricTypes = ['FactorOfSafety', 'MaximumDisplacement', 'VonMisesStress', 'Bearing', 'Shear']
gThermalMetricTypes = ['MaximumTemperature', 'MinimumTemperature']
gQualityLookup = {1: 'LOW', 2: 'MEDIUM', 3: 'HIGH'}
gMetricSummary = dict()             # id, (value, unit)

module_logger = logging.getLogger('')


class ComponentData:
    def __init__(self, componentID, elementID):
        self.MetricsInfo = {}           # type, id
        self.MetricsOutput = {}         # id, value
        self.UnitInfo = {}              # id, unit_string
        self.MaterialProperty = {}      # type, value
        self.Allowables = None
        self.ComponentID = componentID  # string
        self.ElementID = elementID      # string
        self.QualityOutput = {}         # type, quality evaluation
        self.RangeInfo = {}             # type, range evaluation
        self.OutOfRangeDistorted = []
        self.CadType = ''               # Patran_PP
        self.FEAResults = {}            # Patran_PP
        self.Children = []              # children cID's
        self.IsConfigurationID = False

    def __str__(self):
        stringRep = 'Component: ' + self.ComponentID + ' FEAElementID: ' + self.ElementID + '\n'
        for key, value in self.MaterialProperty.iteritems():
            stringRep += "".join(['Material %s:%s ' % (key, value)])
        stringRep += '\n'
        for key, value in self.MetricsInfo.iteritems():
            stringRep += "".join(['Metrics %s:%s ' % (key, value)])
        return stringRep

        
def CreateMaterialDict(rootNode):
    logger = logging.getLogger()
    logger.info('\n\nFUNCTION: CreateMaterialDict()')
    materialDict = {}
    materialNode = rootNode.findall('./Materials/Material')
    for material in materialNode:
        bStressNode = material.find('MaterialProperties/AllowableBearingStress')
        sStressNode = material.find('MaterialProperties/AllowableShearStress')  
        tStressNode = material.find('MaterialProperties/AllowableTensileStress')
        
        bStress = [bStressNode.attrib.get('Value'), bStressNode.attrib.get('Units')]
        sStress = [sStressNode.attrib.get('Value'), sStressNode.attrib.get('Units')]
        tStress = [tStressNode.attrib.get('Value'), tStressNode.attrib.get('Units')]
        
        for s in [bStress, sStress, tStress]:
            if any(v is None for v in s):
                LogException('Material has no value/units: ' + str(s))
            if s[1] != 'MPa':
                LogException('Material units should be MPa and not ' + key['Unit'])
                
        temp = {'Bearing': bStress[0], 'Shear': sStress[0], 'Mises': tStress[0]}
        materialDict[material.attrib.get('MaterialID')] = temp
    
    return materialDict
    

def recurseTree(componentNodes, gComponentList, adams, odbName):
    """ Recursively search assembly tree as represented by CAD metrics file. """
    for node in componentNodes:
        cID = node.attrib.get('ComponentInstanceID')
        eID = node.attrib.get('FEAElementID').upper()
        if eID == '':
            # Name is used for unique name in model-based, not PSOLIDs
            # This file is updated in model-based before PP to replace
            # non-unique name with internal Abaqus unique name.
            eID = node.attrib.get('Name').upper()
        if adams and eID != odbName:
            continue
        aComponentData = ComponentData(cID, eID)        # create a new ComponentData
        mtrl = {}
        if node.attrib.get('Type') == 'PART':
            aComponentData.CadType = 'PART'
            componentMaterial = node.attrib.get('MaterialID')
            if componentMaterial:
                material_properties.get_props_from_material_library([componentMaterial.lower()], mtrl)
                aComponentData.Allowables = \
                    allowable_stress_levels.AnalysisMaterialPropertiesAllowables(componentMaterial)
                allowable_stress_levels.compute_allowable_stress_levels(
                    allowable_stress_levels.MECHANICAL_INFINITE_CYCLES_INDICATOR,
                    mtrl[componentMaterial.lower()],
                    aComponentData.Allowables)
				# The material library properties are in Pa. The FEA run would be in N, mm, MPa.
				# Must convert the allowables to MPa; otherwise, the factor of safety would be wrong.
                allowable_stress_levels.convert_allowables_from_Pa_to_MPa(aComponentData.Allowables)					
        else:
            # Get direct descending children (in adams asm is treated as merged part)
            if gConfigurationID == cID:
                aComponentData.IsConfigurationID = True
            children = []
            for c in node.findall('./Component'):
                children.append(c.attrib.get('ComponentInstanceID'))
                componentMaterial = c.attrib.get('MaterialID')
            if adams:
                material_properties.get_props_from_material_library([componentMaterial.lower()], mtrl)
                aComponentData.Allowables = \
                    allowable_stress_levels.AnalysisMaterialPropertiesAllowables(componentMaterial)
                allowable_stress_levels.compute_allowable_stress_levels(
                    allowable_stress_levels.MECHANICAL_INFINITE_CYCLES_INDICATOR,
                    mtrl[componentMaterial.lower()],
                    aComponentData.Allowables)
            else:
                aComponentData.Children = children
            aComponentData.CadType = 'ASSEMBLY'

            recurseTree(node, gComponentList, adams, odbName)

        gComponentList[aComponentData.ComponentID] = aComponentData
    return


def ParseMetaDataFile(xmlname, odbName, adams):
    logger = logging.getLogger()
    gComponentList = dict()
    try:
        global gConfigurationID
        
        tree = ElementTree()
        tree.parse(xmlname) 
    except Exception, inst:
        LogException('Unexpected error opening ' + xmlname + str(inst))
    
    rootNode = tree.getroot()  # CADAnalysisMetaData
    
    assemblyNode = rootNode.find('Assemblies/Assembly')
    gConfigurationID = assemblyNode.attrib.get('ConfigurationID')
    logger.info('ConfigID: ' + gConfigurationID)

    CreateMaterialDict(rootNode)
    
    logger.info('\n\nFUNCTION: ParseMetaDataFile()')

    componentNodes = assemblyNode.findall('.//Component')
    recurseTree(componentNodes, gComponentList, adams, odbName)

    return gComponentList


def ParseReqMetricsFile(xmlname, componentList=None):
    """ componentList exists when file is initially processed.
        When file is parsed again once ComputedValues.xml has
        been created, componentList does not exist. At this 
        point you are only interested in the metric ID & values 
    """
    logger = logging.getLogger()
    try:
        global gStructuralMetricTypes
        global gThermalMetricTypes
        global gMetricSummary
        
        tree = ElementTree()
        tree.parse(xmlname) 
    except Exception, inst:
        LogException('Unexpected error opening ' + xmlname + str(inst))
        
    logger.info('FUNCTION: ParseMetricsFile()')
    reqMetrics = []
    rootNode = tree.getroot()  # Metrics

    for metric in rootNode.iter('Metric'):
        metricID = metric.attrib.get('MetricID')
        if componentList is not None:
            if metric.attrib.get('ComponentInstanceID') not in componentList:
                continue
            if metric.attrib.get('TopAssemblyComponentInstanceID') != gConfigurationID:
                LogException('Metric is associated with top level assembly ID ' +
                             'not matching Configuration ID above!')
            metricType = metric.attrib.get('MetricType')

            if metricType in gStructuralMetricTypes:
                reqMetrics.extend(['S', 'U'])
            elif metricType in gThermalMetricTypes:
                reqMetrics.append('TEMP')
            else:
                continue

            aComponentData = componentList[metric.attrib.get('ComponentInstanceID')]
            aComponentData.MetricsInfo[metricType] = metricID
            aComponentData.MetricsOutput[metricID] = metric.attrib.get('ArrayValue')
            
            if len(aComponentData.Children) > 0:
                # Add metric entries for the sub-asm children
                for c in aComponentData.Children:
                    componentList[c].MetricsInfo[metricType] = metricID
                    componentList[c].MetricsOutput[metricID] = metric.attrib.get('ArrayValue')

        gMetricSummary[metricID] = (metric.attrib.get('Units'), metric.attrib.get('ArrayValue'))

    return set(reqMetrics)      

    
def WriteXMLFile(componentList):    
    """ Writes requested metric values to ComputedValues.xml. """
    #NS_XSI = '{http://www.w3.org/2001/XMLSchema-instance}'
    #root.set(NS_XSI + 'noNamespaceSchemaLocation', 'CADPostProcessingParameters.xsd')

    global gConfigurationID
    logger = logging.getLogger()
    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    logger.info('FUNCTION: WriteXMLFile()\n')
    root = Element('Metrics')
    root.set('xmlns:' + 'xsi', 'http://www.w3.org/2001/XMLSchema-instance')
    root.set('xsi:' + 'noNamespaceSchemaLocation', 'CADAnalysisMetaData.xsd')
    #root.set('ConfigurationID', gConfigurationID)
    comment = Comment('ComputedMetricsSummary module: v' + gVersion + ' ' + strftime("%Y-%m-%d %H:%M:%S", gmtime()))
    root.append(comment)

    logger.info('ConfigurationID: ' + gConfigurationID)
            
    for component in componentList.values():
        if component.ComponentID.startswith('NON_CYPHY_ID_'):
            # Do not write children to XML - their metric data
            # is already encompassed in it's parent's data
            continue
        logger.info(component.ComponentID + ', ' + component.ElementID)

        for key, value in component.MetricsInfo.items():
            if value in component.MetricsOutput:
                metricType = key
                if 'Stress' in key:
                    metricUnits = 'MPa'
                elif 'Temperature' in key:
                    metricUnits = 'K'
                elif key == 'FactorOfSafety':
                    metricUnits = ''
                else:
                    metricUnits = ''
                SubElement(root, 'Metric', {'ComponenInstancetID': str(component.ComponentID),
                                            'ArrayValue': str(component.MetricsOutput[value]),
                                            'MetricID': str(value),
                                            'Type': str(metricType),
                                            'Units': str(metricUnits),
                                            'DataFormat': 'Scalar'})

    pretty = minidom.parseString(tostring(root, encoding='utf-8')).toprettyxml()
    with open('ComputedValues.xml', 'w') as cvals:
        cvals.write(pretty)

    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    return 'ComputedValues.xml'


def WriteMetric2File(gComponentList):
    print 'WriteMetric2File()'
    global gConfigurationID

    PrintComponentList(gComponentList)
    
    csvFile = open('OutMetrics.csv', 'w')
    xmlFile = open('OutMetrics.xml', 'w')

    csvFile.write('ConfigurationID[' + gConfigurationID + ']\n\n')
    xmlFile.write('<RootObjectType>\n')
    xmlFile.write('<Configurations>\n')
    xmlFile.write('<Configuration ID=\"' + gConfigurationID + '\" Name=\"\">\n')

    for component in gComponentList.values():
        if component.ComponentID.startswith('NON_CYPHY_ID_'):
            continue
        csvFile.write(component.ComponentID + ':' + component.ElementID + ',')
        
        for key, value in component.MetricsInfo.items():
            if value in component.MetricsOutput:
                csvFile.write(key + '(' + value + ')' + ',' + str(component.MetricsOutput[value]) + ',')
                xmlFile.write('<ConfigMetric ComponentInstanceID=\"' + component.ComponentID +
                              '\" DefID=\"' + value + '\" Value=\"' + str(component.MetricsOutput[value]) + '\"/>\n')
            if key in component.QualityOutput:
                csvFile.write(',' + component.QualityOutput[key] + '\n')
        csvFile.write('\n')
    xmlFile.write('</Configuration>\n')
    xmlFile.write('</Configurations>\n')
    xmlFile.write('</RootObjectType>\n')

    csvFile.close()
    xmlFile.close()


def LogException(exception_msg):
    logger = logging.getLogger()
    logger.error(exception_msg)
    raise Exception(exception_msg)


def PrintComponentList(componentList):
    logger = logging.getLogger()
    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    logger.info('FUNCTION: PrintComponentList()\n')
    #Write2Log(str(len(componentList))
              
    for component in componentList.values():
        logger.info('Component ID: ' + component.ComponentID)
        logger.info('ElementID: ' + component.ElementID)
        
        logger.info('~~~ Material Properties ~~~')
        for item in component.MaterialProperty:  # type, value
            logger.info(item + ': ' + component.MaterialProperty[item])
            
        logger.info('~~~ Metrics Info ~~~')
        for item in component.MetricsInfo:       # type, id
            logger.info(item + ': ' + component.MetricsInfo[item])

        logger.info('~~~ Metrics Output ~~~')
        for item in component.MetricsOutput:     # type, id
            logger.info(item + ': ' + str(component.MetricsOutput[item]))

        logger.info('~~~ Metrics Quality ~~~')
        for item in component.QualityOutput:     # type, id
            logger.info(item + ': ' + component.QualityOutput[item])

        logger.info('~~~ Range ~~~')
        for item in component.RangeInfo:
            logger.info(item + ': ' + component.RangeInfo[item])

        # Uncomment for debugging.
        #logger.info('~~~ Distorted Out of Range IDs ~~~')
        #for item in component.OutOfRangeDistorted:
        #   logger.info(str(item))

    logger.info('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')
