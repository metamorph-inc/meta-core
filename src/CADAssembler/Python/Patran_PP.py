import sys
import argparse
import shutil
from subprocess import call
import os
from xml.etree.ElementTree import Element, SubElement, ElementTree, Comment
import ComputedMetricsSummary
import UpdateReportJson_CAD
import logging
import csv
import _winreg


def ParseOutFile(outfilename, gComponentList):
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
            # MetaData : PSOLID_3 || PatranOutput : PSOLID.3
            eID = '.'.join(gComponentList[component].ElementID.rsplit('_',1)).lower()
            if eID == row[part].lower():
                gComponentList[component].FEAResults[row[mtype]] = float(row[value])


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

with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
        META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]
        ppDir = os.path.join(META_PATH, 'bin', 'CAD')
with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,  r'Software\Wow6432Node\MSC.Software Corporation\Patran x64\20.0.0', \
                     0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
        PATRAN_PATH = _winreg.QueryValueEx(key, 'Path')[0]

if (not os.path.exists(os.path.join(ppDir, LIB_FILE_NAME))):
    print("Error: %s file not found in Meta-Tools installation" % LIB_FILE_NAME)

shutil.copy2(os.path.join(ppDir, LIB_FILE_NAME), os.getcwd())

filename = args.xdb_filename.split(".")[0]
filename = filename.replace("_nas_mod","")
bdf_filename = filename + ".bdf"
shutil.copy2(args.nas_filename, bdf_filename)

f = open(filename + "_PP.ses", "w")
f.write("!!compile " + LIB_FILE_NAME +" into patran_pp.plb\n")
f.write("!!library patran_pp.plb\n")
f.write("STRING patranDir[512] = \"" + PATRAN_PATH + "\"\n")
f.write("STRING dir[262] = \"" + os.getcwd() + "\"\n")
f.write("STRING filename[64] = \"" + filename + "\"\n")
f.write("STRING bdfPath[262] = \"" + bdf_filename + "\"\n")
f.write("STRING xdbPath[262] = \"" + args.xdb_filename + "\"\n")
f.write("Patran_PP(patranDir, dir, filename, bdfPath, xdbPath)\n")
f.close()

if (os.path.exists(filename + ".db")):
    os.remove(filename + ".db")
if (os.path.exists(filename + "_out.txt")):
    os.remove(filename + "_out.txt")

print "\nStarting Patran \n"
retcode = call("patran -b -graphics -sfp " +  filename + "_PP.ses -stdout " + filename + "_PP_log.txt", shell=True)

if(retcode == 0):
    print "Patran Process Successful!!"
else:
    raise Exception("Patran Process Failed!")

gComponentList = ComputedMetricsSummary.ParseMetaDataFile(args.MetaDataFile, None, None)

if (not os.path.exists(filename + "_out.txt")):
    print("Error: %s file not found" % (filename + "_out.txt"))

ParseOutFile(filename + "_out.txt", gComponentList)

reqMetrics = ComputedMetricsSummary.ParseReqMetricsFile(args.RequestedMetrics, gComponentList)


################  Compute FOS  #######################
for component in gComponentList.values():
    if component.CadType == "PART":
        fos = float(component.Allowables.mechanical__strength_tensile) / component.FEAResults["VM"]
        #fos = float(component.MaterialProperty['Mises'])  / component.FEAResults["VM"]
        component.FEAResults['FOS'] = fos
        component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = fos
        component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = component.FEAResults["VM"]

################  CSV  ###############################
with open(filename + '.csv', 'wb') as f:
    writer = csv.writer(f)
    writer.writerow(["Unique ID","Allowable Stress","Maximum Stress","Factor of Safety"])
    for component in gComponentList.values():
        if component.CadType == "PART":
            writer.writerow([component.ComponentID,
                             str(component.Allowables.mechanical__strength_tensile), \
                             str(component.FEAResults["VM"]),str(component.FEAResults["FOS"])])


################  Populate Assembly Results  #########
for component in gComponentList.values():
    if component.CadType == "ASSEMBLY":
        FOS = []
        VM = []
        for part in component.Children:
            FOS.append(gComponentList[part].FEAResults["FOS"])
            VM.append(gComponentList[part].FEAResults["VM"])
        component.FEAResults["FOS"] = min(FOS)
        component.FEAResults["VM"] = max(VM)
        component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = min(FOS)
        component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = max(VM)


################  Populate Metrics  #################
computedValuesXml = ComputedMetricsSummary.WriteXMLFile(gComponentList)


################  Update Results Json  ##############
if (os.path.exists(args.ResultsJson)):
    UpdateReportJson_CAD.update_manifest(args.ResultsJson, computedValuesXml)
else:
    print "ERROR: Could not update file: " + args.ResultsJson + ", file does not exist."

print "Post Processing Complete, CSV and metrics updated"
sys.exit(0)


