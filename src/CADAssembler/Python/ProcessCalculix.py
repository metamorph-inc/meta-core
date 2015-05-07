import os
import math
import logging
from xml.etree.ElementTree import ElementTree

import sys
import cad_library
import utility_functions
import ComputedMetricsSummary
import UpdateReportJson_CAD

"""
The Calculix FRD results file contains the metric values at each element in the design.

1) Parse RequestedMetrics.xml to determine which metrics are expected.
    1a) Initialize dictionary of { metric : value } entries.
2) Calculate extremum metric values for each requested type:
    a) Maximum Von Mises Stress
    b) Factor of Safety
    c) Maximum Bearing Stress
    d) Maximum Shear Stress
    e) Maximum Displacement
    f) Maximum Temperature
    g) Minimum Temperature

    - If (b), (c), (d), parse AnalysisMetaData.xml to determine material limits.
3) Update results manifest.
4) Copy/Delete Results.
"""

"""
1) Requested metrics: Which metrics, and for which GME exposed components
2) AnalysisMetaData: which p_solid corresponds to the requested c_IDs
    2a) If GME c_ID is assembly, grab all NonCyPhyIDs associated



"""

# Metrc types available to user in GME.
gStructuralMetricTypes = ['FactorOfSafety', 'MaximumDisplacement', 'VonMisesStress', 'Bearing', 'Shear']
gThermalMetricTypes = ['MaximumTemperature', 'MinimumTemperature']

# Major metric headers in FRD file (stress, displacement, etc)
majorMetrics = {'VonMisesStress': 'STRESSR',
                'FactorOfSafety': 'STRESSR',
                'MinimumTemperature': 'NDTEMP',
                'MaximumTemperature': 'NDTEMP',
                'Bearing': 'STRESSR',
                'Shear': 'STRESSR',
                'MaximumDisplacement': 'DISPR'}

# Minor metric headers in FRD file (subset of major metric types)
minorMetrics = {'VonMisesStress': ['SXX', 'SYY', 'SZZ', 'SXY', 'SYZ', 'SZX'],
                'FactorOfSafety': None,
                'MinimumTemperature': 'T',
                'MaximumTemperature': 'T',
                'Bearing': None,
                'Shear': None,
                'MaximumDisplacement': 'ALL'}

# The metric is returned for each PSOLID, do you want min/max value of that set?
metricOps = {'VonMisesStress': max,
             'FactorOfSafety': min,
             'MinimumTemperature': min,
             'MaximumTemperature': max,
             'Bearing': max,
             'Shear': max,
             'MaximumDisplacement': max}


def get_psolids(components, metrics, m, comp):
    """ Each part is assigned an internal PSOLID. Map the instance IDs to their ID
        (or their children's psolid ID).
    """
    if len(components[comp].Children) == 0:
        # PART, has associated PSOLID
        metrics[m]['cid2psolid'][comp] = float(components[comp].ElementID.split('_')[1])
    else:
        # ASM, get children PSOLIDs
        for child in components[comp].Children:
            get_psolids(components, metrics, m, child)


def calculateVonMises(frd, maj_met, min_met, time, psolid, psolidmap):
    """ Calculate the maximum Von Mises stress for a given PSOLID. """
    VMax = None
    for element in psolidmap[psolid]:
        sxx = frd.results[maj_met][time][min_met[0]][element]
        syy = frd.results[maj_met][time][min_met[1]][element]
        szz = frd.results[maj_met][time][min_met[2]][element]
        sxy = frd.results[maj_met][time][min_met[3]][element]
        syz = frd.results[maj_met][time][min_met[4]][element]
        szx = frd.results[maj_met][time][min_met[5]][element]
        p1 = (sxx - syy)**2 + (syy - szz)**2 + (szz - sxx)**2
        p2 = 6*(sxy**2 + syz**2 + szx**2)
        VM = math.sqrt(0.5*(p1 + p2))
        if VM > VMax or VMax is None:
            VMax = VM
    return VMax


def calculateFactorOfSafety(frd, maj_met, time, cid, psolid, psolidmap, components):
    """ Determine the minimum FOS for a given PSOLID. """
    min_met = minorMetrics['VonMisesStress']
    vm = calculateVonMises(frd, maj_met, min_met, time, psolid, psolidmap)  # Get largest stress in psolid
    try:
        fos = components[cid].Allowables.mechanical__strength_tensile / vm
    except ZeroDivisionError:
        fos = 0.0
    return fos


def calculateMaxDisplacement(frd, maj_met, min_met, time, psolid, psolidmap):
    """ Maximum displcement experienced in a PSOLID. """
    maxDisp = None
    for element in psolidmap[psolid]:
        disp = frd.results[maj_met][time][min_met][element]
        if disp > maxDisp or maxDisp is None:
            maxDisp = disp
    return maxDisp


def calculateMaximumTemperature(frd, maj_met, min_met, time, psolid, psolidmap):
    """ Maximum nodal temperature for a given PSOLID. """
    maxTemp = None
    for element in psolidmap[psolid]:
        temp = frd.results[maj_met][time][min_met][element]
        if temp > maxTemp or maxTemp is None:
            maxTemp = temp
    return maxTemp


def calculateMinimumTemperature(frd, maj_met, min_met, time, psolid, psolidmap):
    """ Minimum nodal temperature for a given PSOLID. """
    minTemp = None
    for element in psolidmap[psolid]:
        temp = frd.results[maj_met][time][min_met][element]
        if temp < minTemp or minTemp is None:
            minTemp = temp
    return minTemp


def calculateBearingStress(frd, maj_met, min_met, time, psolid, psolidmap):
    cad_library.exitwitherror("Bearing stress not yet supported.", -1)


def calculateShearStress(frd, maj_met, min_met, time, psolid, psolidmap):
    cad_library.exitwitherror("Shear stress not yet supported.", -1)


calculateFunctions = {'VonMisesStress': calculateVonMises,
                      'FactorOfSafety': calculateFactorOfSafety,
                      'MinimumTemperature': calculateMinimumTemperature,
                      'MaximumTemperature': calculateMaximumTemperature,
                      'Bearing': calculateBearingStress,
                      'Shear': calculateShearStress,
                      'MaximumDisplacement': calculateMaxDisplacement}


def CalculateMetrics(frdfile, components, reqMetricsFile, elementMapFile):
    # Parse FRD results file
    logger = logging.getLogger()
    logger.info('FUNCTION: CalculateMetrics\n')
    frd = cad_library.CalculixResults()
    frd.parse_frd(frdfile + '.frd')

    # Determine which metrics are requested for which GME component instance IDs
    try:
        tree = ElementTree()
        tree.parse(reqMetricsFile)
    except Exception, inst:
        cad_library.exitwitherror('Unexpected error opening ' + reqMetricsFile + str(inst), -1)
    rootNode = tree.getroot()  # Metrics

    metrics = {}
    metricTypes = gStructuralMetricTypes + gThermalMetricTypes
    for metric in rootNode.findall('./Metric'):
        TLA = metric.attrib.get('TopAssemblyComponentInstanceID')
        metricID = metric.attrib.get('MetricID')
        if metric.attrib.get('MetricType') not in metricTypes:
            # Not all requested metrics could be for FEA reporting purposes, just skip.
            continue
        if metricID in metrics:
            metrics[metricID]['cIDs'].append(metric.attrib.get('ComponentInstanceID'))
        else:
            metrics[metricID] = {'cIDs': [metric.attrib.get('ComponentInstanceID')]}
            metrics[metricID]['cid2psolid'] = {}
            metrics[metricID]['type'] = metric.attrib.get('MetricType')

    # Determine which PSOLIDs correspond to which GME component instance IDs
    for m in metrics.keys():
        for comp in components.keys():
            if comp == TLA:
                # TLA would contain all children, double-counting PSOLIDs.
                continue
            if m in components[comp].MetricsOutput:
                get_psolids(components, metrics, m, comp)

    # Report what information has been determined to be collected.
    logger.info('Requesting metric information for the following instance IDs: ')
    for key1 in metrics.keys():
        logger.info('')
        logger.info('METRIC: ' + metrics[key1]['type'])
        for (key2, val2) in metrics[key1]['cid2psolid'].items():
            logger.info('COMPONENT: ' + str(key2) + ' (PSOLID: ' + str(val2) + ')')

    # Grab elements associated with each PSOLID
    psolid2elems = {}
    with open(elementMapFile) as elemmap:
        for line in elemmap:
            info = line.split(', ')
            psolid2elems[int(info[0])] = [float(f) for f in info[1:-1]]

    # Grab desired output values given psolid and metric id
    logger.info('')
    logger.info('PSOLID Metric Results:')
    for metric in metrics.keys():
        met_result = []
        met_header = majorMetrics[metrics[metric]['type']]
        time = max(frd.results[met_header].keys())  # Keys correspond to timestep, grab last for manifest
        logger.info('')
        logger.info('Metric: ' + metrics[metric]['type'])
        for (cid, psolid) in metrics[metric]['cid2psolid'].items():
            # Grab desired data for each psolid, return to list
            maj_met = majorMetrics[metrics[metric]['type']]
            min_met = minorMetrics[metrics[metric]['type']]
            if metrics[metric]['type'] == 'FactorOfSafety':
                met_result.append(calculateFactorOfSafety(frd, maj_met, time, cid, psolid,
                                                          psolid2elems, components))
            else:
                try:
                    met_result.append(calculateFunctions[metrics[metric]['type']](frd, maj_met, min_met, time,
                                                                                  psolid, psolid2elems))
                except KeyError:
                    cad_library.exitwitherror('Unknown metric type encountered: ' + metrics[metric]['type'], -1)
            logger.info('PSOLID ' + str(psolid) + ': ' + str(met_result[-1]))

        # With data for each psolid requested known, take the appropriate min/max of it for result
        for compid in metrics[metric]['cIDs']:
            components[compid].MetricsOutput[metric] = metricOps[metrics[metric]['type']](met_result)


def ParseArgs():
    frd = None
    metaDataFile = None
    reqMetricsFile = None
    resultsJson = None
    elementMap = None
    argList = sys.argv
    argc = len(argList)
    i = 0
    while i < argc:
        if argList[i][:2] == '-o':
            i += 1
            frd = utility_functions.right_trim(argList[i], '.frd')
        elif argList[i][:2] == '-p':
            i += 1
            metaDataFile = os.path.join(os.getcwd(), argList[i])
        elif argList[i][:2] == '-m':
            i += 1
            reqMetricsFile = os.path.join(os.getcwd(), argList[i])
        elif argList[i][:2] == '-j':
            i += 1
            resultsJson = os.path.join(os.getcwd(), argList[i])
        elif argList[i][:2] == '-e':
            i += 1
            elementMap = os.path.join(os.getcwd(), argList[i])
        i += 1

    if not any([frd, metaDataFile, reqMetricsFile, resultsJson, elementMap]):
        exit(1)

    return frd, metaDataFile, reqMetricsFile, resultsJson, elementMap

if __name__ == '__main__':
    if not os.path.isdir('log'):
        os.makedirs('log')
    gLogger = cad_library.setuplogger('CalculixPostProcess.log')
    frdName, metaDataFilePath, reqMetricsFilePath, resultsJsonPath, elementMapPath = ParseArgs()
    componentList = ComputedMetricsSummary.ParseMetaDataFile(metaDataFilePath, frdName, False)
    reqMetrics = ComputedMetricsSummary.ParseReqMetricsFile(reqMetricsFilePath, componentList)
    CalculateMetrics(frdName, componentList, reqMetricsFilePath, elementMapPath)
    computedValuesXml = ComputedMetricsSummary.WriteXMLFile(componentList)
    ComputedMetricsSummary.WriteMetric2File(componentList)
    UpdateReportJson_CAD.update_manifest(resultsJsonPath, computedValuesXml)
    utility_functions.CopyOrDeleteResults(frdName, resultsJsonPath)
