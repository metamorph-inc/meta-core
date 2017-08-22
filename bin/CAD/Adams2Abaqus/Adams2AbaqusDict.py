"""

Adams2AbaqusDict.py, version 1.1.0

For use with Abaqus 6.13-1 (Python 2.6.2).

Created by Ozgur Yapar <oyapar@isis.vanderbilt.edu>

    - Includes modules which create the dictionaries storing
      the essential data.

"""

import os, re
from numpy import array, cross, transpose, vstack, dot
import numpy.linalg as LA
import string as STR
import xml.etree.ElementTree as ET
import _winreg, sys, ctypes, uuid, traceback


MAIN = os.getcwd()                                                           # initial working directory
LOGDIR = os.path.join(MAIN, "log", "CyPhy2AbaqusCmd.log")                        # path to log file

# unit conversion dictionary uses mmNs, more convenient than mmKs
def generateConvDict(unitScale):
    conv = {}                                                           # unit conversion dictionary
    conv.update([['GPa',(10**9)*(unitScale**2)],                        # to N/length^2
                 ['MPa',(10**6)*(unitScale**2)],                        # to N/length^2
                 ['kPa',(10**3)*(unitScale**2)],                        # to N/length^2
                 ['pascal',unitScale**2],                               # to N/length^2
                 ['Pa',unitScale**2],                                   # to N/length^2
                 ['g/cm^3',(10**6)*(unitScale**3)],                     # to (N-s^2/length)/length^3
                 ['kg/m^3',(10**-3)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['kg/mm^3',(10**6)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['newton',1],                                          # to N
                 ['kN',10**3],                                          # to N
                 ['newton',1],                                          # to N
                 ['newton-mm',(10**-3)*(unitScale**-1)],                # to N-length
                 ['m/s2',1],                                            # to length/s^2
                 ['mm/s2',(10**-3)/unitScale],                          # to length/s^2
                 ['N/A',1],                                             # dimensionless
                 ['1/C',1],                                             # to 1/C
                 ['C',1],                                               # to Celsius
                 ['W/(m-K)',1],                                         # to (N-length/s)/(K-length)
                 ['J/(K-kg)',unitScale**-2]])                           # to (N-length)/(K-(N-s^2/length))

    return conv


	
# returns the material library data
def generateInstRef(metricComponentsXML, xmlMetricsName, xmlName):
    f_p = open(LOGDIR, "w")             # create a log file
    
    try:
        instRef = {}
        instIndex = {}
        
        for component in metricComponentsXML.findall('MetricComponent'):
            if component.get('Type') == 'PART':
                instName = STR.upper(component.get('Name'))
                metricID = component.get('MetricID')
                mtrl = component.find('Material')
                materialIDRead = mtrl.get('Type')
                materialID = materialIDRead.lower()
                
                isPart = True
                for event, elem in ET.iterparse(xmlMetricsName, events=('start', 'end')):
                    if event == 'start' and elem.tag == 'CADComponent':
                        if metricID == elem.get('MetricID'):
                            componentInstanceID = elem.get('ComponentInstanceID')

                instRef.update([[instName,{}]])
                instRef[instName].update([['isPart',isPart]])
                instRef[instName].update([['ComponentID',componentInstanceID]])
                try:
                    instRef[instName].update([['MaterialID',materialID]])
                except:
                    pass
                instIndex.update([[componentInstanceID,instName]])                            
            elif component.get('Type') == 'ASSEMBLY':
                pass

                
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error in creating necessary data arrays\n')
        f_p.write('Possible causes of error are:\n')
        f_p.write('1- Unknown component type definiton (not Part or Assembly) in CADAssembly_metrics.xml file, \n')
        f_p.write('2- Missing information in the CADAssembly_metrics.xml file Part definition(i.e. Name, MetricID, Material, Type, ComponentInstanceID\n')
        f_p.write('3- Missing information in the CADAssembly.xml file Part definition(i.e. ComponentID, MaterialID, ManufacturingParameter, Scalars, Mass, Volume\n')
        f_p.write('4- Missing information in the CADAssembly_metrics.xml file Assembly definition(i.e. Name, MetricID, ComponentInstanceID\n')
        raise

    f_p.close()

    return (instRef, instIndex)

def generateMtrlRef(LIBRARY_MANAGER, metricComponentsXML, unitScale):
    f_p = open(LOGDIR, "w")             # create a log file

    mtrlRef = {}                                                                                                                    # set to contain a dictionary for each material
    f_p.write('\n')
    f_p.write("**********************************************************************************" + '\n')
    f_p.write('\n')
    f_p.write("Grabbing properties of each material from the material library" + '\n')
    f_p.write('Material properties that will be grabbed are density, modulus of elasticity, poisson\'s ratio,\n')
    f_p.write('ultimate tensile strength, fatigue strength, yield strength\n')
    f_p.write('\n')

    mtrlProps = ["density","mechanical__modulus_elastic","mechanical__ratio_poissons",
        "mechanical__strength_tensile_ultimate","mechanical__strength_fatigue","mechanical__strength_tensile_yield"]                # set of names of material properties to get

    conv = generateConvDict(unitScale)
   
    try:
        for component in metricComponentsXML.findall('MetricComponent'):
            try:
                instName = STR.upper(component.get('Name'))
            except:
                f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
                f_p.write('ERROR: Missing part name in CADAssembly_metrics.xml.\n')
                raise               

            if component.get("Type") == "PART":
                try:
                    mtrl = component.find('Material')
                except:
                    f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
                    f_p.write('ERROR: Missing material for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                    raise

                try:
                    mtrlName = mtrl.get('Type').lower()
                except:
                    f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
                    f_p.write('ERROR: Missing material type for part ' + str(instName) + ' in CADAssembly_metrics.xml.\n')
                    raise  

                mtrlRef.update([[mtrlName,{}]])
                    
                try:
                    mtrlData = LIBRARY_MANAGER.materialData(mtrlName)               # get mtrl data from the material library for the corresponding material
                except:
                    f_p.write('\n')
                    f_p.write("ERROR: " + mtrlName + \
                        " is not in the material library." + '\n')
                    raise ValueError("ERROR: " + mtrlName + \
                        " is not in the material library." + '\n')
                for prop in mtrlProps:
                    if mtrlData[prop]["value"]=="None":
                        f_p.write('\n')
                        f_p.write("ERROR: Property " + prop + " of " + mtrlName + \
                            " has not been defined in the material library." + '\n')
                        raise ValueError("Property " + prop + " of " + mtrlName + \
                            " has not been defined in the material library.")
                    if "unit" in mtrlData[prop]:                                            # if property has units
                        units = conv[mtrlData[prop]["unit"]]                                # get unit scaling factor
                    else:                                                                   # if no units (e.g. Poisson)
                        units = 1.0                                                         # unit scaling factor is 1
                    mtrlRef[mtrlName].update([[prop,
                        float(mtrlData[prop]["value"])*units]])

        f_p.write('Grabbing material properties of ' + str(mtrlName) + ' for the part ' + str(instName) + '\n')

                    
    except Exception as e:
        f_p.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f_p.write('ERROR: Error in grabbing and defining material names and properties\n')
        raise

    f_p.close()

    return (mtrlRef)

def generateMetricRef(metricSetXML, instIndex):
    f_p = open(LOGDIR, "w")             # create a log file

    metricRef = {}
    try:
        for metric in metricSetXML:
            if metric.get('ComponentID') in instIndex:
                if not metric.get('MetricType') in metricRef:
                    metricRef.update([[metric.get('MetricType'),{}]])
                metricRef[metric.get('MetricType')].update([[
                    instIndex[metric.get('ComponentID')],None]])

    except:
        f_p.write('WARNING: In the FEA testbench there are no individual parts, just the assembly. Otherwise there might be an error while\n')
        f_p.write('         MetricType, ComponentID or MetricID values are being grabbed from CADAssembly.xml\n')
    
    f_p.close()

    return (metricRef)

def generateSuperRef(instRef, mtrlRef, metricRef, instIndex):
    f_p = open(LOGDIR, "w")             # create a log file

    try:
        superRef = {}
        superRef.update([['instRef',instRef],
                         ['mtrlRef',mtrlRef],
                         ['metricRef',metricRef],
                         ['instIndex',instIndex]])
    except:
        f.write(STR.join(traceback.format_exception(*sys.exc_info())))
        f.write('ERROR: Cannot define the super dictionary, probably because of input discrepancies\n')
        raise    
    f_p.close()

    return (superRef)


