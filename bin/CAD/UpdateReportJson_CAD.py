import json
import os
import ComputedMetricsSummary
import logging


def add_options(parser):
    parser.add_option('-m', '--metricfile', help='input metrics file')
    (opts, args) = parser.parse_args()
    return opts


def convert_pp_metric(metric, value, inUnit, outUnit):
    """Convert temperature to outUnit, otherwise returns None."""
    logger = logging.getLogger(__name__)
    logger.info('Converting ' + metric + ' of ' + str(value) + ' ' + inUnit + ' to ' + outUnit)
    if str(outUnit) == 'Degree Celsius':
        return float(float(value) - 273.15)
    elif str(outUnit) == 'Degree Fahrenheit':
        return float(1.8*(float(value) - 273.15) + 32)
    return None


def update_manifest(tbmanifest, computedValuesXml):
    logger = logging.getLogger(__name__)
    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    logger.info('FUNCTION: UpdateReportJson_CAD()\n')

    if os.path.exists(tbmanifest):
        # read current summary report, which contains the metrics
        with open(tbmanifest, 'r') as file_in:
            result_json = json.load(file_in)

        # update analysis status
        if 'Status' in result_json:
            if os.path.exists('_FAILED.txt'):
                result_json['Status'] = 'FAILED'
            else:
                result_json['Status'] = 'OK'
        else:
            logger.debug('%s does not contain Status' % tbmanifest)

        if os.path.exists(computedValuesXml):
            ComputedMetricsSummary.ParseReqMetricsFile(computedValuesXml)

            if 'Metrics' in result_json:
                for metric in result_json['Metrics']:
                    if 'Name' in metric and 'Value' in metric and 'GMEID' in metric:
                        # key = metric['GMEID']  # JK 8/19/2016  How did this ever work?
                        key = metric['ID']
                        if key not in ComputedMetricsSummary.gMetricSummary:
                            key = metric['GMEID']
                        if key in ComputedMetricsSummary.gMetricSummary:
                            # update metric's value to the last value in
                            # time series
                            metric['Value'] = None
                            if metric.get('Unit') is not None and metric['Unit'] != '':
                                metric['Value'] = convert_pp_metric(key, ComputedMetricsSummary.gMetricSummary[key][1],
                                                                    ComputedMetricsSummary.gMetricSummary[key][0],
                                                                    metric['Unit'])
                            if metric['Value'] is None:
                                metric['Value'] = ComputedMetricsSummary.gMetricSummary[key][1]
                                metric['Unit'] = ComputedMetricsSummary.gMetricSummary[key][0]
                            logger.debug('Metric: {0} {1} {2} was updated.'.format(metric['Name'],
                                                                                   metric['Value'],
                                                                                   metric['Unit']))
                        else:
                            # metric was not found in results
                            logger.debug('ComputedMetrics.xml key error: {0}'.format(key))
            else:
                # create warning message
                logger.debug('% does not contain Metrics' % tbmanifest)
        else:
            logger.error('Given result file does not exist: {0}'.format(computedValuesXml))

        # update json file with the new values
        with open(tbmanifest, 'wb') as file_out:
            json.dump(result_json, file_out, indent=4)
        logger.debug('Finished updating %s file.' % tbmanifest)

    else:
        logger.error('%s does not exist!' % tbmanifest)


    
"""
    Description:
        Given a testbench_manifest.json construct as follows:
       "Metrics": [
            {
                "GMEID": "id-0067-0000082f",
                "Description": "",
                "DisplayedName": null,
                "VisualizationArtifacts": [],
                "Value": "",
                "ID": "4fa4174a-9797-45bf-84da-44f07c62a03f",
                "Unit": "",
                "Name": "CAD_Interferences"
            }
        This function supports changing the fields Value and Unit.
        The entry that would be modified is identified by the GMEID field.

        Anomalous conditions are logged, but no other action would be taken other than logging the condition. In
        other words, no exceptions or any other actions would occur that would stop the program.

    Pre-Conditions:
        in_replace_Value and in_replace_Unit must be True or False
        The Metrics construct must have the fields GMEID, Unit, and, Value
        in_gme_id_key is treated in a case insensitive manner (e.g.  id-0067-0000082f is the same as ID-0067-0000082f

    Post-Conditions:
        if in_tbmanifest not found
            then
                log condition, take no other action

        if in_replace_Value == False and in_replace_Unit == False
            then
                no action would be taken.  The condition would be logged.

        if ( in_gme_id_key in Metrics ) # e.g. "GMEID": "id-0067-0000082f" where in_gme_id_key == "id-0067-0000082f"
            if in_replace_Value == True
                then
                    set Value field to in_value
            if in_replace_Unit == True
                then
                    set Unit field to in_unit
        else
            log condition, take no action.

"""
def update_manifest_metric_properties(in_tbmanifest,
                                      in_gme_id_key, # e.g in_name_key == 'CAD_Interferences'
                                      in_replace_Value, in_value,
                                      in_replace_Unit, in_unit):
    logger = logging.getLogger()
    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    logger.info('FUNCTION: modify_manifest_metric_properties()\n')

    # if not asking to change anything just return
    if not in_replace_Value and not in_replace_Unit:
        logger.debug('modify_manifest_metric_properties() was passed False for in_replace_Value and in_replace_Unit, no action taken.\n')
        return

    changes_occurred = False

    if os.path.exists(in_tbmanifest):
        # read current summary report, which contains the metrics
        with open(in_tbmanifest, 'r') as file_in:
            result_json = json.load(file_in)
            if 'Metrics' in result_json:
                for metric in result_json['Metrics']:
                    if 'GMEID' in metric and 'Value' in metric and 'Unit' in metric:
                        if metric['GMEID'].lower() == in_gme_id_key.strip().lower():
                            if in_replace_Value:
                                metric['Value'] = in_value
                            if in_replace_Unit:
                                metric['Unit'] = in_unit
                            changes_occurred = True
            else:
                # create warning message
                logger.debug('% does not contain Metrics' % in_tbmanifest)

        # update json file with the new values
        if changes_occurred:
            with open(in_tbmanifest, 'wb') as file_out:
                json.dump(result_json, file_out, indent=4)
            logger.debug('Finished updating %s file.' % in_tbmanifest)
    else:
        logger.debug('%s does not exist!' % in_tbmanifest)

"""
    Description:
        This function reads the contents of in_interference_file (if it exists) and updates in_tbmanifest with
        the status of the interference check.

        It is normal for in_interference_file to not exist.  This simply means that no request was submitted
        to the CreateAssembly program to conduct an interference analysis.  Performing an interference analysis
        via Creo takes significant time/resources for large assemblies; therefore, interference analyses would
        only be requested when needed.

    Pre-Conditions:
        None

    Post-Conditions:
        If in_interference_file exists and contains the string 'InterferenceCount:' (e.g. InterferenceCount: 2)
            then
                in_tbmanifest would be updated to reflect that interferences count.
            else
                 in_tbmanifest would be updated with a -1 interferences count.  This should flag the user
                 that an error occurred.

        Note - See the function update_manifest_metric_properties for details on how in_tbmanifest would be updated.

        In general, using keywords such as 'InterferenceCount:' is not ideal, and in the future the
        interference information will be persited in a XML file.

        If the InterferenceCount > 0 as indicated by "InterferenceCount: 2" in in_interference_file, then it is
        expected that the user would open the assembly with Creo and run the interference check via the Creo UI.
        This would graphically point out the interferences or in some case would indicate a failure to
        complete the interference analysis.   If the interference analysis fails, the user should run the
        analysis on subsets of the assembly.
"""

def update_manifest_per_interference_check(in_tbmanifest, in_interference_file):
    logger = logging.getLogger()
    logger.info('\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    logger.info('FUNCTION: update_manifest_per_interference_check()\n')

    metric_id_key_lower = "MetricID:".lower()
    metric_id = ''
    interference_count_key_lower = "InterferenceCount:".lower()

    interference_count = int(-1)

    print(in_interference_file)
    if os.path.exists(in_interference_file):
        with open(in_interference_file, 'r') as file_in:
            for line in file_in:
                line_tokens = line.split(' ')
                first_token_lower = line_tokens[0].lower()
                if first_token_lower == metric_id_key_lower:
                    metric_id = line_tokens[1].strip()
                if first_token_lower == interference_count_key_lower:
                    interference_count = int(line_tokens[1].strip())
                if interference_count != -1 and metric_id != '':
                    break

        update_manifest_metric_properties(in_tbmanifest, metric_id, True, interference_count, False, '')

        if interference_count == 0:
            logger.debug('No interferences were detected.')
        else:
            logger.debug('Interferences were detected.')
    else:
        # Do nothing
        # print('interference_file Does NOT exist')
        logger.debug('File %s not found' % in_interference_file)


if __name__ == '__main__':
    import sys
    import logging
    logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)
    manifest = os.path.join(os.getcwd(), 'testbench_manifest.json')
    update_manifest(manifest, 'ComputedValues.xml')
    interference_check_file = os.path.join(os.getcwd(), 'CADAssembly_interference.txt')
    update_manifest_per_interference_check(manifest, interference_check_file)
