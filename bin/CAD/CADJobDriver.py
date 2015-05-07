import math, re, sys, json, os
import argparse
import _winreg
import subprocess
import random
import string

outputcmds = True

with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
    META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]

def exitwitherror(msg,code):
    f = open('_FAILED.txt','a')
    f.write(msg + '\n') # python will convert \n to os.linesep
    f.close() # you can omit in most cases as the destructor will call if
    exit(code)

def runCreoAssembler():
    isisext = os.environ['PROE_ISIS_EXTENSIONS']
    if isisext is None:
        exitwitherror ('PROE_ISIS_EXTENSIONS env. variable is not set. Do you have the META toolchain installed properly?', -1)
    createasm = os.path.join(isisext,'bin','CADCreoParametricCreateAssembly.exe')
    if not os.path.isfile(createasm):
        exitwitherror ('Can\'t find CADCreoParametricCreateAssembly.exe. Do you have the META toolchain installed properly?', -1)
    #logdir = os.path.join(workdir,'log')
    result = os.system('\"' + createasm + '" -i CADAssembly.xml')
    return result

def callsubprocess(cmd):
    global outputcmds
    if outputcmds == True:
        print cmd
    return subprocess.call(cmd)

def runAbaqusModelBased(meshonly, modelcheck, mode=None):
    feascript = META_PATH + 'bin\\CAD\\Abaqus\\AbaqusMain.py'
    if meshonly:
        if modelcheck:
            param = '-b'
        else:
            param= '-o'
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
    pass

def runAbaqusDeckBased():
    id = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(6))
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat fromnastran job=' + id + ' input=Analysis\Nastran_mod.nas')
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat analysis interactive job=' + id)
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat odbreport job=' + id + ' results')
    callsubprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat cae noGUI="' + META_PATH + '\\bin\\CAD\\ABQ_CompletePostProcess.py\" -- -o ' + id + '.odb  -p Analysis\\Abaqus\\Abaqus.xml')

def runNastran():
    os.chdir(os.getcwd() + '\\Analysis\\Nastran')
    result = callsubprocess(sys.executable + ' \"' + META_PATH + 'bin\\CAD\Nastran.py\" ..\\Nastran_mod.nas')
    if result != 0:
        exitwitherror('CADJobDriver.py: Nastran.py returned with code: ' + str(result),-1)
    patranscript = META_PATH + 'bin\\CAD\\Patran_PP.py'
    if not os.path.isfile(patranscript):
        exitwitherror ('Can\'t find ' + patranscript + '. Do you have the META toolchain installed properly?', -1)
    result = callsubprocess(sys.executable + ' \"' + patranscript + '\" ..\\Nastran_mod.nas Nastran_mod.xdb ..\\AnalysisMetaData.xml ..\\..\\RequestedMetrics.xml ..\\..\\testbench_manifest.json')
    if result != 0:
        exitwitherror('CADJobDriver.py: Patran_PP.py returned with code: ' + str(result),-1)

def main():
    global args
    parser = argparse.ArgumentParser(description='Executes a CAD or FEA job. Invokes the specified assembler, mesher and analyzer in this sequence.')
    parser.add_argument('-assembler', choices=['CREO']);
    parser.add_argument('-mesher', choices=['NONE','CREO','ABAQUS','PATRAN','ABAQUSMDLCHECK','GMESH']);
    parser.add_argument('-analyzer', choices=['NONE','ABAQUSMODEL','ABAQUSDECK','NASTRAN']);
    parser.add_argument('-mode', choices=['STATIC','MODAL','DYNIMPL','DYNEXPL']);
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
    if assembler=='CREO':
        result = runCreoAssembler()
        if result == -1:
            exitwitherror('CADJobDriver.py: The CreateAssembly program returned with an error:' + str(result), -1)
    else:
        exitwitherror('CADJobDriver.py: Only CREO assembler is supported.', -1)

    # Run mesher
    if mesher == 'ABAQUS' or mesher == 'ABAQUSMDLCHECK':
        if analyzer == 'NONE':
            runAbaqusModelBased(True, mesher == 'ABAQUSMDLCHECK')
        elif analyzer == 'ABAQUSMODEL':
            runAbaqusModelBased(False, False, mode)
        else:
            exitwitherror('Abaqus mesher only supports Abaqus Model-Based.',-1)
    elif mesher == 'PATRAN':
        exitwitherror('CADJobDriver.py: PATRAN mesher is not supported yet', -1)
    elif mesher == 'CREO':
        # Skipping, CREO has already been invoked
        pass
    elif mesher == 'NONE':
        # Not meshing, skip analysis
        exit(0)
    else:
        exitwitherror('CADJobDriver.py: Mesher ' + mesher + ' is not supported.', -1)

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

if __name__ == '__main__':
    main()
