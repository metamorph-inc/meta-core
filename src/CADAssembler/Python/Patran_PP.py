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


def recurselist(component, componentList):
    for comp in componentList.values():
        if component.ComponentID in comp.Children and not comp.IsConfigurationID:
            if len(comp.MetricsInfo.keys()) == 0:
                recurselist(comp, componentList)
            else:
                component.MetricsInfo = comp.MetricsInfo

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


class Patran_PostProcess:
    def __init__ (self, args, patran_path):
        self._args = args
        self._logger = logging.getLogger('root.Patran_PostProcess')
        filename = args.xdb_filename.split(".")[0]
        self._filename = filename.replace("_nas_mod","")
        self._lib_file_name = 'patran_pp.pcl'
        self._bdf_file_name = filename + ".bdf"
        shutil.copy2(args.nas_filename, self._bdf_file_name)
        self._xdb_file_name = args.xdb_filename
        self._patran_path = patran_path

    def pre_process_cleanup (self):
        if (os.path.exists(self._filename + ".db")):
            os.remove(self._filename + ".db")
        if (os.path.exists(self._filename + "_out.txt")):
            os.remove(self._filename + "_out.txt")

    def create_session_file (self):
        logger.debug(self._filename)
        f = open(self._filename + "_PP.ses", "w")
        f.write("!!compile " + self._lib_file_name +" into patran_pp.plb\n")
        f.write("!!library patran_pp.plb\n")
        f.write("STRING patranDir[512] = \"" + PATRAN_PATH + "\"\n")
        f.write("STRING dir[262] = \"" + os.getcwd() + "\"\n")
        f.write("STRING filename[64] = \"" + self._filename + "\"\n")
        f.write("STRING bdfPath[262] = \"" + self._bdf_file_name + "\"\n")
        f.write("STRING xdbPath[262] = \"" + self._xdb_file_name + "\"\n")
        f.write("Patran_PP(patranDir, dir, filename, bdfPath, xdbPath)\n")
        f.close()
        
    def run_patran (self):
        self.create_session_file()
        self.pre_process_cleanup()
        retcode = call("patran -b -graphics -sfp " +  self._filename + "_PP.ses -stdout " + self._filename + "_PP_log.txt", shell=True)

        if(retcode == 0):
            logger.info ("Patran Process Successful!!")
        else:
            raise Exception("Patran Process Failed!")

    def update_results_files (self):
        gComponentList = ComputedMetricsSummary.ParseMetaDataFile(args.MetaDataFile, None, None)

        if (not os.path.exists(self._filename + "_out.txt")):
            logger.error ("%s file not found" % (filename + "_out.txt"))

        ParseOutFile(self._filename + "_out.txt", gComponentList)
        reqMetrics = ComputedMetricsSummary.ParseReqMetricsFile(args.RequestedMetrics, gComponentList)
        
        for component in gComponentList.values():
            recurselist(component, gComponentList)

        for component in gComponentList.values():
            for comp in gComponentList.values():
                if component.ComponentID in comp.Children and not comp.IsConfigurationID:
                    # component is actually a child, so parent's metric data
                    # should be updated - provided that child metrics are larger
                    logger.debug(comp)
                    if 'FactorOfSafety' in component.MetricsInfo:
                        component.MetricsInfo['FactorOfSafety'] = comp.MetricsInfo['FactorOfSafety']
                    if 'VonMisesStress' in component.MetricsInfo:
                        component.MetricsInfo['VonMisesStress'] = comp.MetricsInfo['VonMisesStress']
                    break
                
            if component.CadType == "PART":
                fos = float(component.Allowables.mechanical__strength_tensile) / component.FEAResults["VM"]
                #fos = float(component.MaterialProperty['Mises'])  / component.FEAResults["VM"]
                component.FEAResults['FOS'] = fos
                if 'FactorOfSafety' in component.MetricsInfo:
                    component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = fos
                if 'VonMisesStress' in component.MetricsInfo:
                    component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = component.FEAResults["VM"]
        
        ################  CSV  ###############################
        with open(self._filename + '.csv', 'wb') as f:
            writer = csv.writer(f)
            writer.writerow(["Unique ID","Allowable Stress","Maximum Stress","Factor of Safety"])
            for component in gComponentList.values():
                if component.CadType == "PART":
                    writer.writerow([component.ComponentID,
                                     str(component.Allowables.mechanical__strength_tensile), \
                                     str(component.FEAResults["VM"]),str(component.FEAResults["FOS"])])
                    
        ################  Populate Assembly Results  #########
        for component in gComponentList.values():
            logger.info ('ComponentID: %s' %component.ComponentID)
            logger.info (component)
            logger.info ('')
            if component.CadType == "ASSEMBLY" and not component.IsConfigurationID:
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
            logger.error("Could not update file: " + args.ResultsJson + ", file does not exist.")

        logger.info("Post Processing Complete, CSV and metrics updated")


if __name__ == '__main__':

    try:
        logger = logging.getLogger('root.Patran_PostProcess')
        handler = logging.FileHandler('PostProcess_Log.txt', 'w')
        formatter = logging.Formatter(
            '%(asctime)s %(name)-12s %(levelname)-8s %(message)s')
        handler.setFormatter(formatter)
        logger.addHandler(handler)
        logger.setLevel(logging.DEBUG)

        parser = argparse.ArgumentParser(description='Post process Nastran output w/ Patran')
        parser.add_argument('nas_filename', help='.nas File Name')
        parser.add_argument('xdb_filename', help='.xdb File Name')
        parser.add_argument('MetaDataFile', help='.xml AnalysisMetaData File Name')
        parser.add_argument('RequestedMetrics', help='.xml RequestedMetrics File name')
        parser.add_argument('ResultsJson', help='.json summary testresults File name')
        args = parser.parse_args()

        if (not os.path.exists(args.nas_filename)):
            logger.error ("%s file not found" % nas_filename)

        if (not os.path.exists(args.xdb_filename)):
            logger.error ("%s file not found" % xdb_filename)

        if (not os.path.exists(args.MetaDataFile)):
            logger.error ("%s file not found" % MetaDataFile)

        if (not os.path.exists(args.RequestedMetrics)):
            logger.error ("%s file not found" % RequestedMetrics)

        with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
            META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]
        ppDir = os.path.join(META_PATH, 'bin', 'CAD')
        with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,  r'Software\Wow6432Node\MSC.Software Corporation\Patran x64\Latest', \
                             0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
            LATES_PATRAN_VERSION = _winreg.QueryValueEx(key, '')[0]	
        with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,  r'Software\Wow6432Node\MSC.Software Corporation\Patran x64\\' + LATES_PATRAN_VERSION, \
                             0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
            PATRAN_PATH = _winreg.QueryValueEx(key, 'Path')[0]

        LIB_FILE_NAME = 'patran_pp.pcl'
        if (not os.path.exists(os.path.join(ppDir, LIB_FILE_NAME))):
            logger.error ("%s file not found in Meta-Tools installation" % LIB_FILE_NAME)
            exit(1)

        shutil.copy2(os.path.join(ppDir, LIB_FILE_NAME), os.getcwd())
        
        post_process = Patran_PostProcess(args, PATRAN_PATH)
        post_process.run_patran()
        post_process.update_results_files()

    except: # catch *all* exceptions
        import traceback
        e = sys.exc_info()[0]
        var = traceback.format_exc()
        logger.error ("Exception: %s" %var)