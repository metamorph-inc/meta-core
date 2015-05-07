#
# USAGERIGHTS FAME
#
# Code to drive the FAME fault analysis engine from a generated testbed
#

import sys, os, re, traceback, subprocess, tempfile, shutil
from distutils.spawn import find_executable
from collections import OrderedDict

script_dir = os.path.dirname(os.path.realpath(__file__))

JARNAMES = [
    "ModelicaCompiler.jar",
    "util.jar",
    "opencsv-2.3.jar",
    "beaver.jar",
    ]

JAVA_EXE = None

def find_java():
    global JAVA_EXE
    if not (JAVA_EXE and os.path.exists(JAVA_EXE)):
        if os.environ.has_key("JAVA_HOME"):
            JAVA_EXE = os.path.join(os.environ.get("JAVA_HOME"), "bin", "java")
        else:
            JAVA_EXE = find_executable("java")
    print JAVA_EXE
    return '"' + JAVA_EXE + '"'

def fault_enable_library (library_root, other_libraries, output_directory=None, fame_cyphy_package=None):

    if not (os.path.isdir(library_root)
            or (os.path.isfile(library_root) and (os.path.splitext[1].lower() == '.mo'))):
        msg = "Specified library root '%s' invalid (non-existent or not a directory)" % (library_root,)
        sys.stderr.write(msg + "\n")
        raise RuntimeError(msg)

    # find the jar files -- should be in same directory
    directory = os.path.abspath(fame_cyphy_package or os.path.dirname(__file__))
    classpath = []
    for jarname in JARNAMES:
        fullpath = os.path.join(directory, jarname)
        if os.path.exists(fullpath):
            classpath.append(fullpath)

    # use current directory for JMODELICA_HOME
    jmodelica_home = directory

    # where to put the output files?
    if output_directory is None:
        output_directory = tempfile.mkdtemp(dir=os.path.dirname(__file__))
    if os.path.isdir(library_root):
        output_directory = os.path.join(output_directory, os.path.basename(library_root))

    # additional libraries to use
    additional_libraries = [os.path.join(directory, "MSL", "3.2")]
    for library in other_libraries:
        additional_libraries.append(os.path.dirname(os.path.abspath(library)))
    additional_libraries = list(OrderedDict.fromkeys(additional_libraries))
    additional_libraries = os.pathsep.join(additional_libraries)
    print additional_libraries

    # run the fault-enabler
    try:
        cmd = ' '.join([find_java()] + ['-Xmx4g',
               '-cp', os.pathsep.join(classpath), 'com.parc.C2M2L.FF5',
               '-faults=' + os.path.join(directory, "knownfaults.csv"),
               '-modelicapath=' + additional_libraries,
               '-outputdirectory=' + output_directory,
               '-nogui', os.path.normpath(library_root)])
        print cmd

        proc = subprocess.Popen(cmd,
                                stderr=subprocess.STDOUT,
                                shell=True,
                                env={'JMODELICA_HOME': jmodelica_home})
        status = proc.wait()
        if status != 0:
            raise subprocess.CalledProcessError(status, cmd, None)
    except subprocess.CalledProcessError, x:
        sys.stderr.write(str(x) + "\n")
        raise x

    return output_directory

def list_potential_faults_for_model (model_name, libraries_list, fame_cyphy_package=None):

    directory = os.path.abspath(os.path.dirname(__file__))

    #output_directory = tempfile.mkdtemp(dir=directory)
    output_directory = os.path.join(script_dir,'AugmentedLibraries')
    if not os.path.exists(output_directory):
        os.mkdir(output_directory)
    output_file = tempfile.mkstemp(dir=output_directory)

    fault_enabled_output_directories = []
    for library_root in libraries_list:
        other_libraries = libraries_list[:]
        other_libraries.remove(library_root)
        fault_enabled_output_directories.append(
            fault_enable_library
           (library_root, other_libraries, output_directory=output_directory, fame_cyphy_package=fame_cyphy_package))

    # print 'fault_enabled_output_directories', fault_enabled_output_directories

    # list the FAME libraries
    fame_libraries = [os.path.join(directory, "pre-faulted", "MSL", "3.2", "Modelica"),
                      os.path.join(directory, "pre-faulted", "MSL", "3.2", "ModelicaServices"),
                      os.path.join(directory, "pre-faulted", "MSL", "3.2", "Complex.mo"),
                      os.path.join(directory, "FAME"),
                      ]                       

    classpath = []
    for jarname in JARNAMES:
        fullpath = os.path.join(fame_cyphy_package or directory, jarname)
        if os.path.exists(fullpath):
            classpath.append(fullpath)

    # use current directory for JMODELICA_HOME
    jmodelica_home = directory

    # run the fault-lister
    try:
        cmd = ' '.join([find_java()] + ['-Xmx4g',
               '-cp', os.pathsep.join(classpath), 'com.parc.C2M2L.FaultsForModel',
               '-dedup', model_name]
               + fault_enabled_output_directories
               + fame_libraries)
        # print cmd
        proc = subprocess.Popen(cmd,
                                stdout=output_file[0],
                                shell=True,
                                env={'JMODELICA_HOME': jmodelica_home})
        status = proc.wait()
        if status != 0:
            raise subprocess.CalledProcessError(status, cmd, None)

        faults = open(output_file[1], "r").readlines()
        json_faults = []
        for fault in faults:
            if fault.strip().startswith('#') or not fault.strip():
                continue
            type_and_name, faultlist = fault.strip().split(':')
            typ, name = type_and_name.strip().split(" ")
            faults = [(ename, int(value)) for ename, value in [x.strip().split('=') for x in faultlist.split(",")]]
            json_faults.append({'name': name, 'type': typ, 'modes': faults})
        return json_faults
            
    except subprocess.CalledProcessError, x:
        sys.stderr.write(str(x) + "\n")
        raise x
    
    finally:
        os.close(output_file[0])
        os.remove(output_file[1])
        shutil.rmtree(os.path.join(output_directory,"CyPhy"))
        shutil.move(os.path.join(script_dir,"FAME"),os.path.join(output_directory,"FAME"))
        #target_directory = os.path.abspath(os.path.abspath(os.path.join(script_dir,'../AugmentedLibraries')))
        #print target_directory
        #shutil.move(output_directory,target_directory)
        

def fault_analyze_testbench (testbench_model_name, libraries_list, fame_cyphy_package=None):
    try:
        return list_potential_faults_for_model(testbench_model_name, libraries_list,
                                               fame_cyphy_package=fame_cyphy_package)
    except:
        fp = open("_FAILED.txt", "w")
        fp.write(traceback.format_exc())
        fp.close()
