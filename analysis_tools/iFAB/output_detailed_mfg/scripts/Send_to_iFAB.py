# Script to call GenerateSubmissionJSON.py, and submit results folder to PSU
import sys
import os
import json
import subprocess
import time
import re
import uuid
import shutil
import _winreg


def query_analysis_tools():
    analysis_tools_key_name = r'SOFTWARE\Wow6432Node\META\AnalysisTools'

    analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                         _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY)

    number_of_keys =  _winreg.QueryInfoKey(analysis_tools_key)[0]  # 0 means number of sub_keys

    info = {}
    for sub_key_id in range(0, number_of_keys):
        sub_key_name = _winreg.EnumKey(analysis_tools_key, sub_key_id)
        sub_key = _winreg.OpenKey(analysis_tools_key, sub_key_name)
        info[sub_key_name] = {}
        number_of_values = _winreg.QueryInfoKey(sub_key)[1]
        for value_id in range(0, number_of_values):
            value_tuple = _winreg.EnumValue(sub_key, value_id)
            value_name = value_tuple[0]
            value = value_tuple[1]
            info[sub_key_name][value_name] = value

    return info

tools = query_analysis_tools()
tool_path = os.path.join(os.path.join(tools['detailed_mfg']['InstallLocation']), "psu_python_library")
keystore_path = os.path.join(tool_path, "projects.keystore")
jar_path = os.path.join(tool_path, "iFABFoundry-VehicleForgeInterface.jar")
base_command = r'java -Xmx4g -Djavax.net.ssl.trustStore="' + keystore_path + '" -Djavax.net.ssl.trustStorePassword=NSTasIC06 -cp "' + jar_path + '"'

def GenerateSubmissionJSON():
    # Create json to write to
    with open('iFAB.submission.json', 'wb') as submission_json: 
        # Setup inner dict template
        submission = dict({ 
            'submissionID': '',
            'designID': '',
            'teamID': '',
            'modelEntities': '',
            'metaDesign': ''
            })

        # Assign inner dict to outer dict
        submission_dict = dict({ 
            'submission': submission
            })
           
        with open('testbench_manifest.json', 'rb') as testresults:
            summary = json.load(testresults)

            #### designID section ####
            if 'DesignID' in summary:
                submission['designID'] = summary['DesignID'].strip('{}')
                
            #### metaDesign section ####
            if 'TestBench' in summary:
                if os.path.exists(summary['TestBench'] + '.adm'):
                    submission['metaDesign'] = str(summary['TestBench'] + '.adm')

            # If the ADM file was not specified by the TestBench field, search the directory for an ADM file
            if 'metaDesign' not in submission:
                print "ADM file not specified in testbench_manifest.json!"
                for file in os.listdir(directory):
                    if file.upper().endswith(".ADM"):
                        submission['metaDesign'] = file
                        print "Found " + file

            #### submissionID section ####
            submission['submissionID'] = str(uuid.uuid4())            
            submissionID = submission['submissionID']
            
            #### teamID section ####
            try:
                with open('stat.json', 'r') as stat:
                    stats = json.load(stat)
                    submission['teamID'] = stats['UserId']
            except:
                submission['teamID'] = 'NoIdFound'
           
        #### modelEntities section ####
        modelEntities = []
    
        with open('manufacturing.manifest.json', 'rb') as manuf_manifest:
            manifest = json.load(manuf_manifest)
           
# This isn't needed anymore now that iFAB processes the ADM and CADAssembly_metrics.xml
#            if 'ComponentManufactureList' in manifest:
#                list = manifest.values()[0]
#                
#                for part_dict in list:
#                    if 'STEPModel' in part_dict:
#                        name = re.search(r'/.+....', part_dict['STEPModel'])
#                        # stpPrefix is the folder containing all stp files
#                        stpPrefix = re.search(r'.+/', part_dict['STEPModel'])
#                        if name != None:
#                            name = name.group()
#                            name = name.strip('/.')         
#
#                        geometry = part_dict['STEPModel']
#                    else:
#                        continue
#                    if 'id' in part_dict:
#                        partID = part_dict['id']
#                    if 'ManufacturingModel' in part_dict:
#                        details = part_dict['ManufacturingModel']
#                        
#                    # Create subdict containing info on each part
#                    list_dict = dict({
#                            'name': name[:-4], # Name = stp file minus '_prt.stp'
#                            'partID': partID.strip('{}'),
#                            'geometry': geometry,
#                            'details': details,
#                        })
#                    # Add each subdict to mE list, 
#                    # assign the list of dicts to modelEntities key in submission dict                        
#                                    
#                                        
#                    modelEntities.append(list_dict)	
#                
#                # Sort modelentities by name values
#                modelEntities = sorted(modelEntities, key=lambda x: x['name'])

            
        # Locate the STEP assembly file    
        stpPrefix = "AP203_E2_SEPARATE_PART_FILES\\"
        matchingFiles = None

        if not os.path.exists(stpPrefix):
            print "Could not find AP203_E2_SEPARATE_PART_FILES directory!"

        r = re.compile(manifest['Name'] + '.*_asm.stp', flags=re.I)
        matchingFiles = [ x for x in os.listdir(stpPrefix) if r.match(x) ]

        if len(matchingFiles) == 0:
            r = re.compile(manifest['Name'].replace(" ", "_") + '.*_asm.stp', flags=re.I)
            matchingFiles = [ x for x in os.listdir(stpPrefix) if r.match(x) ]

        if len(matchingFiles) == 0:
            r = re.compile('.*_asm.stp', flags=re.I)
            matchingFiles = [ x for x in os.listdir(stpPrefix) if r.match(x) ]

        if len(matchingFiles) == 0:
            print "Unable to locate STEP assembly file!"
        else:
            print "Found STEP assembly: " + matchingFiles[0]

            modelEntities.append(dict({
                'name': re.sub(r'_asm.*', '_asm', matchingFiles[0]).upper(),
                'partID': submission['designID'],
                'geometry': str(stpPrefix + matchingFiles[0]),
                'isTopLevel': True}))
                
        submission['modelEntities'] = modelEntities
            
        # Write all information to json file
        json.dump(submission_dict, submission_json, indent=4) 

    return submissionID

def Create_Submission_Request():
    """ Create Submission Request """
    create_sub_request = r'edu.psu.arl.ifoundry.web.trustedclient.vehforge.CreateSubmissionRequest'
    call = subprocess.Popen(base_command + ' ' + create_sub_request + ' "' + os.getcwd() + '"', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout = call.stdout.read()
    stderr = call.stderr.read()
    call.communicate()

    # If the following is not hit in this command it will not be hit in the other functions. 
    if stderr != '':
        print stderr
        print ''
        raise Exception('\n\n'
            'Java exception. Missing keystore or IP address is firewalled.\n'
            'If exception above mentions "trustAnchors parameter must be non-empty", machine the test'
            'bench is running on does not have the required keystore file. If exception says the '
            'connection has timed out, your IP address has been firewalled.\n\n')
    
def Query_Status(UUID):
    """ Query PSU to see if analysis has completed """
    query_status = r'edu.psu.arl.ifoundry.web.trustedclient.vehforge.SummaryStatusRequest'
    call = subprocess.Popen(base_command + ' ' + query_status + ' ' + UUID, stdout=subprocess.PIPE, shell=True)
    out, err = call.communicate()
    return json.loads(out)

def ManufacturabilityResultRequest(UUID):
    """ Query PSU for the analysis results """
    costWeight = 0.5
    timeWeight = 0.5
    result_request = r'edu.psu.arl.ifoundry.web.trustedclient.vehforge.ManufacturabilityResultRequest'
    call = subprocess.Popen(base_command + ' ' + result_request + ' ' + str(costWeight) + ' ' + str(timeWeight) + ' ' + UUID, stdout=subprocess.PIPE, shell=True)
    out, err = call.communicate()
    return json.loads(out)
    
def DownloadAllfiles(out):
    """ Download any detailed feedback PDFs """
    fileIds = []

    # Grab the files for manufacturable submissions
    if "bestScore" in out[0]:
        for entry in out[0]["bestScore"]["manufacturingDetails"]:
            if "detailedFeedback" in entry:
                fileIds.append(entry["detailedFeedback"]["fileID"])

    # Grab the files for non-manufacturable submissions
    if "manufacturabilityAssessment" in out[0]:
        for entry in out[0]["manufacturabilityAssessment"]:
            if "detailedFeedback" in entry:
                fileIds.append(entry["detailedFeedback"]["fileID"])

    # Download the files if there are any
    if len(fileIds) > 0:
        feedbackDir = os.path.join(".", "feedback")

        if not os.path.exists(feedbackDir):
            os.mkdir(feedbackDir)

        file_download = r'edu.psu.arl.ifoundry.web.trustedclient.vehforge.FileDownloadRequest'
        call = subprocess.Popen(base_command + ' ' + file_download + ' ' + ' '.join(fileIds) + ' ' + feedbackDir, stdout=subprocess.PIPE, shell=True)
        out, err = call.communicate()
    else:
        print "No feedback PDFs to download."

def CreateDummySubmission():
    submissionID = str(uuid.uuid4())

    with open('iFAB.submission.json', 'r') as data_file:
        data = json.load(data_file)

    data["submission"]["submissionID"] = submissionID

    with open('iFAB.submission.json', 'w') as data_file:
        json.dump(data, data_file, indent=4) 

    return submissionID
    
def main():
    # Create Submission.json and extract UUID
    submissionID = GenerateSubmissionJSON()
    #submissionID = CreateDummySubmission()
    UUID = str(submissionID)
    print 'Generated UUID:', UUID 

    # Send to iFAB
    Create_Submission_Request()
    
    # Query Status Results
    IsCompleted = False

    while IsCompleted == False:
        out = Query_Status(UUID) # Query submission status        

        if out[0]["status"]=="complete" or out[0]["status"]=="failed":
            print "Manufacturability assessment completed!"
            IsCompleted = True
        elif out[0]["status"]=="error":
            raise Exception(out)
        else:
            print "Still running,", out[0]["completedTasks"], "of", out[0]["totalTasks"], "tasks complete."
            time.sleep(30)

    # Get the manufacturing results
    out = ManufacturabilityResultRequest(UUID)
        
    # Write the output of the results to a json file to be saved in specific results folder
    with open('detailed.manufacturabilityResults.json', 'w') as result_file:        
        json.dump(out, result_file, indent=4)

    # Finally download any PDF feedback documents that have been generated
    DownloadAllfiles(out)

if __name__ == '__main__':
    main()
            