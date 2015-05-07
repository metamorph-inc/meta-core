import os
import json
import sys
import shlex
import subprocess
import logging
import itertools, collections
import re

om_script = "model2dae.mos"  # name of generated Open Modelica script

class CygwinError(Exception):
    """Exception raised for dealing with Cygwin.

    Attributes:
        msg  -- explanation of the error
    """
    def __init__(self, msg):
        self.msg = msg

def error(msg):
    log.critical('{} -- exiting.'.format(msg))
    sys.exit(1)

def getHSalExe():
    if sys.platform.startswith('win'):  # windows
        import _winreg
        analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, 
                                             r'SOFTWARE\Wow6432Node\META\AnalysisTools', 
                                             0,
                                             _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY)
        number_of_keys = _winreg.QueryInfoKey(analysis_tools_key)[0]  # 0 means number of sub_keys
        for sub_key_id in range(0, number_of_keys):
            sub_key_name = _winreg.EnumKey(analysis_tools_key, sub_key_id)
            if sub_key_name == "HybridSal":
                sub_key = _winreg.OpenKey(analysis_tools_key, sub_key_name)
                number_of_values = _winreg.QueryInfoKey(sub_key)[1]
                for value_id in range(0, number_of_values):
                    value_tuple = _winreg.EnumValue(sub_key, value_id)
                    if value_tuple[0] == "InstallLocation": # we foud what we were looking for
                        return os.path.join(value_tuple[1], "bin", "hsalRA.exe")
    return None

def updateEnviron(log,varname,value):
    if varname in os.environ:
        os.environ[varname] += os.pathsep + value
        log.debug("  Added {} to existing {} environment variable.".format(value,varname))
    else:
        os.environ.update({varname:value})
        log.debug("  Created envrionment variable {}: {}".format(varname,os.environ[varname]))

def printStdOutErr(log,name,stdout,stderr):
    if stdout: # not empty
        print
        log.info("  === start of {} STDOUT ===\n{}".format(name,stdout.strip()))
        log.info("  === end of {} STDOUT ===".format(name))
    if stderr: # not empty
        print
        log.info("  === start of {} STDERR ===\n{}".format(name,stderr.strip()))
        log.info("  === end of {} STDERR ===".format(name))

def main():
    # -------------------------------------------------
    # set up logging
    if not os.path.isdir('log'):
        os.mkdir('log')
    # set up logging to file - see previous section for more details
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(levelname)-8s %(message)s',
                        #datefmt='%m-%d %H:%M',
                        filename='log/HybridSal_runner.log',
                        filemode='w')
    # define a handler which writes INFO messages or higher to the sys.stderr
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    # tell the handler to use simpler format
    console.setFormatter(logging.Formatter('%(levelname)-8s %(message)s'))
    # add the handler to the main logger
    log = logging.getLogger(__name__)
    log.addHandler(console)    

    # -------------------------------------------------
    log.info("* Identify test bench for verification...")
    cyphy_dir = os.path.normpath("CyPhy")
    if not os.path.isdir(cyphy_dir):
        error('Couldn\'t find directory {}'.format(cyphy_dir))

    # read model config data
    model_config = os.path.normpath(os.path.join(cyphy_dir,"model_config.json"))
    if not os.path.isfile(model_config):
        error('Couldn\'t find file {}'.format(model_config))
    with open(model_config,'r') as f:
        data = json.load(f)
    log.info("  Read configuration from file {}".format(model_config))
    #print json.dumps(data,indent=2)

    # setup OPENMODELICALIBRARY (if any library paths are given)
    lib_paths = ""
    # supports relative paths and adds the package paths if exist
    for lib_path in data["lib_package_paths"]:
        lib_full_path = os.path.abspath(os.path.join(cyphy_dir,lib_path))
        if os.path.exists(lib_full_path):
            log.debug("  Found additional library path {}".format(lib_full_path))
            lib_paths += lib_full_path
            lib_paths += os.pathsep
        else:
            log.warning("This library path does not exist, so not adding to env: {}".format(lib_full_path))
    if lib_paths: # not empty
        if 'OPENMODELICALIBRARY' in os.environ:
            os.environ['OPENMODELICALIBRARY'] += os.pathsep + lib_paths
            log.debug("  Added paths to existing OPENMODELICALIBRARY environment variable.")
        else:
            if os.name == 'nt':
                om_std_lib = os.path.join(os.environ['OPENMODELICAHOME'],"lib","omlibrary")
            elif os.name == 'posix':
                om_std_lib = os.sep + os.path.join("usr","lib","omlibrary")
            else:
                error('Only Windows and Linux are supported.')
            om_lib = {'OPENMODELICALIBRARY':'{}{}{}'.format(om_std_lib,os.pathsep,lib_paths)}
            os.environ.update(om_lib)
            log.debug("  Created envrionment variable OPENMODELICALIBRARY: {}".format(os.environ['OPENMODELICALIBRARY']))

    # write .mos file from data
    model_name = data["verification_model_name"]
    result_prefix = model_name.split('.')[2] 
    om_script_path = os.path.join(cyphy_dir,om_script)
    with open(om_script_path,'w') as f:
        print >> f, '''
//echo(false);
loadModel(Modelica, {{"{}"}}); print(getErrorString());
loadFile("{}"); print(getErrorString());'''.format(data["MSL_version"],data["model_file_name"])
        for name in data["lib_package_names"]:
            f.write('loadModel({}); print(getErrorString());\n'.format(name))
        print >> f, '''dumpXMLDAE({},"optimiser",addMathMLCode=true,fileNamePrefix="{}"); print(getErrorString());
'''.format(model_name,result_prefix)
    log.info("  Written Open Modelica script to file {}".format(om_script_path))

    # -------------------------------------------------
    print
    log.info("* Translate test bench to DAEs using Open Modelica...")
    daexml_file = os.path.abspath(os.path.join(cyphy_dir,result_prefix+".xml"))
    if sys.platform.startswith('win'):  # windows
        omc_path = os.path.normpath(os.path.join(os.environ["OPENMODELICAHOME"], "bin", "omc.exe"))
        if not os.path.isfile(omc_path):
            error('Couldn\'t find Open Modelica compiler {}'.format(omc_path))
        cmd = "{} {}".format(omc_path,om_script).split()
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=cyphy_dir)
        (stdout, stderr) = process.communicate()
        printStdOutErr(log,"omc.exe",stdout,stderr)
        # abort if omc.exe throws error!
        if process.returncode:
            log.error("  Calling 'omc.exe' threw an error: {} -- exiting!".format(process.returncode))
            sys.exit(process.returncode)
        else:
            log.debug("  Called 'omc.exe' with script {}".format(os.path.join(cyphy_dir,om_script)))
            log.debug("  This should have created file {}".format(daexml_file))
    else:
        log.warning("Skipping omc.exe because not in Windows!")

    # -------------------------------------------------
    print
    log.info("* Run HybridSal on DAE xml...")
    if not os.path.isfile(daexml_file):
        error('Couldn\'t find DAE XML file\n  {}'.format(daexml_file))
    log.debug("  Found DAE XML file at {}".format(daexml_file))

    if sys.platform.startswith('win'):
        hsal_exe = getHSalExe()
        if not os.path.isfile(hsal_exe):
            error('Couldn\'t find HybridSal executable at\n  {}'.format(hsal_exe))
        log.debug("  Found HybridSal at {}".format(hsal_exe))

        # updating PATH and Java classpath... (still need to run from hsal_dir!)
        hsal_dir = os.path.dirname(hsal_exe)
        updateEnviron(log,'PATH',hsal_dir)
        hsal2xml_jar = os.path.join(hsal_dir,'hybridsal2xml.jar')
        updateEnviron(log,'CLASSPATH',hsal2xml_jar)

        log.debug("  Running HybridSal on file {}".format(daexml_file))
        #cmd = "{} {} {}".format(hsal_exe,daexml_file,os.path.abspath(os.path.join(cyphy_dir,"msd_prop.json"))).split() # TODO: remove this!
        cmd = "{} {}".format(hsal_exe,daexml_file).split()
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=hsal_dir)
        (stdout, stderr) = process.communicate()
        printStdOutErr(log,"HybridSal",stdout,stderr)

        # -------------------------------------------------
        print
        log.info("* Processing the result...")
        # read manifest
        summary_file = os.path.join(os.path.dirname(cyphy_dir),'testbench_manifest.json')
        summary_results = {}
        with open(summary_file,'r') as f:
            summary_results = json.load(f)

        # update status
        if process.returncode: # HybridSal created a non-zero return code
            log.warning('  HybridSal failed with code {}'.format(process.returncode))
            summary_results['Status'] = 'FAILED'
        else:
            summary_results['Status'] = 'OK'
        if not 'FormalVerification' in summary_results:
            summary_results['FormalVerification'] = []
        verif_results = summary_results['FormalVerification']

        hsal_results = {'Source': 'SRI', 'Result': 'UNKNOWN', 'ReqName': '-unknown-', 'Details': []}

        # obtain "ReqName" entry (if it exists)
        limits_file = os.path.abspath(os.path.join(cyphy_dir,"limits.json"))
        if os.path.isfile(limits_file):
            log.debug("  Found limits in file {}".format(limits_file))
            limits = {}
            with open(limits_file,'r') as f:
                limits = json.load(f)
            if 'LimitChecks' in limits:
                log.debug("  Found limit checks")
                checks = limits['LimitChecks'][0]  # TODO: more checks on items in this list!
                if 'ModelicaRecordName' in checks:
                    log.debug("  Found 'ModelicaRecordName' in limit checks")
                    hsal_results['ReqName'] = checks['ModelicaRecordName']

        # process results
        sal_output_file = os.path.abspath(os.path.join(cyphy_dir,result_prefix+"ModelResult.txt"))
        if os.path.isfile(sal_output_file):
            with open(sal_output_file,'r') as f:
                sal_output = f.read()
                if 'no counterexample' in sal_output.lower():
                    hsal_results['Result'] = 'SUCCESS'
                    log.info("  Result: no counterexample")
                elif 'counterexample' in sal_output.lower():
                    hsal_results['Result'] = 'FAIL'

                    # parse counterexample into data structure for "GroupBody":
                    group_body = []
                    # markers and collectors in loop:
                    step = None
                    transition = None
                    assigns = {}
                    information = ''            
                    # patterns for parsing:
                    stepRE = re.compile(r'Step\s+(\d+):')      # Step <N>:
                    assignRE = re.compile(r'---\s+(.+)\s+---') # --- <assignments title> ---
                    varsRE = re.compile(r'(\S+)\s+=\s+(\S+)')  # <var> = <value>
                    # loop over lines (plus a final delimiter)
                    sal_lines = sal_output.splitlines()
                    sal_lines.append('-')  # last delimiter to make loop work
                    iterator = range(len(sal_lines)).__iter__()
                    for i in iterator:
                        line = sal_lines[i]
                        # skip blank or certain lines:
                        if not line.strip() or line.startswith(('Counterexample','=','Path')):
                            log.debug("  -- skipping: {}".format(line))
                            continue
                        stepM = stepRE.match(line)
                        if stepM:
                            # parse "Step" information:
                            step = int(stepM.group(1))
                            transition = None
                            log.debug("  -- found step {}".format(step))
                            # consume next line:
                            i = next(iterator)
                            line = sal_lines[i]
                            assignM = assignRE.match(line)
                            if not assignM:
                                log.warning("  Couldn't find assignment title at {}: {}".format(i,line))
                        elif step is not None:
                            # collect assignments until reaching line that starts with "-" or end
                            varsM = varsRE.match(line)
                            if line.startswith('-'):
                                group_body.append({'Step':step, assignM.group(1):assigns})
                                assigns = {}  # reset
                                transition = step
                                step = None
                            elif varsM:
                                assigns[varsM.group(1)] = varsM.group(2)
                            else:
                                log.warning("  Couldn't find var assignment at {}: {}".format(i,line))
                        elif transition is not None:
                            if line.startswith('-'):
                                group_body.append({'Transition':'{} -> {}'.format(transition,transition+1), 'Information':information})
                                transition = None
                            elif line.startswith('Transition Information:'):
                                information = ''  # reset
                            else:
                                # collect transition information
                                information += line
                        else:
                            log.warning("  Cannot parse Counterexample line at {}: {}".format(i,line))

                    hsal_results['Details'].append({'GroupTitle': 'Counterexample generated by HybridSal with these steps and transitions','GroupBody': group_body})
                    log.info("  Result: counterexample found...\n"+sal_output)
                else:
                    log.warning("  No result found!")

        # update verification results
        mapping = dict((item['Source'], item) for item in verif_results)
        mapping['SRI'] = hsal_results  # this will replace prior entries, if present

        # write results back
        summary_results['FormalVerification'] = mapping.values()
        with open(summary_file,'w') as f:
            json.dump(summary_results,f,indent=2)
        log.info("  Summary results file written as {}".format(summary_file))
        #print json.dumps(summary_results['FormalVerification'],indent=2) # TODO: remove after testing

        # -------------------------------------------------
        print
        log.info("* Returning HybridSal exit code...")
        sys.exit(process.returncode)

    else:
        log.warning("Skipping hsalRA.exe because not in Windows!")

if __name__ == "__main__":
    main()
