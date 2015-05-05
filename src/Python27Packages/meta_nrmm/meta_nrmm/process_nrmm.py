from Cheetah.Template import Template
import os
import os.path
import imp
import sys
import datetime
import json
import re
import string
import shutil
from win32file import CreateHardLink
import pywintypes

OBSEXEPATH=r'C:\NRMM\OBS78B'
OBSEXE='OBSDP.exe'

NRMMEXEPATH=r'C:\NRMM\NRMMII'
NRMMEXE='nrmmG95.exe'

OBS_VEH_FILENAME='fang_gen.VEH'
OBS_VEH_TEMPLATENAME='fang_gen.VEH.tmpl'
NRMM_STD_FILENAME='fang_gen.STD'
NRMM_STD_TEMPLATENAME='fang_gen.STD.tmpl'

template_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates')

DATA_DIR = r'C:\NRMM\NRMMII\data'

FANG_DATA = ['TRACK.OBS', 'Wheel.obs', 'fang_NK.INP', 'fang_GE.INP', 'fang_ME.INP', 'fang.INP']

def link_files_to_cwd_then_remove(paths):
    for path in paths:
        try:
            CreateHardLink(os.path.basename(path), path)
        except pywintypes.error as e:
            if e.winerror == 5: # ACCESS DENIED
                shutil.copy(path, ".")
            elif e.winerror != 183: # "Cannot create a file when that file already exists"
                raise
    import atexit
    def remove_files(paths):
        for path in paths:
            os.unlink(path)
    atexit.register(remove_files, [os.path.join(os.getcwd(), os.path.basename(path)) for path in paths])

def check_system(cmdline):
    import subprocess
    subprocess.check_call(cmdline, shell=True)
    return
    retval = os.system(cmdline)
    if retval != 0:
        sys.stderr.write('Command "%s" failed with exit code %d\n' % (cmdline, retval))
        sys.exit(retval)
            

def main(argv):
    link_files_to_cwd_then_remove([os.path.join(DATA_DIR, filename) for filename in os.listdir(DATA_DIR)])
    link_files_to_cwd_then_remove([os.path.join(os.path.dirname(os.path.abspath(__file__)), filename) for filename in FANG_DATA])
                
    # $FrontAxleWeight
    # $RearAxleWeight
    #
    #
    #
    #
    DefaultFrontAxleWeight=1000.0
    DefaultRearAxleWeight=2000.0
    DefaultFinalDriveRatio=4.67
    DefaultFinalDriveEfficiency=0.98

    ##########################################
    # parse metrics file
    ##########################################

    #if not os.path.exists('summary.report.json'):
    #    print 'File does not exist: summary.report.json'
    #    sys.exit()

    if (len(argv) > 1):
        if not os.path.exists(argv[1]):
            print 'Given metric file does not exist: {0}'.format(argv[1])
            sys.exit(2)
    else:
        sys.stderr.write('Usage: %s testbench_manifest.json\n' % sys.argv[0])
        sys.exit(2)
        
    result_json = {}
    # read current summary report, which contains the metrics

    #with open('summary.report.json', 'r') as file_in:
    with open(argv[1], 'r') as file_in:
        result_json = json.load(file_in)

    nameSpace = {'FrontAxleWeight':DefaultFrontAxleWeight, 'RearAxleWeight':DefaultRearAxleWeight, 'FinalDriveRatio':DefaultFinalDriveRatio, 'FinalDriveEfficiency':DefaultFinalDriveEfficiency}

    if 'Metrics' in result_json:
        for metric in result_json['Metrics']:
            if 'Name' in metric and 'Value' in metric:
                if metric['Name'] in nameSpace:
                    print metric['Name'] + ': '+ metric['Value']
                    nameSpace[metric['Name']] = float(metric['Value'])
            else:
                # create warning message
                pass
    else:
        # create warning message
        print 'no metric file'
        sys.exit(3)
        
    nameSpace.update({'vehicle_identifier':'M1 TANK WES Jan94', 'VEHICLE':'$VEHICLE', 'VEHICL':'$VEHICL', 'END':'$END', 'LFVDAT':'$LFVDAT'})

    #print 'hit enter'
    #os.system('pause')

    ##########################################
    # LATER: Generate *.INP  (points to *.VEH)
    ##########################################

    ##########################################
    # create *.veh for OBSDP.exe
    ##########################################

    templateDef = """ 1 ${vehicle_identifier} 2
    """
    template_file = OBS_VEH_TEMPLATENAME
    template_fullpath = os.path.join(template_path, template_file)

    t = Template(file=template_fullpath, searchList=[nameSpace])  #searchList=[nameSpace])
    now = datetime.datetime.now()
    t.timestamp = now.strftime("%Y-%m-%d %H:%M")
    t.filename = template_file
    print t
    absoutputfilename = OBS_VEH_FILENAME

    try:
        f = open(absoutputfilename, "wb")
    except Exception, e:
        self.log("Skipping Output: Could not open file for writing %s: %s" % (absoutputfilename, str(e)), 3)
                    
    f.write(str(t))
    f.close()

    print 'wrote ' + absoutputfilename

    #print 'hit enter'
    #os.system('pause')


    ##########################################
    # run OBSDP.exe < *.inp > *.log
    ##########################################

    cmdexe_fullpath = os.path.join(OBSEXEPATH, OBSEXE)
    #cmdline = cmdexe_fullpath + '< fang.INP > fang_OBS.log'
    cmdline = cmdexe_fullpath + '< fang.INP > fang_OBS.log'

    if not os.path.exists(cmdexe_fullpath):
        print 'File does not exist:' + cmdexe_fullpath
        os.exit(4)

    print cmdline

    #print 'hit enter to run OBSDP'
    #os.system('pause')

    # FIXME: this is known to be broken.
    #check_system(cmdline)

    #print 'hit enter'
    #os.system('pause')

    ##########################################
    # parse results (and store for append to STD)
    ##########################################


    ##########################################
    # LATER: Generate *.INP  (points to *.STD)
    ##########################################

    ##########################################
    # create *.STD
    ##########################################

    template_file = NRMM_STD_TEMPLATENAME
    template_fullpath = os.path.join(template_path, template_file)

    t = Template(file=template_fullpath, searchList=[nameSpace])  #searchList=[nameSpace])
    now = datetime.datetime.now()
    t.timestamp = now.strftime("%Y-%m-%d %H:%M")
    t.filename = template_file
    print t
    absoutputfilename = NRMM_STD_FILENAME

    try:
        f = open(absoutputfilename, "wb")
    except Exception, e:
        self.log("Skipping Output: Could not open file for writing %s: %s" % (absoutputfilename, str(e)), 3)
                    
    f.write(str(t))
    f.close()

    print 'wrote ' + absoutputfilename

    #print 'hit enter'
    #os.system('pause')


    ####################################################################################
    # Copy *.OBS results and maybe *.veh to the end (*.veh at end will be ignored)
    ####################################################################################



    ##########################################
    # NRMMII: run nrmmG95 < *.inp > *.log
    ##########################################

    #### ME
    cmdexe_fullpath = os.path.join(NRMMEXEPATH, NRMMEXE)
    cmdline = cmdexe_fullpath + '< fang_ME.INP'
    #print cmdexe_fullpath
    print cmdline
    if not os.path.exists(cmdexe_fullpath):
        print 'File does not exist:' + cmdexe_fullpath
        os.exit(5)

    check_system(cmdline)

    #### GE
    cmdexe_fullpath = os.path.join(NRMMEXEPATH, NRMMEXE)
    cmdline = cmdexe_fullpath + '< fang_GE.INP'
    print cmdline
    if not os.path.exists(cmdexe_fullpath):
        print 'File does not exist:' + cmdexe_fullpath
        os.exit(6)
        
    check_system(cmdline)

    #### NK
    cmdexe_fullpath = os.path.join(NRMMEXEPATH, NRMMEXE)
    cmdline = cmdexe_fullpath + '< fang_NK.INP'
    print cmdline
    if not os.path.exists(cmdexe_fullpath):
        print 'File does not exist:' + cmdexe_fullpath
        os.exit(7)
    check_system(cmdline)

    ##########################################
    # parse results & write into metric file
    ##########################################

    # parse fang_ME.sta

    #VCI
    #"Percent area NO-GO"

    ### TODO: grab title, matrix with [title, GO_NOGO, VCI]
    ### TODO: table matching title with metric names [title, metric_name]
    #Germany_DryNormal_Go_NoGo

    nrmm_metrics=[]
    nrmm_v80_metrics=[]

    sta_file = open("fang_GE.sta", "r").readlines()
    for line in sta_file:
        if re.search('Percent area NO-GO',  line):
            #print 'found a match: ' + line
            words = line.split()
            #print 'words: '
            #print words
            nrmm_metrics.append(words[-1])

    #Parse V80
    sta_file = open("fang_GE.sta", "r").readlines()
    for line in sta_file:
        n = string.find(line, '8X');
        if n > 0 and line[1] != 'h':
            #print n
            v80_str = line[n+3] + line[n+4] + line[n+5] + line[n+6] + line[n+7]
            print v80_str
            v80 = float(v80_str)
            #print v80
            nrmm_v80_metrics.append(str(v80))
            
    #print 'hit enter (end parse V80)'
    #os.system('pause')

            
    sta_file = open("fang_ME.sta", "r").readlines()
    for line in sta_file:
        if re.search('Percent area NO-GO',  line):
            #print 'found a match: ' + line
            words = line.split()
            #print 'words: '
            #print words
            nrmm_metrics.append(words[-1])

    sta_file = open("fang_ME.sta", "r").readlines()
    for line in sta_file:
        n = string.find(line, '8X');
        if n > 0 and line[1] != 'h':
            #print n
            v80_str = line[n+3] + line[n+4] + line[n+5] + line[n+6] + line[n+7]
            #print v80_str
            v80 = float(v80_str)
            #print v80
            nrmm_v80_metrics.append(str(v80))
            
    sta_file = open("fang_NK.sta", "r").readlines()
    for line in sta_file:
        if re.search('Percent area NO-GO',  line):
            #print 'found a match: ' + line
            words = line.split()
            #print 'words: '
            #print words
            nrmm_metrics.append(words[-1])
            
    sta_file = open("fang_NK.sta", "r").readlines()
    for line in sta_file:
        n = string.find(line, '8X');
        if n > 0 and line[1] != 'h':
            #print n
            v80_str = line[n+3] + line[n+4] + line[n+5] + line[n+6] + line[n+7]
            #print v80_str
            v80 = float(v80_str)
            #print v80
            nrmm_v80_metrics.append(str(v80))
            
    print 'metrics: '
    print nrmm_metrics
    print nrmm_v80_metrics
            
    ##########################################
    # write to metric file
    ##########################################

    for metric in result_json['Metrics']:
        if 'Name' in metric and 'Value' in metric:
            if metric['Name'] == 'Germany_DryNormal_Go_NoGo':
                metric['Value'] = nrmm_metrics[0]
            if metric['Name'] == 'Germany_WetNormal_Go_NoGo':
                metric['Value'] = nrmm_metrics[1]
            if metric['Name'] == 'Germany_Snow_Go_NoGo':
                metric['Value'] = nrmm_metrics[2]
            if metric['Name'] == 'ME_DryNormal_Go_NoGo':
                metric['Value'] = nrmm_metrics[3]
            if metric['Name'] == 'ME_Sand_Go_NoGo':
                metric['Value'] = nrmm_metrics[4]
            if metric['Name'] == 'NK_DryNormal_Go_NoGo':
                metric['Value'] = nrmm_metrics[5]
            if metric['Name'] == 'NK_WetNormal_Go_NoGo':
                metric['Value'] = nrmm_metrics[6]
            if metric['Name'] == 'Germany_DryNormal_V80':
                metric['Value'] = nrmm_v80_metrics[0]
            if metric['Name'] == 'Germany_WetNormal_V80':
                metric['Value'] = nrmm_v80_metrics[1]
            if metric['Name'] == 'Germany_Snow_V80':
                metric['Value'] = nrmm_v80_metrics[2]
            if metric['Name'] == 'ME_DryNormal_V80':
                metric['Value'] = nrmm_v80_metrics[3]
            if metric['Name'] == 'ME_Sand_Go_V80':
                metric['Value'] = nrmm_v80_metrics[4]
            if metric['Name'] == 'NK_DryNormal_V80':
                metric['Value'] = nrmm_v80_metrics[5]
            if metric['Name'] == 'NK_WetNormal_V80':
                metric['Value'] = nrmm_v80_metrics[6]           
        else:
                # create warning message
            pass
            
    print 'write out metrics'

    # update json file with the new values

    with open(argv[1], 'wb') as file_out:
        json.dump(result_json, file_out)


    print 'done'
    sys.exit(0)


if __name__=='__main__':
    main(sys.argv)
