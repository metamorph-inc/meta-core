import os
import sys
import logging
import subprocess
import json
import shutil
import glob
import re
import SAL_CE_parser


CC_EXE = "cc_modelica_hra_verifier.exe"  # name of SRI's executable
OM_SCRIPT = "model2dae.mos"  # name of generated Open Modelica script

INPUT_PATTERN = re.compile(r'\s*INPUT\s+(\S+)\s*:\s+(\S+)')  # searching for input variables and types in HSAL file


def error(msg):
    log = logging.getLogger(__name__)
    log.critical('  {} -- exiting.'.format(msg))
    sys.exit(1)


def getCCExe():
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
                        return os.path.join(value_tuple[1], "bin", CC_EXE)
    return None


def printStdOutErr(name, stdout, stderr):
    # write to new log files:
    if not os.path.isdir('log'):
        os.mkdir('log')

    if stdout:
        with open('log/{}-stdout.log'.format(name),'a') as f:
            f.write("\n  === start of {} STDOUT ===\n{}".format(name,stdout.strip()))
            f.write("\n  === end of {} STDOUT ===\n\n".format(name))
    if stderr: # not empty
        with open('log/{}-stderr.log'.format(name),'a') as f:
            f.write("\n  === start of {} STDERR ===\n{}".format(name,stderr.strip()))
            f.write("\n  === end of {} STDERR ===\n\n".format(name))


def run(xml_file1="", xml_file2="", matlab_path=""):
    cyphy_dir = os.path.normpath("CyPhy")
    log = logging.getLogger() # use global logger

    if not os.path.isfile(xml_file1):
        error("Cannot find XML file")
    xml_file1 = os.path.abspath(xml_file1)  # convert to absolute path
    if os.path.isfile(xml_file2):
        xml_file2 = os.path.abspath(xml_file2)  # convert to absolute path

    # -------------------------------------------------
    print()
    log.info(" ** Run Cyber Composition Verification on xml file(s)...")
    if xml_file2:
        log.info("    Using XML files at {} and {}".format(xml_file1, xml_file2))
    else:
        log.info("    Using XML file at {}".format(xml_file1))
    returncode = 0
    stdout = ""

    if sys.platform.startswith('win'):
        cc_exe = getCCExe()
        if not os.path.isfile(cc_exe):
            error('Couldn\'t find Cyber Composition Verification executable at\n  {}'.format(cc_exe))
        log.debug("    Found Cyber Composition Verification at {}".format(cc_exe))
        hsal_dir = os.path.dirname(cc_exe)

        log.debug("    Running Cyber Composition Verification on file {}".format(xml_file1))
        cmd = "{} {} {}".format(cc_exe,xml_file1,xml_file2).split()
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=hsal_dir)
        (stdout, stderr) = process.communicate()
        printStdOutErr(CC_EXE, stdout, stderr)
        if process.returncode: # CC created a non-zero return code
            log.warning("    Cyber Composition Verification failed with code {}".format(process.returncode))
        returncode = process.returncode
    else:
        log.warning("    Skipping {} because not in Windows!".format(CC_EXE))

    # -------------------------------------------------
    print()
    log.info(" ** Processing the result...")
    result = [] # default
    if returncode:
        # Check for missing LTL formula error:
        if "No LTL properties were provided" in stdout:
            log.warning("    No LTL properties found in file {}!".format(xml_file1))
            # add warning information to JSON result:
            result = [{"Source": "SRI",
                       "Result": "UNKNOWN",
                       "ReqName": "-unknown-",
                       "Details": [{
                           "GroupBody": ["For verifying the model, add a property in the model and redo the analysis."],
                           "GroupTitle": "WARNING: No LTL properties were provided in file {}!".format(xml_file1)}]}]
            returncode = 0  # don't signal ERROR

    elif os.path.isfile(xml_file1):
        fileName1, _ = os.path.splitext(xml_file1)

        # check if MATLAB output shall _not_ be generated:
        # 1) matlab_path = "" AND
        # 2) .mdl file(s) not present in CyPhy/Cyber/ OR
        # 3) 'where matlab.exe' (Win) 'which matlab' (UNIX) returns non-zero exit code
        output = os.path.join(os.path.dirname(xml_file1),'')  # default: perform MATLAB
        if matlab_path == "":  # only turn MATLAB off if path not explicitly given
            if len(glob.glob(os.path.join(output,'*.mdl'))) > 0:  # whether there are any MDL files in results
                cmd = ["where","matlab.exe"] if sys.platform.startswith('win') else ["which","matlab"]
                if subprocess.call(cmd):
                    output = None  # there is at least 1 MDL file but no executable found
            else:
                output = None  # no MDL files in results found
        if output == None:
            log.info("    Skipping MATLAB input file generation in case of counter example due to missing files!")

        # extracting input variables:
        input_vars = None
        if output:
            hsal_file = os.path.abspath("{}.hsal".format(os.path.splitext(xml_file1)[0]))
            if os.path.isfile(hsal_file):
                log.info("    Looking at HSAL file for input variables: {}".format(hsal_file))
                input_dict = {}
                for i, line in enumerate(open(hsal_file)):
                    for match in re.finditer(INPUT_PATTERN, line):
                        if match.group(1) not in input_dict: # only capture first occurrence
                            input_dict[match.group(1)] = match.group(2)
                log.info("    Found {} input variables with type information".format(len(input_dict)))
                # turn dict into list of tuples; TODO: if more than 2 types, need to do something fancier!
                input_vars = [tuple([n,'int32' if t == 'INTEGER' else 'double']) for n,t in input_dict.iteritems()]
                log.info("    Input variables and types are {}".format(input_vars))
            else:
                log.warning("    Couldn't find HSAL file to parse for input vars: {}".format(hsal_file))

        # parsing counter example(s):
        result_files = glob.glob("{}*Result.txt".format(fileName1))
        if len(result_files):
            result_file = result_files[0]
            if os.path.isfile(result_file):
                log.info("    Found results file at {}".format(result_file))
                try:
                    with open(result_file, 'rU') as f:
                        # make sure to use universal newline mode,
                        # as result files sometimes have mixed breaks (from Cygwin/SAL)
                        data = f.readlines()
                        result = SAL_CE_parser.parse_multiple(data,
                                                              lineNo=1, # add line numbers to output for dashboard
                                                              title="Counterexample generated by Cyber Composition Slicer and Verification",
                                                              output=output,
                                                              input_vars=input_vars,
                                                              matlab_path=matlab_path)
                except IOError as _:
                    log.exception("    Unable to read results file {}".format(result_file))
            else:
                log.warning("    No results file found at {}".format(result_file))
        else:
            log.warning("    No results files found with pattern {}".format("{}*Result.txt".format(fileName1)))

    # -------------------------------------------------
    print()
    log.info(" ** Generating dashboard output...")
    if os.path.isdir(cyphy_dir):
        # read manifest
        summary_file = os.path.join(os.path.dirname(cyphy_dir),'testbench_manifest.json')
        with open(summary_file,'r') as f:
            log.info("    Reading summary results from file {}".format(summary_file))
            summary_results = json.load(f)

        # update status
        if returncode: # CC created a non-zero return code
            summary_results['Status'] = 'FAILED'
        else:
            summary_results['Status'] = 'OK'
        if not 'FormalVerification' in summary_results:
            summary_results['FormalVerification'] = []
        verif_results = summary_results['FormalVerification']
        verif_results.extend(result) # add new results

        # write results back
        summary_results['FormalVerification'] = verif_results
        with open(summary_file,'w') as f:
            json.dump(summary_results,f,sort_keys=False,indent=2)
        log.info("    Summary results file written as {}".format(summary_file))

    else:
        log.warning("    Cannot find CyPhy dir, so printing result on screen (if log level is DEBUG)")
        log.debug('\n{}'.format(json.dumps(result,sort_keys=False,indent=2)))

    return returncode


def finish(returncode):
    log = logging.getLogger() # use global logger
    print("\n[PROGRESS] 100%\n")
    log.info("* Returning Cyber Composition Verification exit code {}...".format(returncode))
    sys.exit(returncode)


def main(xml_file1="", xml_file2="", matlab_path=""):
    cyphy_dir = os.path.normpath("CyPhy")
    print("\n[PROGRESS] 0%\n")

    # -------------------------------------------------
    # set up logging: to file only if CyPhy and the console
    log = logging.getLogger() # use global logger
    log.setLevel(logging.DEBUG)

    if os.path.isdir(cyphy_dir):
        if not os.path.isdir('log'):
            os.mkdir('log')
        # set up logging to file, which logs debug messages
        fh = logging.FileHandler('log/CyberComp_runner.log',mode='w')
        fh.setLevel(logging.DEBUG)
        fh.setFormatter(logging.Formatter(fmt='%(asctime)s %(levelname)-8s (%(name)s): %(message)s'))
        log.addHandler(fh)

    # define a handler which writes INFO messages or higher to the sys.stderr
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    # tell the handler to use simpler format
    console.setFormatter(logging.Formatter(fmt='%(levelname)-8s (%(name)s): %(message)s'))
    # add the handler to the main logger
    log.addHandler(console)

    # -------------------------------------------------
    if not xml_file1 or not xml_file2:
        # locate CyPhy/Cyber:
        if not os.path.isdir(cyphy_dir):
            error('Couldn\'t find directory {}'.format(cyphy_dir))

        # find map JSON file:
        mapfile = os.path.join(cyphy_dir,'modelicaURI2CyPhyMap.json')
        if not os.path.isfile(mapfile):
            error('Couldn\'t find map file {}'.format(mapfile))

        # create plant XML file in CyPhy/Cyber:
        log.info("* Creating plant xml file using OpenModelica...")
        # read model config data
        model_config = os.path.normpath(os.path.join(cyphy_dir,"model_config.json"))
        if not os.path.isfile(model_config):
            error('Couldn\'t find file {}'.format(model_config))
        with open(model_config,'r') as f:
            data = json.load(f)
        log.info("  Read configuration from file {}".format(model_config))

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
                om_std_lib = ""
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
        result_prefix = model_name.split('.')[len(model_name.split('.')) - 1]  # use only last one
        om_script_path = os.path.join(cyphy_dir,OM_SCRIPT)
        with open(om_script_path,'w') as f:
            print('''
loadModel(Modelica, {{"{}"}});
loadModel(ModelicaServices); '''.format(data["MSL_version"]), file=f)
            for name in data["lib_package_names"]:
                f.write('loadModel({}); \n'.format(name))
            print('''
loadFile("{}");
setCommandLineOptions("+debug=failtrace");
setCommandLineOptions("+showErrorMessages");
setCommandLineOptions("+indexReductionMethod=dynamicStateSelection" );
dumpXMLDAE({},"optimiser",addMathMLCode=true,fileNamePrefix="{}");
print(getErrorString());'''.format(data["model_file_name"],model_name,result_prefix), file=f)
        log.info("  Written Open Modelica script to file {}".format(om_script_path))

        xml_file2 = os.path.abspath(os.path.join(cyphy_dir,result_prefix+".xml"))
        if sys.platform.startswith('win'):  # windows
            if not os.path.isfile(xml_file2):
                omc_path = os.path.normpath(os.path.join(os.environ["OPENMODELICAHOME"], "bin", "omc.exe"))
                if not os.path.isfile(omc_path):
                    error('Couldn\'t find Open Modelica compiler {}'.format(omc_path))
                cmd = "{} {}".format(omc_path,OM_SCRIPT).split()
                process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=cyphy_dir)
                (stdout, stderr) = process.communicate()
                printStdOutErr("omc.exe", stdout, stderr)
                # abort if omc.exe throws error!
                if process.returncode:
                    log.error("  Calling 'omc.exe' threw an error: {} -- exiting!".format(process.returncode))
                    sys.exit(process.returncode)
                else:
                    log.debug("  Called 'omc.exe' with script {}".format(os.path.join(cyphy_dir,OM_SCRIPT)))
                    log.debug("  This should have created file {}".format(xml_file2))
            else:
                log.warning("  Skipping Open Modelica compilation as DAE XML file already present: {}".format(xml_file2))
        else:
            log.warning("  Skipping omc.exe because not in Windows!")
        print("\n[PROGRESS] 50%")
        if not os.path.isfile(xml_file2):
            error('Plant XML file {} did not get generated!'.format(xml_file2))

        # go through all controller .cyber.json files in CyPhy/ to find the other XML file(s):
        print()
        log.info("* Identifying component(s) for verification...")
        returncode = 200  # initialize with non-zero in case no matching XML files found!
        json_files = glob.glob(os.path.join(cyphy_dir,'*.cyber.json'))
        if not len(json_files):
            log.warning("  Could not find any components (files ending with .cyber.json) -- falling back to single argument!")
            returncode = run(xml_file2, '', matlab_path)  # this invokes the legacy functionality
            finish(returncode)  # this method does not return!
        progress_increment = 50/len(json_files)  # how much to increment each time
        progress = 50  # keep track of progress
        for name in json_files:
            print()
            log.info(" ** Found: {}".format(name))
            # open file for reading:
            with open(name, 'r') as f:
                cyber_results = json.load(f)
            xml_file1 = ''
            if not "cyberxml.path" in cyber_results:
                #error('Couldn\'t load "cyberxml.path" from JSON file {}'.format(name))
                # TODO: work-around while Joe is out: go through all values and find first ".xml"
                for v in cyber_results.itervalues():
                    if v.endswith('.xml'):
                        xml_file1 = os.path.join(cyphy_dir, v)
            else:
                xml_file1 = os.path.join(cyphy_dir, cyber_results["cyberxml.path"] if sys.platform.startswith('win')
                    else cyber_results["cyberxml.path"].replace('\\', '/'))
            if not os.path.isfile(xml_file1):
                error('Couldn\'t load XML file name from JSON file {}'.format(name))
            log.info("    XML path: {}".format(xml_file1))
            # copy map file if not already present:
            if not os.path.basename(mapfile) in os.listdir(os.path.dirname(xml_file1)):
                shutil.copy(mapfile, os.path.dirname(xml_file1))
                log.info("    Copied map file from {}".format(mapfile))
            returncode = run(xml_file1, xml_file2, matlab_path)
            progress = progress + progress_increment
            print(("\n[PROGRESS] {0:d}%".format(progress)))
            if returncode:  # stop and show warning
                log.error("    Run with component in file {} did NOT succeed -- stopping!".format(xml_file1))
                break
    else:
        print("\n[PROGRESS] 50%")
        returncode = run(xml_file1, xml_file2, matlab_path)

    finish(returncode)


if __name__ == "__main__":
    xml_file1 = ""
    xml_file2 = ""
    matlab_path = ""
    if len(sys.argv) == 2 or len(sys.argv) == 4:
        matlab_path = sys.argv[-1]  # last element contains MATLAB path
    if len(sys.argv) > 2:
        xml_file1 = sys.argv[1]
        xml_file2 = sys.argv[2]
    main(xml_file1, xml_file2, matlab_path)
