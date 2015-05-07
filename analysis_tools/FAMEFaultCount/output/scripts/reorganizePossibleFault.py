import os,json,csv,random
from optparse import OptionParser

script_dir = os.path.dirname(os.path.realpath(__file__))
#num_fault_tested = 20

# This function reformat the json file and down select number of fault
def reformatPossibleFault(jsonFilename,outfilename,num_fault_tested):
    with open(os.path.join(script_dir,jsonFilename),'r') as jsonfile:
        jsondata = json.load(jsonfile)


    formattedData = []
    for line in jsondata:
        for mode in line[u'modes']:
            newline = []
            newline.append(line[u'name'].encode('ascii','ignore'))
            newline.append(line[u'type'].encode('ascii','ignore')+".Modes."+mode[0].encode('ascii','ignore'))
            if mode[0].encode('ascii','ignore').lower()[:5] == "broke":
                newline.append(1)
            else:
                newline.append(0.8)
            formattedData.append(newline)


    if num_fault_tested == 0:
        num_fault_tested = len(formattedData)
        #random.shuffle(formattedData)
    else:
        random.shuffle(formattedData)
        
        
    outfile = open(os.path.join(script_dir,outfilename),"wb")
    csvWriter = csv.writer(outfile,delimiter=" ")
    for i in range(num_fault_tested):
        csvWriter.writerow(formattedData[i])
    outfile.close()
 
    
def main():
    jsonFilename = 'possibleFault.json'
    outfilename = "faults.txt"
    parser = OptionParser()
    parser.add_option("-n", "--NumFault", dest="num_fault_tested",
                  help="number of fault to be randomly selected", metavar="VAR",default=0)
    options, args = parser.parse_args()
    reformatPossibleFault(jsonFilename,outfilename,int(options.num_fault_tested))
    
    
if __name__ == "__main__":
    main()
    
