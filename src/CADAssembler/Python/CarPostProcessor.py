import os
import json
import logging

def update_manifest(tbmanifest, metricvalues):
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

        if 'Metrics' in result_json:
            for metric in result_json['Metrics']:
                if 'Name' in metric and 'Value' in metric and 'GMEID' in metric:
                    key = metric['Name']
                    if key in metricvalues:
                        # update metric's value to the last value in
                        # time series
                        metric['Value'] = metricvalues[key]
        else:
            # create warning message
            print('% does not contain Metrics' % tbmanifest)
    else:
        print('Given result file does not exist: {0}'.format(tbmanifest))

    # update json file with the new values
    with open(tbmanifest, 'wb') as file_out:
        json.dump(result_json, file_out, indent=4)
    print('Finished updating %s file.' % tbmanifest)

class timeinfo(object):
    def __init__(self):
        self.starttime = 0.0
        self.endtime = 0.0
        self.range = 0.0
        self.numsteps = 0

    def readfrom(self, filename):
        with open(filename, 'r') as file_in:
            for line in file_in:
                if 'Maximum Value' in line:
                    self.endtime = self.__extractvalue(line)
                elif 'Minimum Value' in line:
                    self.starttime = self.__extractvalue(line)
                elif 'Time Range' in line:
                    self.range = self.__extractvalue(line)
                elif 'Number of steps' in line:
                    self.numsteps = self.__extractvalue(line)


    def __extractvalue(self,input):
        tokens = input.split('=')
        return float(tokens[1].split(' ')[1].strip())

def readadamsoutput(filename):
    with open(filename, 'r') as file_in:
        i = 0
        result = []
        for line in file_in:
            if i<6:
                i = i+1
            else:
                result.append(float(line.strip()))

    return result

def processmetric(metric):
    timeinf = timeinfo()
    timeinf.readfrom('TIME_' + metric['SimName'] +  '_res.txt')
    series = readadamsoutput('test_' + metric['SimName'] + '.' + metric['AdamsResultName']+'_res.txt')
    if metric['ReverseLookup']:
        time = timeinf.starttime
        # Find the 1st number in the series which matches
        for i in series:
            if i > float(metric['LookupValue']):
                return time
            time = time + (timeinf.range/timeinf.numsteps)
    else:
        if metric['Type']==0:
            if float(metric['LookupValue'])>timeinf.endtime:
                index = len(series)-1
            else:
                index = int((float(metric['LookupValue']-timeinf.starttime))/(timeinf.range/timeinf.numsteps))
            num = series[index]
        elif metric['Type']==1: # Max
            max=-111111
            for i in series:
                if i>max:
                    max=i
            num=max
        elif metric['Type']==2: # Min
            min=111111
            for i in series:
                if i<min:
                    min=i
            num=min
        elif metric['Type']==3: # Range
            min=111111
            max=-111111
            for i in series:
                if i<min:
                    min=i
                if i>max:
                    max=i
            num=max-min
        return num

def exitwitherror(msg, code, program=''):
    logger = logging.getLogger()
    logger.error(msg)
    with open('_FAILED.txt', 'a') as f:
        f.write(program + ': ' + msg + '\n')  # python will convert \n to os.linesep
    exit(code)

def main():
    try:
        with open('metrics_tmp.json', 'r') as file_in:
            result_json = json.load(file_in)


        metricvalues = {}

        for jitem in result_json:
            metricvalues[jitem['MetricID']] = processmetric(jitem)

        update_manifest('testbench_manifest.json', metricvalues)
    except:
        exitwitherror("Job failed.", -1)

if __name__ == '__main__':
    main()