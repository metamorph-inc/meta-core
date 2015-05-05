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

def main():
    from optparse import OptionParser
    parser = OptionParser()
    options = add_options(parser)
    
    debug_log_path = './log/CAD_update_testbench_manifest.log'
    if not os.path.exists('./log'):
        os.makedirs('./log')
    else:
        if os.path.exists(debug_log_path):
            os.remove(debug_log_path)
            
    logger = utility_functions.setup_logger(debug_log_path)

    result_json = {}

    resultmanifest_file = 'testbench_manifest.json'
    if os.path.exists(resultmanifest_file):
        # read current summary report, which contains the metrics
        with open(resultmanifest_file,'r') as file_in:
            result_json = json.load(file_in)

        # update analysis status
        if 'Status' in result_json:
            if os.path.exists('_FAILED.txt'):
                result_json['Status'] = 'FAILED'
            else:
                    result_json['Status'] = 'OK'
        else:
            logger.debug('%s does not contain Status' %(resultmanifest_file))
        
        metrics_file = options.metricfile

        if os.path.exists(metrics_file):
            Parsed_ComputedValueList = dict()
            Parsed_ComputedValueList = ComputedMetricsSummary.ParseXMLFile(metrics_file)
            #print (ComputedMetricsSummary.gMetricSummary)

            if 'Metrics' in result_json:
                for metric in result_json['Metrics']:
                    if 'Name' in metric and 'Value' in metric and 'GMEID' in metric:
                        key = metric['GMEID']
                        if ComputedMetricsSummary.gMetricSummary.has_key(key):
                            # update metric's value to the last value in
                            # time series
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
                logger.debug('% does not contain Metrics' %(resultmanifest_file))
        else:
            logger.debug('Given result file does not exist: {0}'.format(metrics_file))

        # update json file with the new values
        with open(resultmanifest_file,'wb') as file_out:
            json.dump(result_json, file_out, indent=4)
        logger.debug('Finished updating %s file.' %(resultmanifest_file))
        
    else:
        logger.debug('%s does not exist!' %(resultmanifest_file))                   

    
if __name__ == '__main__':
    main()



