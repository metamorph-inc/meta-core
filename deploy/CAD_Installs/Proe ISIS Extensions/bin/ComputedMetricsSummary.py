from xml.etree.ElementTree import Element, SubElement, ElementTree, Comment
import sys
import logging
from time import gmtime, strftime

gVersion = '1.0.0.2'
gConfigurationID = ''
gStructuralMetricTypes = ['FactorOfSafety', 'MaximumDisplacement', 'Mises', 'Bearing', 'Shear']
gQualityLookup = {1:'LOW', 2:'MEDIUM', 3:'HIGH'}
gMetricSummary = dict()             #id, (value, unit)

module_logger = logging.getLogger('')

class ComponentData:
    def __init__(self, componentID, elementID):
        self.MetricsInfo = {}     	#type, id
        self.MetricsOutput = {}	  	#id, value
        self.UnitInfo = {}          #id, unit_string
        self.MaterialProperty = {}	#type, value
        self.ComponentID = componentID	#string
        self.ElementID = elementID	#string  
        self.QualityOutput = {}         #type, quality evaluation
        self.RangeInfo = {}             #type, range evaluation
        self.OutOfRangeDistorted = []

        
    def __str__(self):
        stringRep = 'Component: ' + self.ComponentID + ' FEAElementID: '+ self.ElementID +'\n'
        #stringRep += 'Material Bearing: {Bearing} Material Shear: {Shear} Material Mises: {Mises}'.format(**self.MaterialProperty)
        for key, value in self.MaterialProperty.iteritems():
            stringRep += "".join(['Material %s:%s ' % (key, value)])
        stringRep += '\n'
        for key, value in self.MetricsInfo.iteritems():
            stringRep += "".join(['Metrics %s:%s ' % (key, value) ]) 
        return stringRep


def ParseXMLFile(xmlname):
    logger = logging.getLogger()
    gComponentList = dict()
    try:
        global gConfigurationID
        global gStructuralMetricTypes
        global gMetricSummary
        
        tree = ElementTree()
        tree.parse(xmlname)
        rootNode = tree.getroot()
        if rootNode.attrib.has_key('ConfigurationID'):
            gConfigurationID= rootNode.attrib.get('ConfigurationID')
            logger.info('ConfigID: ' + gConfigurationID)

        logger.info('\n\nFUNCTION: ParseXMLFile()')
        rootNode.findall('Component')
        for node in rootNode:
            cID = node.attrib.get('ComponentInstanceID')
            eID = node.attrib.get('FEAElementID')
            aComponentData = ComponentData(cID, eID)        # create a new ComponentData

            for subE in list(node):
                logger.info('Printing subElement tag: ' + subE.tag)
                if subE.tag == 'Material':
                    if not subE.attrib.has_key('Bearing'):
                        LogException('Material has no Bearing Strength attribute!')# throw exception
                    if not subE.attrib.has_key('Shear'):
                        LogException('Material has no Shear Strength attribute!')
                    if not subE.attrib.has_key('Mises'):
                        LogException('Material has no Mises Strength attribute!')
                    if not subE.attrib.has_key('Units'):
                        LogException('Material has no Units attribute!')
                    else:
                        if subE.get('Units') != 'MPa':
                            LogException('Material units should be MPa and not ' + matUnits)
                    aComponentData.MaterialProperty = subE.attrib
                    #print aComponentData.MaterialProperty
                elif subE.tag == 'Metrics':
                    for metricSub in subE:
                        metricType = metricSub.get('Type')
                        metricID = metricSub.get('MetricID')
                        logger.info('MetricType: ' + metricType)
                        #if metricType not in gStructuralMetricTypes:
                        #    LogException('Invalid Metric Type: ' + metricType)
                        aComponentData.MetricsInfo[metricType] = metricID
                        aComponentData.UnitInfo[metricID] = metricSub.get('Units')
                        aComponentData.MetricsOutput[metricID] = metricSub.get('ArrayValue')

                        gMetricSummary[metricID] = (metricSub.get('Units'), metricSub.get('ArrayValue'))

            #print aComponentData
            gComponentList[aComponentData.ElementID] = aComponentData           # append to global list
        
    except Exception, inst:
        logger.info('Unexpected error opening ' + xmlname + str(inst))

    return gComponentList

def WriteXMLFile(componentList):    
    #NS_XSI = '{http://www.w3.org/2001/XMLSchema-instance}'
    #root.set(NS_XSI + 'noNamespaceSchemaLocation', 'CADPostProcessingParameters.xsd')

    global gConfigurationID
    logger = logging.getLogger()
    
    root = Element('Components')
    root.set('xmlns:' + 'xsi', 'http://www.w3.org/2001/XMLSchema-instance')
    root.set('xsi:' + 'noNamespaceSchemaLocation', 'CADPostProcessingParameters.xsd')
    root.set('ConfigurationID', gConfigurationID)
    comment = Comment('ComputedMetricsSummary module: v' + gVersion + ' ' + strftime("%Y-%m-%d %H:%M:%S", gmtime()))
    root.append(comment)

    logger.info('ConfigurationID: ' + gConfigurationID)
    logger.info(len(componentList.values()))
    for component in componentList.values():
        logger.info(component.ComponentID + ', ' + component.ElementID)
        componentNode = SubElement(root, 'Component', {'ComponentInstanceID':component.ComponentID,
                                              'FEAElementID':component.ElementID,
                                              })
        materialNode = SubElement(componentNode, 'Material', component.MaterialProperty)
        
        metricsParentNode = SubElement(componentNode, 'Metrics')        
        for key, value in component.MetricsInfo.items():
            if value in component.MetricsOutput:
                metricType = key

                if (key == 'FactorOfSafety'):
                    metricUnits = ''
                else:
                    metricUnits = 'MPa'
                metricNode = SubElement(metricsParentNode, 'Metric', {'ArrayValue':str(component.MetricsOutput[value]),
                                                                      'MetricID':str(value),
                                                                      'Type':str(metricType),
                                                                      'Units':str(metricUnits),
                                                                      'DataFormat':'Scalar'
                                                                      })


    ElementTree(root).write('ComputedValues.xml',
                            encoding='utf-8')
    #xmlFile.write('<?xml version="1.0" encoding="UTF-8"?>' + tostring(root)) 
                                                            
    #ElementTree(root).write(xmlFile,
    #                        xml_declaration=True,encoding='utf-8',
    #                        method="xml",)


def WriteMetric2File(gComponentList):
    print 'WriteMetric2File()'
    global gConfigurationID
    #global gComponentList

    PrintComponentList(gComponentList)
    
    csvFile = open('OutMetrics.csv', 'w')
    xmlFile = open('Out.xml', 'w')

    csvFile.write('ConfigurationID[' + gConfigurationID + ']\n')
    xmlFile.write('<RootObjectType>\n')
    xmlFile.write('<Configurations>\n');
    xmlFile.write('<Configuration ID=\"' + gConfigurationID + '\" Name=\"\">\n');

    for component in gComponentList.values():
        csvFile.write(component.ComponentID + ':' + component.ElementID + '\n')
        
        for key, value in component.MetricsInfo.items():
            if value in component.MetricsOutput:
                csvFile.write(key + '(' + value + ')' + ',' + str(component.MetricsOutput[value]) + ',')
                xmlFile.write('<ConfigMetric DefID=\"' + value + '\" Value=\"' + str(component.MetricsOutput[value]) + '\"/>\n')
            if key in component.QualityOutput:
                csvFile.write(',' + component.QualityOutput[key] + '\n')
    
    xmlFile.write('</Configuration>\n');
    xmlFile.write('</Configurations>\n');
    xmlFile.write('</RootObjectType>\n')

    
    csvFile.close()
    xmlFile.close()


def LogException(exception_msg, logger):
    logger.error(exception_msg)
    raise Exception(exception_msg)


def PrintComponentList(componentList, logger):
    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    logger.info('FUNCTION: PrintComponentList()\n')
    #Write2Log(str(len(componentList))
              
    for component in componentList.values():
        logger.info('Component ID: ' + component.ComponentID)
        logger.info('ElementID: ' + component.ElementID)
        
        logger.info('~~~ Material Properties ~~~')
        for item in component.MaterialProperty:	#type, value
            logger.info(item + ': ' + component.MaterialProperty[item])
            
        logger.info('~~~ Metrics Info ~~~')
        for item in component.MetricsInfo:     	#type, id
            logger.info(item + ': ' + component.MetricsInfo[item])

        logger.info('~~~ Metrics Output ~~~')
        for item in component.MetricsOutput:    #type, id
            logger.info(item + ': ' + str(component.MetricsOutput[item]))

        logger.info('~~~ Metrics Quality ~~~')
        for item in component.QualityOutput:     	#type, id
            logger.info(item + ': ' + component.QualityOutput[item])

        logger.info('~~~ Range ~~~')
        for item in component.RangeInfo:
            logger.info(item + ': ' + component.RangeInfo[item])

        logger.info('~~~ Distorted Out of Range IDs ~~~')
        for item in component.OutOfRangeDistorted:
            logger.info(str(item))

        
    logger.info('\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')
