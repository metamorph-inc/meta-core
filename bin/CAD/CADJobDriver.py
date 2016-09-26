import sys
import os
import argparse
import subprocess
import random
import string
import cad_library

outputcmds = True


def runCreoAssembler():
    isisext = os.environ.get('PROE_ISIS_EXTENSIONS')
    if isisext is None:
        cad_library.exitwitherror('PROE_ISIS_EXTENSIONS env. variable is not set. Do you have the META toolchain installed properly?', -1)
    createasm = os.path.join(isisext, 'bin', 'CADCreoParametricCreateAssembly.exe')
    if not os.path.isfile(createasm):
        cad_library.exitwitherror('Can\'t find CADCreoParametricCreateAssembly.exe. Do you have the META toolchain installed properly?', -1)
    # logdir = os.path.join(workdir,'log')
    result = os.system('\"' + createasm + '" -i CADAssembly.xml')
    return result


def callsubprocess(cmd, failonexit=True):
    global outputcmds
    if outputcmds is True:
        print cmd
    try:
        result = subprocess.call(cmd)
    except Exception as e:
        cad_library.exitwitherror('Failed to execute: ' + cmd + ' Error is: ' + e.message, -1)
    if result != 0 and failonexit:
        cad_library.exitwitherror('The command ' + cmd + ' exited with value: ' + result, -1)


def copyfailedandexit(code):
    for (root, dirs, files) in os.walk(os.getcwd(), topdown=False):
        for file in files:
            print os.path.join(root, file)
            if cmp(file, '_FAILED.txt') == 0:
                # print 'copy ' + os.path.join(root, file) +' '+ os.getcwd()
                os.system('copy ' + os.path.join(root, file) + ' ' + os.getcwd())

    exit(code)


def runAbaqusModelBased(meshonly, modelcheck, mode=None):
    feascript = cad_library.META_PATH + 'bin\\CAD\\Abaqus\\AbaqusMain.py'
    if meshonly:
        if modelcheck:
            param = '-b'
        else:
            param = '-o'
    else:
        if mode == 'STATIC':
            param = '-s'
        elif mode == 'MODAL':
            param = '-m'
        elif mode == 'DYNIMPL':
            param = '-i'
        elif mode == 'DYNEXPL':
            param = '-e'

    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat cae noGUI="' + feascript + '" -- ' + param)


def runAbaqusDeckBased():
    id = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(6))
    os.chdir(os.getcwd() + '\\Analysis\\Abaqus')
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat fromnastran job=' + id + ' input=..\Nastran_mod.nas')
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat analysis interactive job=' + id)
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat odbreport job=' + id + ' results')
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat cae noGUI="' + cad_library.META_PATH + '\\bin\\CAD\\ABQ_CompletePostProcess.py\" -- -o ' + id + '.odb -p ..\\AnalysisMetaData.xml -m ..\\..\\RequestedMetrics.xml -j ..\\..\\testbench_manifest.json')


def runNastran():
    os.chdir(os.getcwd() + '\\Analysis\\Nastran')
    callsubprocess(sys.executable + ' \"' + cad_library.META_PATH + 'bin\\CAD\Nastran.py\" ..\\Nastran_mod.nas')
    # if result != 0:
    #     cad_library.exitwitherror('CADJobDriver.py: Nastran.py returned with code: ' + str(result),-1)
    patranscript = cad_library.META_PATH + 'bin\\CAD\\Patran_PP.py'
    if not os.path.isfile(patranscript):
        cad_library.exitwitherror('Can\'t find ' + patranscript + '. Do you have the META toolchain installed properly?', -1)
    callsubprocess(sys.executable + ' \"' + patranscript + '\" ..\\Nastran_mod.nas Nastran_mod.xdb ..\\AnalysisMetaData.xml ..\\..\\RequestedMetrics.xml ..\\..\\testbench_manifest.json')
    # if result != 0:
    #     cad_library.exitwitherror('CADJobDriver.py: Patran_PP.py returned with code: ' + str(result),-1)


def runCalculix():
    import _winreg
    isisext = os.environ['PROE_ISIS_EXTENSIONS']
    os.chdir(os.getcwd() + "\\Analysis\\Calculix")
    if isisext is None:
        cad_library.exitwitherror('PROE_ISIS_EXTENSIONS env. variable is not set. Do you have the META toolchain installed properly?', -1)
    deckconvexe = os.path.join(isisext, 'bin', 'DeckConverter.exe')
    callsubprocess(deckconvexe + ' -i ..\\Nastran_mod.nas')
    with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\CMS\CalculiX', 0,
                     _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
        bconvergedpath = _winreg.QueryValueEx(key, 'InstallLocation')[0]
    callsubprocess(bconvergedpath+'\\CalculiX\\bin\\ccx.bat -i ..\\Nastran_mod')
    metapython = os.path.join(cad_library.META_PATH, 'bin', 'Python27', 'Scripts', 'python.exe')
    calculix_pp = os.path.join(cad_library.META_PATH, 'bin', 'CAD', 'ProcessCalculix.py')
    callsubprocess(metapython + " " + calculix_pp + " -o ..\\Nastran_mod.frd -p ..\\AnalysisMetaData.xml -m ..\\..\\RequestedMetrics.xml -j ..\\..\\testbench_manifest.json -e PSolid_Element_Map.csv")


def main():
    global args
    parser = argparse.ArgumentParser(description='Executes a CAD or FEA job. Invokes the specified assembler, mesher and analyzer in this sequence.')
    parser.add_argument('-assembler', choices=['CREO'])
    parser.add_argument('-mesher', choices=['NONE', 'CREO', 'ABAQUS', 'PATRAN', 'ABAQUSMDLCHECK', 'GMESH'])
    parser.add_argument('-analyzer', choices=['NONE', 'ABAQUSMODEL', 'ABAQUSDECK', 'NASTRAN', 'CALCULIX'])
    parser.add_argument('-mode', choices=['STATIC', 'MODAL', 'DYNIMPL', 'DYNEXPL'])
    args = parser.parse_args()

    assembler = args.assembler

    if assembler is None:
        assembler = 'CREO'
        print 'No assembler speicifed, defaulting to ' + assembler

    mesher = args.mesher

    if mesher is None:
        mesher = 'NONE'
        print 'No mesher was specified, defaulting to ' + mesher

    analyzer = args.analyzer

    if analyzer is None:
        analyzer = 'NONE'
        print 'No analyzer was specified, defaulting to ' + analyzer

    mode = args.mode

    if mode is None:
        mode = 'STATIC'
        print 'No mode was specified, defaulting to ' + mode

    # Run assembler
    if assembler == 'CREO':
        result = runCreoAssembler()
        if result != 0:
            cad_library.exitwitherror('CADJobDriver.py: The CreateAssembly program returned with an error:' + str(result), -1)
    else:
        cad_library.exitwitherror('CADJobDriver.py: Only CREO assembler is supported.', -1)

    # Run mesher
    if mesher == 'ABAQUS' or mesher == 'ABAQUSMDLCHECK':
        if analyzer == 'NONE':
            runAbaqusModelBased(True, mesher == 'ABAQUSMDLCHECK')
        elif analyzer == 'ABAQUSMODEL':
            runAbaqusModelBased(False, False, mode)
        else:
            cad_library.exitwitherror('Abaqus mesher only supports Abaqus Model-Based.', -1)
    elif mesher == 'PATRAN':
        cad_library.exitwitherror('CADJobDriver.py: PATRAN mesher is not supported yet', -1)
    elif mesher == 'CREO':
        # Skipping, CREO has already been invoked
        pass
    elif mesher == 'NONE':
        # Not meshing, skip analysis
        copyfailedandexit(0)
    else:
        cad_library.exitwitherror('CADJobDriver.py: Mesher ' + mesher + ' is not supported.', -1)

    # Run analyzer
    if analyzer == 'ABAQUSMODEL':
        pass
        if mesher == 'ABAQUS' or mesher == 'ABAQUSMDLCHECK':
            # Skip this, it has already been executed in teh previous section
            pass
        else:
            runAbaqusModelBased(False, False, mode)
    elif analyzer == 'ABAQUSDECK':
        runAbaqusDeckBased()
    elif analyzer == 'NASTRAN':
        runNastran()
    elif analyzer == 'CALCULIX':
        runCalculix()

    copyfailedandexit(0)

if __name__ == '__main__':
    import win32api
    import win32job
    hProcess = win32api.GetCurrentProcess()
    if not win32job.IsProcessInJob(hProcess, None):
        hJob = win32job.CreateJobObject(None, "")
        extended_info = win32job.QueryInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation)
        extended_info['BasicLimitInformation']['LimitFlags'] = win32job.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | win32job.JOB_OBJECT_LIMIT_BREAKAWAY_OK
        win32job.SetInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation, extended_info)
        win32job.AssignProcessToJobObject(hJob, hProcess)
    main()
