
from __future__ import absolute_import
from __future__ import print_function
import sys
import os
import os.path
import json
import errno
import uuid
import subprocess
import argparse

import imp
# taken from CyPhyPython:
#  pythoncom.py calls LoadLibrary("pythoncom27.dll"), which will load via %PATH%
#  Anaconda's pythoncom27.dll (for one) doesn't include the correct SxS activation info, so trying to load it results in "An application has made an attempt to load the C runtime library incorrectly."
#  load our pythoncom27.dll (which we know works) with an explicit path
import os.path
import afxres
from six.moves import range
# FIXME: would this be better : pkg_resources.resource_filename('win32api', 'pythoncom27.dll')
imp.load_dynamic('pythoncom', os.path.join(os.path.dirname(afxres.__file__), 'pythoncom%d%d.dll' % sys.version_info[0:2]))
import pythoncom

import six.moves.winreg as winreg
with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
    meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")
sys.path.append(os.path.join(meta_path, 'bin'))
import udm


def log(s):
    print(s)


def log_formatted(s):
    print(s)


try:
    import CyPhyPython  # will fail if not running under CyPhyPython
    import cgi

    def log(s):
        CyPhyPython.log(s)

    def log_formatted(s):
        CyPhyPython.log(cgi.escape(s))
except ImportError:
    pass


def start_pdb():
    '''Start pdb, the Python debugger, in a console window.'''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    import sys
    sys.stdout = open('CONOUT$', 'wt')
    sys.stdin = open('CONIN$', 'rt')
    import pdb
    pdb.set_trace()


def test_jobmanager_running(mga_dir):
    try:
        with open(r'\\.\pipe\MetaJobManager', 'r+b', 0):
            pass
    except IOError as e:
        if e.errno != errno.ENOENT:
            raise
    else:
        return

    for jobmanager_exe in (os.path.join(meta_path, "bin", "PETBrowser.exe"), os.path.join(meta_path, "bin", "JobManager.exe"),
            os.path.join(meta_path, r"src\PETBrowser\bin\Release\PETBrowser.exe"),
            os.path.join(meta_path, r"src\JobManager\JobManager\bin\Release\JobManager.exe")):
        if os.path.isfile(jobmanager_exe):
            break
    else:
        raise ValueError('Couldnt find job manager')

    jobmanager = subprocess.Popen([jobmanager_exe], stdout=subprocess.PIPE, cwd=mga_dir)
    jobmanager.stdout.readline()


# This is the entry point
def invoke(focusObject, rootObject, componentParameters, dn=None, **kwargs):
    # output_dir = componentParameters['output_dir']
    import subprocess
    import udm
    from multiprocessing import cpu_count
    import threading
    import pythoncom
    CyPhyML = udm.map_uml_names(focusObject.type.parent)

    tb = focusObject
    if tb.type.name == 'ParametricExploration':
        tbrefs = tb.children(child_type=CyPhyML.TestBenchRef)
        if not tbrefs:
            raise ValueError('ParametricExploration must have a TestBenchRef')
        tb = tbrefs[0].ref

    tlsut = tb.children('TopLevelSystemUnderTest')
    if not tlsut:
        raise ValueError('TestBench has no TopLevelSystemUnderTest')
    tlsut = tlsut[0]
    design = tlsut.ref
    if design.type.name != 'DesignContainer':
        raise ValueError('TopLevelSystemUnderTest must refer to a DesignContainer')

    if kwargs.get('ids'):
        cwcs = [dn.get_object_by_id(udm.GmeId2UdmId(id_)) for id_ in kwargs.get('ids').split(',')]
    elif kwargs.get('configuration_id'):
        cwcs = [cwc for cwc in dn.get_object_by_id(udm.GmeId2UdmId(kwargs.get('configuration_id'))).children(child_type=CyPhyML.CWC)]
    else:
        import pdb; pdb.set_trace()
        cwcs = [cwc for config in design.children(child_type=CyPhyML.Configurations) for cwc in config.children(child_type=CyPhyML.CWC)]

    # CyPhyMasterExe.exe "MGA=C:\Users\kevin\Documents\jolt-sketch\OpenMETA\Jolt.mga" "/@Testing/@Tron Coin Changer Detection" "/@DesignSpaces/@VendingMachine/@Exported-Configurations-at--09-12--16-38-52/@cfg1"
    master_exe_path = os.path.join(meta_path, 'bin', 'CyPhyMasterExe.exe')
    if not os.path.isfile(master_exe_path):
        master_exe_path = os.path.join(meta_path, r'src\CyPhyMasterExe\bin\Release\CyPhyMasterExe.exe')
    job_collection_id = str(uuid.uuid4())
    master_exe_args = [master_exe_path,
        "--no-job-collection-done",
        "--job-collection-id", job_collection_id,
        focusObject.convert_udm2gme().Project.ProjectConnStr,
        focusObject.convert_udm2gme().AbsPath]
    cwc_paths = [cwc.convert_udm2gme().AbsPath for cwc in cwcs]
    cwc_paths.sort()
    messages = []
    lock = threading.Lock()
    messages_condition = threading.Condition()

    conn_str = focusObject.convert_udm2gme().Project.ProjectConnStr
    mga_dir = os.path.dirname(conn_str[len('MGA='):])
    results_metaresults = os.path.join(mga_dir, 'results', 'results.metaresults.json')
    if not os.path.isfile(results_metaresults):
        try:
            os.makedirs(os.path.dirname(results_metaresults))
        except OSError as exception:
            if exception.errno != errno.EEXIST:
                raise
        with open(results_metaresults, 'w') as results:
            results.write(json.dumps({"Results": []}))

    test_jobmanager_running(mga_dir=mga_dir)

    def message(msg):
        with messages_condition:
            messages.append(msg)
            messages_condition.notify()

    def run_mi():
        try:
            while True:
                with lock:
                    if len(cwc_paths) == 0:
                        return
                    cwc_path = cwc_paths.pop()
                # message(cwc_path)
                args = master_exe_args + [cwc_path]
                try:
                    process = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    stdout, stderr = process.communicate()
                    if process.wait() != 0:
                        message(stderr)
                except Exception as e:
                    message(str(e))
        finally:
            message(None)
    threads = [threading.Thread(target=run_mi) for _ in range(cpu_count())]
    for thread in threads:
        thread.start()
    threads_done = 0
    while True:
        with messages_condition:
            pythoncom.PumpWaitingMessages()
            for msg in messages:
                if msg is None:
                    threads_done = threads_done + 1
                else:
                    log(msg)
            messages[:] = []
            if threads_done == len(threads):
                break
            messages_condition.wait(1)

    # start_pdb()
    args = [master_exe_path,
        "--job-collection-id", job_collection_id,
        "--send-job-collection-done"]
    subprocess.check_call(args)
    log('Parallel Master Interpreter finished')


# Allow calling this script with a .mga file as an argument
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run MasterInterpreter in parallel')
    parser.add_argument('--ids', help='design configuration ids')
    parser.add_argument('--configuration-id', help='Configurations model id')
    parser.add_argument('filename')
    parser.add_argument('testbench_id')
    args = parser.parse_args()

    # need to open meta DN since it isn't compiled in
    uml_diagram = udm.uml_diagram()
    meta_dn = udm.SmartDataNetwork(uml_diagram)
    import os.path
    CyPhyML_udm = os.path.join(meta_path, r"generated\CyPhyML\models\CyPhyML_udm.xml")
    if not os.path.isfile(CyPhyML_udm):
        CyPhyML_udm = os.path.join(meta_path, r"meta\CyPhyML_udm.xml")
    meta_dn.open(CyPhyML_udm, "")

    dn = udm.SmartDataNetwork(meta_dn.root)
    dn.open(os.path.abspath(args.filename), "")
    focusObject = dn.get_object_by_id(udm.GmeId2UdmId(args.testbench_id))
    invoke(focusObject, dn.root, {}, **{"ids": args.ids, "configuration_id": args.configuration_id, "dn": dn})
    dn.close_no_update()
    meta_dn.close_no_update()
