import sys
import argparse
import shutil
from subprocess import call
import os
from xml.etree.ElementTree import Element, SubElement, ElementTree, Comment
import logging
import csv
import json

gComponentList = dict()
gMaterialList = dict()
gMetricList = dict()

class ComponentData:
    def __init__(self, componentID, elementID):
		self.CadType = ""				#string
		self.CadName = ""	  	
		self.FEAElementType = ""		#string
		self.MaterialID = ""			#string
		self.ComponentID = componentID	#string
		self.ElementID = elementID		#string
		self.FEAResults = dict()
		self.Parts = []
		
class MaterialData:
	def __init__(self, materialID):
		self.MaterialID = materialID 	#string
		self.Properties = dict()
		
class MaterialProperty:
	def __init__(self, propertyID):
		self.PropertyID = propertyID
		self.Source = ""
		self.Units = ""
		self.Value = 0.0
class Metric:
	def __init__(self, metricID):
		self.MetricID = metricID
		self.ComponenInstancetID = ""
		self.MetricType = ""
		self.Value = 0.0

def XMLTreeParse(Node):
	global gComponentList
	global gMaterialList

	for node in Node.findall('Component'):
#		print node.attrib.get('ComponentInstanceID')
		if not node.attrib.get('Type') == "ASSEMBLY":
			cID = node.attrib.get('ComponentInstanceID')
			eID = node.attrib.get('FEAElementID').replace("_",".")
#			print cID, eID
			aComponentData = ComponentData(cID, eID)        # create a new ComponentData
			aComponentData.CadType = node.attrib.get('Type')				
			aComponentData.CadName = node.attrib.get('Name')	  	
			aComponentData.FEAElementType = node.attrib.get('FEAElementType')		
			aComponentData.MaterialID = node.attrib.get('MaterialID')			
			gComponentList[aComponentData.ComponentID] = aComponentData           # append to global list
		else:
			cID = node.attrib.get('ComponentInstanceID')
			eID = "None"
#			print cID, eID
			aComponentData = ComponentData(cID, eID)        # create a new ComponentData
			aComponentData.CadType = node.attrib.get('Type')
			aComponentData.CadName = node.attrib.get('Name')
			
			for part in node.findall(".//*[@Type='PART']"):
#				print "sub: " + part.attrib.get('ComponentInstanceID')
				aComponentData.Parts.append(part.attrib.get('ComponentInstanceID')) 
			gComponentList[aComponentData.ComponentID] = aComponentData           # append to global list
			XMLTreeParse(node)
      
def ParseXMLFile(xmlname):
    logger = logging.getLogger()
    try:
		global gComponentList
		global gMaterialList
			  
		tree = ElementTree()
		tree.parse(xmlname)
		rootNode = tree.getroot()
		logger.info('\n\nFUNCTION: ParseXMLFile()')
		subNode = rootNode.find("./Assemblies/Assembly")
		
		XMLTreeParse(subNode)
		
		
		subNode = rootNode.find("Materials")
		for node in subNode.findall('Material'):
#			print node.attrib.get("MaterialID")
			aMaterial = MaterialData(node.attrib.get("MaterialID"))
			pnode = node.find("MaterialProperties")
			for snode in pnode.iter():
				if not snode.tag == "MaterialProperties":
#					print snode.tag
					aProperty = MaterialProperty(snode.tag)
					aProperty.Source = snode.attrib.get("Source")
					aProperty.Units = snode.attrib.get("Units")
					aProperty.Value = snode.attrib.get("Value")
					aMaterial.Properties[aProperty.PropertyID] = aProperty
			gMaterialList[aMaterial.MaterialID] = aMaterial						# append to global list
    except Exception, inst:
        logger.info('Unexpected error opening ' + xmlname + str(inst))
        return

def ParseMetricFile(metricfilename):
	logger = logging.getLogger()
	try:
		global gMetricList

		tree = ElementTree()
		tree.parse(metricfilename)
		rootNode = tree.getroot()
		logger.info('\n\nFUNCTION: ParseMetricsFile()')
		subNode = rootNode.find("Metrics")
		
		for node in rootNode.findall('Metric'):
				mID = node.attrib.get('MetricID')
				aMetric = Metric(mID)        						# create a new Metric
				aMetric.ComponenInstancetID = node.attrib.get('ComponenInstancetID')
				aMetric.MetricType = node.attrib.get('MetricType')

				gMetricList[aMetric.MetricID] = aMetric           # append to global list
	
	except Exception, inst:
		logger.info('Unexpected error opening ' + metricfilename + str(inst))
	return

def ParseOutFile(outfilename):
	##Format##
	mtype = 0
	lcase = 1
	part  = 2
	value = 3
	##########
	ifile = open(outfilename)
	reader = csv.reader(ifile)
	for row in reader:
		for component in gComponentList:
			if gComponentList[component].ElementID.lower() == row[part].lower() :
				gComponentList[component].FEAResults[row[mtype]] = float(row[value])
		
def UpdateJson(jsonfilename):
	result_json = {}
	logger = logging.getLogger()
	# read current summary report, which contains the metrics
	with open(jsonfilename,'r') as file_in:
		result_json = json.load(file_in)

		if 'Metrics' in result_json:
			for metric in result_json['Metrics']:
				if 'Name' in metric and 'Value' in metric and 'ID' in metric:
					key = metric['GMEID']
	
					if gMetricList.has_key(key):
						# update metric's value to the last value in
						# time series
						metric['Value'] = str(gMetricList[key].Value)
					else:
						# metric was not found in results
						logger.debug("ComputedMetrics.xml key error:" + key)
		else:
			# create warning message
			logger.debug('summary.testresults.json does not contain Metrics')
	

	# update json file with the new values
	with open(jsonfilename,'wb') as file_out:
		json.dump(result_json, file_out, indent=4)
	logger.debug('Finished updating summary.testresults.json file.')
                              



			
LIB_FILE_NAME="patran_pp.pcl"

parser = argparse.ArgumentParser(description='Post process Nastran output w/ Patran')

parser.add_argument('nas_filename', help='.nas File Name')
parser.add_argument('xdb_filename', help='.xdb File Name')
parser.add_argument('MetaDataFile', help='.xml AnalysisMetaData File Name')
parser.add_argument('RequestedMetrics', help='.xml RequestedMetrics File name')
parser.add_argument('ResultsJson', help='.json summary testresults File name')
args = parser.parse_args()

if (not os.path.exists(args.nas_filename)):
	print("Error: %s file not found" % nas_filename)

if (not os.path.exists(args.xdb_filename)):
	print("Error: %s file not found" % xdb_filename)
	
if (not os.path.exists(args.MetaDataFile)):
	print("Error: %s file not found" % MetaDataFile)
	
if (not os.path.exists(args.RequestedMetrics)):
	print("Error: %s file not found" % RequestedMetrics)
	
# Don't stop run if args.ResultsJson cannont be found.  Log this later
#if (not os.path.exists(args.ResultsJson)):
#	print("Error: %s file not found" % ResultsJson)

if (not os.path.exists(os.path.expandvars("%PROE_ISIS_EXTENSIONS%\\bin\\" + LIB_FILE_NAME))):
	print("Error: %s file not found in Meta-Tools installation" % LIB_FILE_NAME)

shutil.copy2(os.path.expandvars("%PROE_ISIS_EXTENSIONS%\\bin\\" + LIB_FILE_NAME), os.getcwd())
	
filename = args.xdb_filename.split(".")[0]
filename = filename.replace("_nas_mod","")
bdf_filename = filename + ".bdf"
shutil.copy2(args.nas_filename, bdf_filename)

f = open(filename + "_PP.ses", "w")
f.write("!!compile " + LIB_FILE_NAME +" into patran_pp.plb\n")
f.write("!!library patran_pp.plb\n")
f.write("STRING dir[262] = \"" + os.getcwd() + "\"\n")
f.write("STRING filename[64] = \"" + filename + "\"\n")
f.write("STRING bdfPath[262] = \"" + bdf_filename + "\"\n")
f.write("STRING xdbPath[262] = \"" + args.xdb_filename + "\"\n")
f.write("Patran_PP(dir, filename, bdfPath, xdbPath)\n")
f.close()

if (os.path.exists(filename + ".db")):
	os.remove(filename + ".db")
	
if (os.path.exists(filename + "_out.txt")):
	os.remove(filename + "_out.txt")

print "\nStarting Patran \n"
retcode = call("patran -b -graphics -sfp " +  filename + "_PP.ses -stdout " + filename + "_PP_log.txt", shell=True)

if(retcode == 0):
	print "Patran Process Successful!!"
	
ParseXMLFile(args.MetaDataFile)
	
if (not os.path.exists(filename + "_out.txt")):
	print("Error: %s file not found" % (filename + "_out.txt"))
	
ParseOutFile(filename + "_out.txt")

ParseMetricFile(args.RequestedMetrics)

########################Compute FOS###########

for component in gComponentList:
	if gComponentList[component].CadType == "PART":
		gComponentList[component].FEAResults["FOS"] = float(gMaterialList[gComponentList[component].MaterialID].Properties["AllowableTensileStress"].Value)  / gComponentList[component].FEAResults["VM"]

##############################################	

###################CSV########################
#print "Part Name,Unique ID,Fatigue Strength,Maximum Stress,Factor of Safety"
#for component in gComponentList:
#	print gComponentList[component].CadName + "," + gComponentList[component].ComponentID + "," + str(gMaterialList[gComponentList[component].MaterialID].Properties["AllowableTensileStress"].Value) + "," + str(gComponentList[component].FEAResults["VM"]) + "," + str(gComponentList[component].FEAResults["FOS"])

with open(filename + '.csv', 'wb') as f:
	writer = csv.writer(f)
	writer.writerow(["CAD Part Name","Unique ID","Allowable Strength","Maximum Stress","Factor of Safety"])
	for component in gComponentList:
		if gComponentList[component].CadType == "PART":
			writer.writerow([gComponentList[component].CadName,gComponentList[component].ComponentID,str(gMaterialList[gComponentList[component].MaterialID].Properties["AllowableTensileStress"].Value),str(gComponentList[component].FEAResults["VM"]),str(gComponentList[component].FEAResults["FOS"])])
##############################################
#######################Populate Assembly Results#######
for component in gComponentList:
		if gComponentList[component].CadType == "ASSEMBLY":
			FOS = []
			VM = []
			for part in gComponentList[component].Parts:
				FOS.append(gComponentList[part].FEAResults["FOS"])
				VM.append(gComponentList[part].FEAResults["VM"])
			gComponentList[component].FEAResults["FOS"] = min(FOS)
			gComponentList[component].FEAResults["VM"] = max(VM)

######################################################
#######################Populate Metrics#######
for aMetric in gMetricList:
	if gMetricList[aMetric].MetricType == "Mises":
		gMetricList[aMetric].Value = gComponentList[gMetricList[aMetric].ComponenInstancetID].FEAResults["VM"]

	if gMetricList[aMetric].MetricType == "FactorOfSafety":
		gMetricList[aMetric].Value = gComponentList[gMetricList[aMetric].ComponenInstancetID].FEAResults["FOS"]

##############################################
#for aMetric in gMetricList:
#	print gMetricList[aMetric].MetricID + "," + gMetricList[aMetric].ComponenInstancetID + "," + gMetricList[aMetric].MetricType + "," + str(gMetricList[aMetric].Value)
	
################Update Results Json##############

if (os.path.exists(args.ResultsJson)):
	UpdateJson(args.ResultsJson)
else:
	print "ERROR: Could not update file: " + args.ResultsJson + ", file does not exists."
#	Should add better error messages, this will cause a _FAILED.txt with little value. print("Error: %s file not found" % ResultsJson)

print "Post Processing Complete, CSV and metrics updated"


