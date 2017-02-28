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
import json
from lxml import etree as letree
import _winreg

# 10/12/2016 R.O. WARNING - This file was updated to support:
#    FEA_Total_Volume
#    FEA_Total_Mass
# in testbench_manifest.json, but only for the update_results_files_xdb_op2 function in this file.
# This case is for Patran composite runs.  This program should be updated to support FEA_Total_Volume/ FEA_Total_Mass
# for all cases ( e.g. solid model, non-composite plate model).
#
# 2/28/2017 R.O. - Added support for FEA_Total_Volume/ FEA_Total_Mass in update_results_files_xdb_op2 for
# isotropic Patran_Nastran models. Previously, FEA_Total_Volume/ FEA_Total_Mass was only supported for composite models.
#

def recurse_list(component, component_list):
    for comp in component_list.values():
        if component.ComponentID in comp.Children and not comp.IsConfigurationID:
            if len(comp.MetricsInfo.keys()) == 0:
                recurse_list(comp, component_list)
            else:
                component.MetricsInfo = comp.MetricsInfo


def parse_out_file(out_filename, g_component_list, logger_instance):

    logger_instance.info('BEGIN ParseOutFile()')

    ##Format##
    mtype = 0
    lcase = 1
    part  = 2
    value = 3

    ##########
    ifile = open(out_filename)
    reader = csv.reader(ifile)
    for row in reader:
        logger_instance.info(row[mtype] + " " + row[lcase] + " " + row[part] + " " + row[value])
        for component in g_component_list:
            # MetaData : PSOLID_3 || PatranOutput : PSOLID.3
            eID = '.'.join(g_component_list[component].ElementID.rsplit('_',1)).lower()
            if eID == row[part].lower():
                g_component_list[component].FEAResults[row[mtype]] = float(row[value])

    logger_instance.info("  ")
    for component in g_component_list:
        logger_instance.info(g_component_list[component].ElementID)
    logger_instance.info('END ParseOutFile()')


def parse_out_file_v2(out_filename, g_component_list, logger_instance):
    """
    Copy Paste from above; check guid instead of ElementID
    args are Nastran_mod_out_v2, g_component_list, logger_instance
    """

    logger_instance.info('BEGIN parse_out_file_v2()')

    ##Format##
    mtype = 0
    guid = 1
    value = 2

    ##########
    ifile = open(out_filename)
    reader = csv.reader(ifile)
    for row in reader:
        logger_instance.info("{} {} {}".format(row[mtype], row[guid], row[value]))
        for component in g_component_list:
            if g_component_list[component].ComponentID == row[guid]:
                g_component_list[component].FEAResults[row[mtype]] = float(row[value])

    logger_instance.info("  ")
    for component in g_component_list:
        logger_instance.info(g_component_list[component].ElementID)
    logger_instance.info('END ParseOutFile()')


def get_logger(logger_name):

    if not os.path.exists('log'):
        os.mkdir('log')

    logger_instance = logging.getLogger(logger_name)
    logger_instance.setLevel(logging.DEBUG)

    log_file_path = os.path.join('log', 'PostProcess_Log.txt')

    # create logging file handler
    fh = logging.FileHandler(log_file_path)
    fh.setLevel(logging.DEBUG)

    # create console handler with a higher log level
    ch = logging.StreamHandler()
    ch.setLevel(logging.WARNING)

    # create formatter and add it to the handlers
    formatter = logging.Formatter('%(levelname)s - %(message)s')
    fh.setFormatter(formatter)
    ch.setFormatter(formatter)

    logger_instance.addHandler(fh)
    logger_instance.addHandler(ch)

    return logger_instance


class PatranPostProcess:

    def __init__(self, meta_data_file, requested_metrics, results_json):

        self.logger = logging.getLogger('Patran_PostProcess')

        # with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
        #     meta_path = _winreg.QueryValueEx(key, 'META_PATH')[0]

        self.meta_data_file = meta_data_file
        self.requested_metrics = requested_metrics
        self.results_json = results_json

        self.meta_bin_cad_dir = os.path.split(os.path.abspath(__file__))[0]
        self.logger.info('META/bin/CAD: {}'.format(self.meta_bin_cad_dir))

        self.analysis_dir = os.path.abspath(os.getcwd())  # this should be abcdef/Analysis/(PATRAN_NASTRAN|NASTRAN|???)
        self.logger.info("Calling {} in {}".format(__file__, self.analysis_dir))

        self.filename = "Nastran_mod"

    def write_failed_and_exit(self, msg=None):

        with open('_FAILED.txt', 'a') as f_out:
            if msg is not None:
                f_out.write('ERROR: {}'.format(msg))
            f_out.write("{} has failed; see log/PostProcess_Log.txt for details.".format(__file__))

        sys.exit(1)

    def create_session_file(self):
        """This method added 8/11/16 by James Klingler. Uses the refactored PCL Post-Processing scripts from
        https://github.com/metamorph-inc/meta-blademda/commit/c07cfb1977f76d2b7367aa4daffc893e92eef269"""

        ppp_dir_name = 'PostProcessing\PatranPostProcessing'
        pp_ses_name = 'PatranPostProcessing.ses'

        self.logger.info("Creating {}...".format(pp_ses_name))

        patran_post_processing_dir = os.path.join(self.meta_bin_cad_dir, ppp_dir_name)  # PP PCL dir
        session_file_template = os.path.join(patran_post_processing_dir, pp_ses_name)

        # substitute the path to the PP PCL dir
        self.logger.info('Replacing zz_POST_PROCESSING_SOURCE_DIR_zz with {}'.format(patran_post_processing_dir))

        with open(session_file_template, 'r') as f:
            ses_template_txt = f.read()
            replace_pcl_dir = ses_template_txt.replace("zz_POST_PROCESSING_SOURCE_DIR_zz", patran_post_processing_dir)
            replace_model_name = replace_pcl_dir.replace("zz_Model_Name_zz", self.filename)

        # write the session file to 'this' PATRAN_NASTRAN directory
        with open(pp_ses_name, 'w') as f:
            f.write(replace_model_name)

    def run_patran_pp(self):

        self.logger.info("Enter run_patran_pp()")

        status = True

        self.create_session_file()

        # create the PLB directory for the compile Post-Processing files
        pp_plb_dir = os.path.join(self.analysis_dir, 'PatranPostProcess_PLB')
        if not os.path.exists(pp_plb_dir):
            os.makedirs(pp_plb_dir)

        patran_call = "patran -b -graphics -sfp  PatranPostProcessing.ses -stdout .\log\PatranPostProcessing_Session.log"

        # check if Nastra_mod.bdf exists (Patran_Nastran)
        analysis_dir_name = os.path.split(self.analysis_dir)[-1]
        bdf_name = 'Nastran_mod.bdf'

        if analysis_dir_name == "Nastran":
            if not os.path.exists(bdf_name):
                nas_file_path = os.path.join('..', 'Nastran_mod.nas')
                if os.path.exists(nas_file_path):
                    shutil.copy2(nas_file_path, os.path.join(self.analysis_dir, bdf_name))

        self.logger.info("Calling {}".format(patran_call))
        retcode = call(patran_call)

        if retcode == 0:
            self.logger.info("Patran Process Successful!!")
        else:
            status = False
            self.logger.error("Patran Process Failed; error code: {}".format(retcode))

        return status

    def update_results_files_xdb(self, results_txt_file):

        self.logger.info("Calling update_results_files_xdb()")

        status = True
        g_component_list = ComputedMetricsSummary.ParseMetaDataFile(self.meta_data_file, None, None)

        parse_out_file(results_txt_file, g_component_list, self.logger)

        req_metrics = ComputedMetricsSummary.ParseReqMetricsFile(self.requested_metrics, g_component_list)
        
        for component in g_component_list.values():
            recurse_list(component, g_component_list)

        for component in g_component_list.values():
            for comp in g_component_list.values():
                if component.ComponentID in comp.Children and not comp.IsConfigurationID:
                    # component is actually a child, so parent's metric data
                    # should be updated - provided that child metrics are larger
                    self.logger.debug(comp)
                    if 'FactorOfSafety' in component.MetricsInfo:
                        component.MetricsInfo['FactorOfSafety'] = comp.MetricsInfo['FactorOfSafety']
                    if 'VonMisesStress' in component.MetricsInfo:
                        component.MetricsInfo['VonMisesStress'] = comp.MetricsInfo['VonMisesStress']
                    break
                
            if component.CadType == "PART":
                self.logger.info(str(len(component.FEAResults)))
                for name in component.FEAResults.keys():
                    self.logger.info("the key name is " + name)
                fos = float(component.Allowables.mechanical__strength_tensile) / component.FEAResults["VM"]
                #fos = float(component.MaterialProperty['Mises'])  / component.FEAResults["VM"]
                component.FEAResults['FOS'] = fos
                if 'FactorOfSafety' in component.MetricsInfo:
                    component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = fos
                if 'VonMisesStress' in component.MetricsInfo:
                    component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = component.FEAResults["VM"]
        
        ################  CSV  ###############################
        with open(self.filename + '.csv', 'wb') as f:
            writer = csv.writer(f)
            writer.writerow(["Unique ID", "Allowable Stress", "Maximum Stress", "Factor of Safety"])
            for component in g_component_list.values():
                if component.CadType == "PART":
                    writer.writerow([component.ComponentID,
                                     str(component.Allowables.mechanical__strength_tensile), 
                                     str(component.FEAResults["VM"]), 
                                     str(component.FEAResults["FOS"])])
                    
        ################  Populate Assembly Results  #########
        for component in g_component_list.values():
            self.logger.info('ComponentID: %s' % component.ComponentID)
            self.logger.info(component)
            self.logger.info('')
            if component.CadType == "ASSEMBLY" and not component.IsConfigurationID:
                FOS = []
                VM = []
                for part in component.Children:
                    FOS.append(g_component_list[part].FEAResults["FOS"])
                    VM.append(g_component_list[part].FEAResults["VM"])
                component.FEAResults["FOS"] = min(FOS)
                component.FEAResults["VM"] = max(VM)
                component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = min(FOS)
                component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = max(VM)


        ################  Populate Metrics  #################
        computed_values_xml = ComputedMetricsSummary.WriteXMLFile(g_component_list)
        
        ################  Update Results Json  ##############
        if os.path.exists(self.results_json):
            UpdateReportJson_CAD.update_manifest(self.results_json, computed_values_xml)
        else:
            self.logger.error("Could not update file: " + self.results_json + ", file does not exist.")
            status = False

            self.logger.info("Post Processing Complete, CSV and metrics updated")

        return status

    def update_results_files_xdb_op2(self, result_txt_file):

        self.logger.info("Enter update_results_files_op2()")

        check_failure_criterion = False

        # Open Nastran_mod_out_v2.txt and read line by line
        self.logger.info("Working with {}".format(result_txt_file))

        comp_id_to_value = {}

        globalMetrics = {}

        with open('Nastran_mod_out_v2.txt', 'r') as tb_file:

            # TODO: Need to see if it has Failure_Criteria, or VM or ???
            # TODO: and handle all those cases

            for line in tb_file.readlines():
                # line example: "FailureIndex,d44a4d66-1560-4634-9d20-c83c063be51d,1.0381887E-005"
                split_line = line.strip('\n').split(',')
                # Note if split_line[1] == this means that it is a global field (e.g. TotalVolume/TotalMass), which
                # does not have a comp_id_to_value
                if ( len(split_line[1].strip()) !=  0 ):
                    comp_id_to_value[split_line[1]] = split_line[2]
                    # The first entry will always be 'FailureIndex' ...?
                    if split_line[0] == 'FailureIndex':
                        check_failure_criterion = True
                else:
                    # The following was added to support metrics such as TotalVolume/TotalMass
                    globalMetrics[split_line[0].lower()] = split_line[2]

        if check_failure_criterion:  # this is Failure_Criteria territory; all new code.

            # Open CADAssembly.xml (two directories up), and get ComponentID/MetricID map
            self.logger.info("Getting Component/Metric IDs from CADAssembly.xml")

            metric_id_to_comp_id = {}

            cad_assembly_path = os.path.join('..', '..', 'CADAssembly.xml')

            if os.path.exists(cad_assembly_path):
                cad_assm_tree = letree.parse(cad_assembly_path)
                cad_assm_root = cad_assm_tree.getroot()

                metric_path = "Assembly/Analyses/FEA/Metrics/Metric"
                self.logger.info("Searching for {} in update_results_files_op2()".format(metric_path))
                metrics = cad_assm_root.findall(metric_path)

                for metric in metrics:
                    metric_id = metric.attrib.get('MetricID')
                    comp_id = metric.attrib.get('ComponentID')
                    metric_id_to_comp_id[metric_id] = comp_id

            # Open TB manifest.json and put the value from component_id_to_value_map into the appropriate Metric
            self.logger.info("Updating Metrics in testbench_manifest.json")

            tb_metrics = None
            tb_manifest_json = None

            with open(self.results_json, 'r') as tb_file:
                tb_manifest_json = json.load(tb_file)


            tb_metrics = tb_manifest_json.get('Metrics')

            for metric in tb_metrics:
                metric_id = metric['ID']
                metrid_name =  metric['Name']
                print metrid_name
                print globalMetrics
                if ( metrid_name.lower() == "fea_total_volume" or metrid_name.lower() == "fea_total_mass" ):
                    if (  metrid_name.lower() == "fea_total_volume" ):
                        metric_value = globalMetrics.get("TotalVolume".lower())
                    else:
                        metric_value = globalMetrics.get("TotalMass".lower())
                    if metric_value is None:
                        msg = "{} did not contain a Value for metric name for FEA_Total_Volume/FEA_Total_Mass {}.".format()
                        self.write_failed_and_exit()
                else:
                    associated_comp_id = metric_id_to_comp_id.get(metric_id)
                    metric_value = comp_id_to_value.get(associated_comp_id)
                    if metric_value is None:
                        msg = "{} did not contain a Value for Component ID {}.".format()
                        self.write_failed_and_exit()

                metric['Value'] = metric_value

            tb_manifest_json['Metrics'] = tb_metrics

            with open(self.results_json, 'w') as tb_file:
                json.dump(tb_manifest_json, tb_file, indent=4)

            return True

        else:  # this is essentially the old method, except that the Nastran_mod_out_v2 allows us to skip a step.

            # TODO This is hacky, but I don't have the time to do it properly. JK 8/19/2016

            g_component_list = ComputedMetricsSummary.ParseMetaDataFile(self.meta_data_file, None, None)
            parse_out_file_v2(result_txt_file, g_component_list, self.logger)  # new method, skipping a step

            req_metrics = ComputedMetricsSummary.ParseReqMetricsFile(self.requested_metrics, g_component_list)

            for component in g_component_list.values():
                recurse_list(component, g_component_list)

            for component in g_component_list.values():
                for comp in g_component_list.values():
                    if component.ComponentID in comp.Children and not comp.IsConfigurationID:
                        # component is actually a child, so parent's metric data
                        # should be updated - provided that child metrics are larger
                        self.logger.debug(comp)
                        if 'FactorOfSafety' in component.MetricsInfo:
                            component.MetricsInfo['FactorOfSafety'] = comp.MetricsInfo['FactorOfSafety']
                        if 'VonMisesStress' in component.MetricsInfo:
                            component.MetricsInfo['VonMisesStress'] = comp.MetricsInfo['VonMisesStress']
                        break

                if component.CadType == "PART":
                    self.logger.info(str(len(component.FEAResults)))
                    for name in component.FEAResults.keys():
                        self.logger.info("the key name is " + name)
                    fos = float(component.Allowables.mechanical__strength_tensile) / component.FEAResults["VM"]
                    #fos = float(component.MaterialProperty['Mises'])  / component.FEAResults["VM"]
                    component.FEAResults['FOS'] = fos
                    if 'FactorOfSafety' in component.MetricsInfo:
                        component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = fos
                    if 'VonMisesStress' in component.MetricsInfo:
                        component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = component.FEAResults["VM"]

            ################  CSV  ###############################
            with open(self.filename + '.csv', 'wb') as f:
                writer = csv.writer(f)
                writer.writerow(["Unique ID", "Allowable Stress", "Maximum Stress", "Factor of Safety"])
                for component in g_component_list.values():
                    if component.CadType == "PART":
                        writer.writerow([component.ComponentID,
                                         str(component.Allowables.mechanical__strength_tensile),
                                         str(component.FEAResults["VM"]),
                                         str(component.FEAResults["FOS"])])

            ################  Populate Assembly Results  #########
            for component in g_component_list.values():
                self.logger.info('ComponentID: %s' % component.ComponentID)
                self.logger.info(component)
                self.logger.info('')
                if component.CadType == "ASSEMBLY" and not component.IsConfigurationID:
                    FOS = []
                    VM = []
                    for part in component.Children:
                        FOS.append(g_component_list[part].FEAResults["FOS"])
                        VM.append(g_component_list[part].FEAResults["VM"])
                    component.FEAResults["FOS"] = min(FOS)
                    component.FEAResults["VM"] = max(VM)
                    component.MetricsOutput[component.MetricsInfo['FactorOfSafety']] = min(FOS)
                    component.MetricsOutput[component.MetricsInfo['VonMisesStress']] = max(VM)


            ################  Populate Metrics  #################
            computed_values_xml = ComputedMetricsSummary.WriteXMLFile(g_component_list)

            ################  Update Results Json  ##############
            if os.path.exists(self.results_json):
                UpdateReportJson_CAD.update_manifest(self.results_json, computed_values_xml)
            else:
                msg = "Could not update file: {}, file does not exist.".format(self.results_json)
                self.logger.error(msg)
                self.write_failed_and_exit(msg)


            #################################################################################
            # Update tb_manifest_json with fea_total_volume and fea_total_mass, if specified
            #################################################################################

            tb_metrics = None
            tb_manifest_json = None

            with open(self.results_json, 'r') as tb_file:
                tb_manifest_json = json.load(tb_file)


            tb_metrics = tb_manifest_json.get('Metrics')

            found_mass_or_volume = False
            for metric in tb_metrics:
                metric_id = metric['ID']
                metrid_name =  metric['Name']
                print metrid_name
                print globalMetrics
                if ( metrid_name.lower() == "fea_total_volume" or metrid_name.lower() == "fea_total_mass" ):
                    found_mass_or_volume = True
                    if (  metrid_name.lower() == "fea_total_volume" ):
                        metric_value = globalMetrics.get("TotalVolume".lower())
                    else:
                        metric_value = globalMetrics.get("TotalMass".lower())

                    metric['Value'] = metric_value

            if found_mass_or_volume:
                tb_manifest_json['Metrics'] = tb_metrics

                with open(self.results_json, 'w') as tb_file:
                    json.dump(tb_manifest_json, tb_file, indent=4)

            return True

    def run(self):

        if not self.run_patran_pp():
            self.logger.error('"run_patran_pp()" has failed.')
            self.write_failed_and_exit()

        v1_res_name = '{}_out.txt'.format(self.filename)
        v2_res_name = '{}_out_v2.txt'.format(self.filename)

        if os.path.exists(v1_res_name):
            self.logger.info("Calling update_results_files_xdb({})...".format(v1_res_name))
            xdb_method_success = self.update_results_files_xdb(v1_res_name)

            if not xdb_method_success:
                self.logger.error("update_results_files_xdb({}) failed.".format(v1_res_name))
                self.write_failed_and_exit()

        elif os.path.exists(v2_res_name):
            self.logger.info("Calling update_results_files_xdb_op2({})...".format(v2_res_name))
            op2_method_success = self.update_results_files_xdb_op2(v2_res_name)

            if not op2_method_success:
                self.logger.error('"update_results_files_op2({})" failed.'.format(v2_res_name))
                self.write_failed_and_exit()


if __name__ == '__main__':

    logger = get_logger("Patran_PostProcess")

    try:

        parser = argparse.ArgumentParser(description='Post process Nastran output w/ Patran')

        parser.add_argument('MetaDataFile', help='.xml AnalysisMetaData File Name')
        parser.add_argument('RequestedMetrics', help='.xml RequestedMetrics File name')
        parser.add_argument('ResultsJson', help='.json summary testresults File name')

        args = parser.parse_args()

        if not os.path.exists(args.MetaDataFile):
            logger.error("File not found: {}".format(args.MetaDataFile))

        if not os.path.exists(args.RequestedMetrics):
            logger.error("File not found: {}".format(args.RequestedMetrics))

        post_process = PatranPostProcess(args.MetaDataFile, args.RequestedMetrics, args.ResultsJson)
        post_process.run()

    except Exception:  # catch *all* exceptions
        import traceback
        e = sys.exc_info()[0]
        var = traceback.format_exc()
        logger.error("Exception: %s" % var)
        sys.exit(1)

