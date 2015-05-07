import json
import os
import sys
from xml.etree.ElementTree import ElementTree
import ComputedMetricsSummary
import utility_functions
import logging


def add_options(parser):
    parser.add_option('-m', '--metricfile', help='input metrics file')
    (opts, args) = parser.parse_args()
    return opts
    

def convert_pp_metric(metric, value, inUnit, outUnit):
    # If GME testbench metrics have either degrees Celsius or Fahrenheit,
    # convert data from Kelvin to desired units.    
    logger = logging.getLogger()
    logger.info('Converting ' + metric + ' of ' + str(value) + ' ' + inUnit + ' to ' + outUnit)
    if str(outUnit) == 'Degree Celsius':
        return float(float(value) - 273.15)
    elif str(outUnit) == 'Degree Fahrenheit':
        return float(1.8*(float(value) - 273.15) + 32)   
    elif str(outUnit) == 'MPa':
        # Place holder for now
        return value
    

def update_manifest(tbmanifest, computedValuesXml):
    #from optparse import OptionParser
    #parser = OptionParser()
    #options = add_options(parser)

    logger = logging.getLogger()
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
                        key = metric['GMEID']
                        if key in ComputedMetricsSummary.gMetricSummary:
                            # update metric's value to the last value in
                            # time series
                            if metric['Unit'] != '':
                                metric['Value'] = convert_pp_metric(key, ComputedMetricsSummary.gMetricSummary[key][1],
                                                                    ComputedMetricsSummary.gMetricSummary[key][0],
                                                                    metric['Unit'])
                            else:
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
            logger.debug('Given result file does not exist: {0}'.format(computedValuesXml))

        # update json file with the new values
        with open(tbmanifest, 'wb') as file_out:
            json.dump(result_json, file_out, indent=4)
        logger.debug('Finished updating %s file.' % tbmanifest)
        
    else:
        logger.debug('%s does not exist!' % tbmanifest)

    
if __name__ == '__main__':
    manifest = os.path.join(os.getcwd(), 'testbench_manifest.json')
    update_manifest(manifest, 'ComputedValues.xml')
