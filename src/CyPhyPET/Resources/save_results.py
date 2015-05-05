
import io
import json
import csv

class save_results():

    jsonDict = dict({'inputNames': ''})
    jsonDict.update({'inputNames': list()})
    jsonDict['outputNames'] = []
    jsonDict['result'] = []
    
    Assembly = None
    
    def __init__(self, assembly, results):
        self.Assembly = assembly
        # Get name
        self.jsonDict['name'] = assembly.name
        
        # Get type
        self.jsonDict['type'] = 'TODO TODO TODO' #assembly.driver.type
        
        # Get input names
        for i in assembly.driver.list_param_targets():
            self.jsonDict['inputNames'].append(i)
        # Get output names
        for i in assembly.driver.case_outputs:
            self.jsonDict['outputNames'].append(i)
        
        
        # Get results
        for c in assembly.driver.recorders[0].get_iterator():
            thisResult = dict();
            for i in assembly.driver.list_param_targets():
                thisResult[i] = c[i]
            # Get output names
            for i in assembly.driver.case_outputs:
                thisResult[i] = c[i]
                
            self.jsonDict['result'].append(thisResult)
            
    def save(self, filename):
        # save dictionary into a JSON file
        value = json.dumps(self.jsonDict, indent = 4)
        f = open(filename, 'w+')
        f.write(value)
        f.close()
        
        
        with open('output.csv', 'wb') as fcsv:
            writer = csv.writer(fcsv)
            j = 0
            
            thisResult = ["#id"]
            
            for i in self.Assembly.driver.list_param_targets():
                thisResult.append(i)
            # Get output names
            for i in self.Assembly.driver.case_outputs:
                thisResult.append(i)
            
            writer.writerow(thisResult)
            
            for c in self.Assembly.driver.recorders[0].get_iterator():
                j = j + 1;
                thisResult = [j]
                
                for i in self.Assembly.driver.list_param_targets():
                    thisResult.append(c[i])
                # Get output names
                for i in self.Assembly.driver.case_outputs:
                    thisResult.append(c[i])
                    
                
                writer.writerow(thisResult)
    
        # return with JSON in a string format
        return value

    def getJSON(self):
        # return with the dictionary
        return json.dumps(self.jsonDict)