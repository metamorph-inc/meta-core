#
# USAGERIGHTS FAME
#
# Code to drive the FAME fault analysis engine from a generated testbed
#

import sys, os, re, traceback, subprocess, tempfile, shutil
from distutils.spawn import find_executable

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
        if os.environ.has_key("JAVA_HOME") and os.path.exists(os.path.join(os.environ.get("JAVA_HOME", "bin"))):
            JAVA_EXE = find_executable("java", path=os.path.join(os.environ.get("JAVA_HOME", "bin")))
        else:
            JAVA_EXE = find_executable("java")
    return JAVA_EXE

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
    additional_libraries = os.pathsep.join(additional_libraries)

    # run the fault-enabler
    try:
        cmd = ' '.join([find_java()] + ['-Xmx1g',
               '-cp', os.pathsep.join(classpath), 'com.parc.C2M2L.FF5',
               '-faults=' + os.path.join(directory, "knownfaults.csv"),
               '-modelicapath=' + additional_libraries,
               '-outputdirectory=' + output_directory,
               '-nogui', os.path.normpath(library_root)])
        # print cmd
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

    output_directory = tempfile.mkdtemp(dir=directory)
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
    fame_libraries = [os.path.join(fame_cyphy_package or directory, "pre-faulted", "MSL", "3.2", "Modelica"),
                      os.path.join(fame_cyphy_package or directory, "pre-faulted", "MSL", "3.2", "ModelicaServices"),
                      os.path.join(fame_cyphy_package or directory, "pre-faulted", "MSL", "3.2", "Complex.mo"),
                      os.path.join(fame_cyphy_package or directory, "FAME"),
                      ]                       

    classpath = []
    for jarname in JARNAMES:
        fullpath = os.path.join(fame_cyphy_package or directory, jarname)
        if os.path.exists(fullpath):
            classpath.append(fullpath)

    # use current directory for JMODELICA_HOME
    jmodelica_home = fame_cyphy_package or directory

    # run the fault-lister
    try:
        cmd = ' '.join([find_java()] + ['-Xmx1g',
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
        shutil.rmtree(output_directory)

def fault_analyze_testbench (testbench_model_name, libraries_list, fame_cyphy_package=None):
    try:
        return list_potential_faults_for_model(testbench_model_name, libraries_list,
                                               fame_cyphy_package=fame_cyphy_package)
        sys.stdout.write("Fault analysis for model '" + testbench_model_name + "' was successful.\n");
    except:
        msg = traceback.format_exc()
        fp = open("_FAILED.txt", "w")
        fp.write(msg)
        fp.close()
        sys.stderr.write(msg)
