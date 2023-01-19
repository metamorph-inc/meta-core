
import sys
import os
import subprocess
import shutil
import argparse

files_modelica = '''
./modelica2hsal
./modelica2hsal/bin
./modelica2hsal/bin/modelica2hsal
./modelica2hsal/src
./modelica2hsal/src/daexml2hsal.py
./modelica2hsal/src/modelica2sal.py
./modelica2hsal/src/hsalRA.py
./modelica2hsal/src/HSalXMLPP.py
./modelica2hsal/src/daeXML.py
./modelica2hsal/src/modelica2hsal.py
./modelica2hsal/src/ddae.py
./modelica2hsal/src/daexmlPP.py
./modelica2hsal/src/ModelicaXML.py
./modelica2hsal/README
./modelica2hsal/examples
./modelica2hsal/examples/Torque_Converter2.Torque_Converter2.xml
./modelica2hsal/examples/context-property.xml
./modelica2hsal/examples/MassSpringDamperTest.MassSpringDamperTest.xml
./modelica2hsal/examples/C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Drive_Line.Power_Take_Off_Module.test_PTM_TC_only_Simple.xml
./modelica2hsal/examples/RCEngine.xml
./modelica2hsal/examples/RCEngine.mo
./modelica2hsal/examples/C2M2L_Ext.C2M2L_Delivered_Component_Implementations.Drive_Line.Transfer_Case.test_transfer_case_No3D.xml
./modelica2hsal/examples/engine1a.xml
./modelica2hsal/examples/MassSpringDamperTest.MassSpringDamperTest_init.xml
./modelica2hsal/examples/MassSpringDamperTest.property.json
./modelica2hsal/examples/Transfer_Case2.Transfer_Case2.xml
./bin/modelica2hsal
./bin/modelica2sal
'''

files_hsal = '''
./bin
./bin/hsal2Tsal
./bin/hybridsal2xml.template
./bin/hasal2sal
./bin/hsal2hasal
./bin/hxml2hsal
./bin/hsal2hxml
./COPYRIGHT
./INSTALL.txt
./doc
./doc/hybridsal-tutorial.pdf
./doc/manpage-hxml2hsal.txt
./doc/manpage-hasal2sal.txt
./doc/manpage-hsal2hasal.txt
./doc/manpage-hybridsal2xml.txt
./doc/hsal-ra.pdf
./doc/hsal-ra-report.pdf
./doc/relAbs-tutorial.pdf
./install.py
./src
./src/HSalCegar.py
./src/HSalPreProcess2.py
./src/linearAlgebra.py
./src/HSalExtractRelAbs.py
./src/polyrep.py
./src/linAlDevel.py
./src/HSalXMLPP.py
./src/HSalRelAbsCons.py
./src/xmlHelpers.py
./src/polyrep2XML.py
./src/HSalPreProcess.py
./src/Makefile
./src/HSalTimedRelAbsCons.py
./src/HSalTimeAwareAux.py
./hybridsal2xml
./hybridsal2xml/hybridsal2xml.template
./hybridsal2xml/HybridSalParser.g.bkp
./hybridsal2xml/XmlAst.java
./hybridsal2xml/Indent.java
./hybridsal2xml/HybridSalParser.g
./hybridsal2xml/hybridsal2xml
./hybridsal2xml/HybridSalTokenTypes.txt
./hybridsal2xml/Makefile.in
./hybridsal2xml/HybridSal2Xml.mine
./hybridsal2xml/README
./hybridsal2xml/HybridSalTokenTypes.java
./hybridsal2xml/HybridSal2Xml.java
./hybridsal2xml/Makefile
./hybridsal2xml/examples
./hybridsal2xml/HybridSalParser.java
./hybridsal2xml/HybridSalLexer.java
./hybridsal2xml/hybridsal2xml.jar
./hybridsal2xml/install.sh
./README
./Makefile
./examples
./examples/nav10.hsal
./examples/nav.template.hsal
./examples/Linear3.hsal
./examples/SimpleThermo4.hsal
./examples/nav26.hsal
./examples/nav30.hsal
./examples/nav01.hsal
./examples/nav21.hsal
./examples/nav14.hsal
./examples/powertrainSimple.hsal
./examples/LinearRepeated.hsal
./examples/nav3.hsal
./examples/Linear1.hsal
./examples/PITimed.hsal
./examples/nav2.hsal
./examples/Linear6.hsal
./examples/nav28.hsal
./examples/drivetrain2.hsal
./examples/nav4.hsal
./examples/drivetrain.hsal
./examples/Linear8.hsal
./examples/RLC.hsal
./examples/TGC.hsal
./examples/nav11.hsal
./examples/nav24.hsal
./examples/nav13.hsal
./examples/PISatTimed.hsal
./examples/PTimed.hsal
./examples/nav12.hsal
./examples/nav29.hsal
./examples/Linear5.hsal
./examples/Linear7.hsal
./examples/nav8.hsal
./examples/nav6.hsal
./examples/nav7.hsal
./examples/Linear4.hsal
./examples/nav20.hsal
./examples/nav22.hsal
./examples/Linear2.hsal
./examples/nav1.hsal
./examples/nav25.hsal
./examples/CAC_FlowControl_CyPhy.hsal
./examples/InvPenTimed.hsal
./examples/nav18.hsal
./examples/nav17.hsal
./examples/SimpleThermo5.hsal
./examples/robot.hsal
./examples/powertrain.hsal
./examples/nav.hsal
./examples/robotnav.hsal
./examples/nav27.hsal
./examples/drivetrain3.hsal
./examples/nav9.hsal
./examples/nav01b.hsal
./examples/nav5.hsal
./examples/nav23.hsal
./examples/nav16.hsal
./examples/nav19.hsal
./examples/nav15.hsal
./examples/Heating.hsal
'''

files = files_hsal + files_modelica

def which(program):
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath,os.X_OK)
    fpath,fname = os.path.split(program)
    if fpath:
        if is_exe(program):    # check in current working directory
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return None

def checkProg(name):
    """Check if name is installed on this machine"""
    #proc = subprocess.Popen(['which', name], stdout=subprocess.PIPE)
    #output = proc.stdout.read()
    #if output == '' or output == '\n':
    output = which(name) or which(name + '.exe')
    if not output:
        #print 'Warning: %s not found...trying...' % name
        return False
    return output

    
def isFile(filename):
    tmp = os.path.normpath(filename)
    return os.path.isfile(tmp)

def checkFile(filename):
    if not os.path.isfile(filename):
        print('Failed.')
        print('***ERROR: File {0} not found'.format(filename))
        print('***Download of HybridSal .tgz was incomplete???')
        print('***Aborting...')
        sys.exit(1)
    return True

def findFile(baseList, dirList, fileList):
    """See if you can find rt.jar in any directory in baseList"""
    # On Macs: /System/Library/Frameworks/JavaVM.framework/Versions/1.4.1/Classes/classes.jar
    done = False
    for i in fileList:
        for k in baseList:
            for j in dirList:
                rtjar = os.path.join(k, j, i)
                if isFile(rtjar):
                    done = True
                    break
            if done:
                break
        if done:
            break
    if done:
        rtjar = os.path.normpath(rtjar)
    else:
        rtjar = None
    return rtjar

def sed(f1, f2, assgn):
    fp1 = open(f1, 'r')
    fp2 = open(f2, 'w')
    for line in fp1:
        for (s1,s2) in assgn:
            line = line.replace(s1,s2)
        fp2.write(line)
    fp1.close()
    fp2.close()

def searchForrtjar( arg_rtjar, output ):
    global parser
    print('Searching for rt.jar...', end=' ')
    rtjar = 'rt.jar'
    if arg_rtjar != None:
        rtjar = arg_rtjar
    if not(isFile(rtjar)):
        javapath = os.path.realpath(output)
        (javabase, javafile) = os.path.split(javapath)
        baseList = [javabase]
        forMacs = os.path.join(os.path.sep, 'System', 'Library', 'Frameworks', 'JavaVM', 'framework', 'Versions', 'CurrentJDK', 'Classes')
        #baseList.append('/System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Classes')
        baseList.append(forMacs)
        forWindows = os.path.join('C:',os.path.sep,'Program Files','Java','jre6','lib')
        baseList.append(forWindows)
        if os.environ.has_key('JAVA_HOME'):
            baseList.append( os.environ['JAVA_HOME'] )
        if os.environ.has_key('JDK_HOME'):
            baseList.append( os.environ['JDK_HOME'] )
        p1 = os.path.join('..','lib')
        p2 = os.path.join('..','Classes')
        rtjar = findFile(baseList, [p1, p2], ['rt.jar', 'classes.jar'])
    if rtjar == None:
        print('Failed.')
        print('***Warning: Failed to find rt.jar in all possible places')
        print('***Continuing without giving explicit rt.jar path; if this does not work, then...Make sure the system has rt.jar')
        print('***Mac: it is sometimes called classes.jar and is located at /System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Classes/classes.jar)')
        print('***Win: often located at C:\Program Files\Java\jre6\lib....Once you have rt.jar...')
        print('***Rerun install script as: {0}'.format( parser.format_usage() ))
        rtjar = '.'
    else:
        print('Successful. Found {0}'.format(rtjar))
    rtjar = os.path.abspath(rtjar)
    if rtjar.find('java-6-openjdk') >= 0:
        print('****Warning: rt.jar in java-6-openjdk is buggy; use java-6-sun/jre instead****')
        print('****Rerun install script as: {0}'.format( parser.format_usage() ))
    return rtjar

def searchForjikes():
    #
    # Search for jikes? 
    # NOTE: BD I don't have jikes but it still compile fine??
    #
    print('Searching for jikes or javac...', end=' ')
    jikespath = ''
    output = checkProg('javac')
    if not output:
        # print 'Warning: jikes not found; Trying to find javac ...'
        jikespath = ''
    else:
        jikespath = os.path.realpath(output)
    if jikespath == '':
        output = checkProg('jikes')
        if not output:
            pass
        else:
            jikespath = os.path.realpath(output)
    if jikespath == '':
        print('Failed.')
        print('***Warning: jikes/javac not found; You can not COMPILE hybridsal2xml')
        print('*** But you may be able to use the class files already present')
        print('*** The class files were built on {0} {1}'.format(sys.platform,sys.version))
        print('***Optionally, you can try to install jikes/javac and rerun this script')
        #sourceforge.net/projects/jikes/files/Jikes/1.22/jikes-1.22-1.windows.zip
        jikespath = 'javac'
    else:
        print('Successful. Found {0}'.format(jikespath))
    return jikespath

def create_hybridsal2xml_exe( pwd, shell, jarfile ):
    # create file hybridsal2xml
    bindir = os.path.join(pwd, "bin")
    hybridsal2xml = os.path.join(bindir, 'hybridsal2xml')
    hybridsal2xmltemplate = os.path.join(bindir, 'hybridsal2xml.template')
    if not(os.path.isdir(bindir)) or not os.path.isfile(hybridsal2xmltemplate):
        return # os.makedirs(bindir)
    print("Creating script hybridsal2xml at {0}...".format(bindir), end=' ')
    scriptArgs = ''
    topshell = ''
    if sys.platform.startswith('win'):  # windows
        classpathsep = ';'
        scriptArgs = '%1 %2 %3'
        topshell = ''
    else:       # linux or mac
        classpathsep = ':'
        scriptArgs = '$*'
        topshell = '#!{0}'.format(shell)
    if sys.platform.startswith('win'):
        if os.path.isfile(hybridsal2xml):
            os.remove(hybridsal2xml)
        hybridsal2xml += '.bat'
    assgn = [ ("__HYBRIDSAL2XMLJAR__", os.path.abspath(jarfile)), ("__ARGS__", scriptArgs), ("__SH__",topshell) ]
    sed( hybridsal2xmltemplate, hybridsal2xml, assgn )
    os.chmod( hybridsal2xml, 0775 )
    hybridsal2xml1 = os.path.join( pwd, 'hybridsal2xml' )
    shutil.copy( hybridsal2xml, hybridsal2xml1 )
    print("Created script {0}; also copied in directory {1}.".format(hybridsal2xml, hybridsal2xml1))
    return hybridsal2xml

def compile_hybridsal2xml( hybridsal2xml, shell, rtjar, jikespath ):
    #
    # Run the install.sh script in hybridsal2xml
    #
    os.chdir(hybridsal2xml)
    # run ./install.sh antlrpath rtjar jikespath
    print('Searching for antlr...', end=' ')
    antlrpath = os.path.join(hybridsal2xml, 'antlr-2.7.1')
    if os.path.isdir(antlrpath):
        print('Successful. Found {0}'.format(antlrpath))
    else:
        print('Failed. Failed to find antlr-2.7.1/')
        return 1
    # subprocess.call(['sh', 'install.sh', antlrpath, rtjar, jikespath ])
    print("Installing hybridsal2xml at {0}".format(os.getcwd()))
    scriptArgs = ''
    topshell = ''
    iswin = sys.platform.startswith('win')	# windows
    if iswin:	# windows
        classpathsep = ';'
        scriptArgs = '%1 %2 %3'
        topshell = ''
    else:	# linux or mac
        classpathsep = ':'
        scriptArgs = '$*'
        topshell = '#!{0}'.format(shell)
    javaclasspath = classpathsep.join([ '.', antlrpath, hybridsal2xml, os.path.join(antlrpath, 'antlr'), os.path.join(hybridsal2xml, 'antlr'), rtjar ])
    if 'CLASSPATH' in os.environ.keys():
        javaclasspath = classpathsep.join([ javaclasspath, os.environ['CLASSPATH']])
    tmp = '"{0}"'.format(javaclasspath)
    assgn = [ ("__ANTLR_PATH__", antlrpath), ("__JIKES_PATH__",repr(jikespath)), ("__RTJAR_PATH__",rtjar), ("__JAVACLASSPATH__",tmp) ]
    sed( 'Makefile.in', 'Makefile', assgn)
    javaclasspath = classpathsep.join([ hybridsal2xml, javaclasspath])
    tmp = '"{0}"'.format(javaclasspath)
    assgn = [ ("__JAVACLASSPATH__", tmp), ("__ARGS__", scriptArgs), ("__SH__",topshell) ]
    # hybridsal2xmltemplate = os.path.join('hybridsal2xml', 'hybridsal2xml.template')
    hybridsal2xmltemplate = 'hybridsal2xml.template'
    # hybridsal2xml = os.path.join('hybridsal2xml','hybridsal2xml')
    hybridsal2xml = 'hybridsal2xml'
    if iswin:   # sys.platform.startswith('win'):
        if os.path.isfile(hybridsal2xml):
            os.remove(hybridsal2xml)
        hybridsal2xml += '.bat'
    #else:
        #hybridsal2xml += '.sh'
    sed( hybridsal2xmltemplate, hybridsal2xml, assgn )
    os.chmod( hybridsal2xml, 0775 )
    print("Created script {0}.".format(hybridsal2xml))
    print('Searching for make...', end=' ')
    output = checkProg('make')
    if not output:
        print('Failed.')
        print('***To build hybridsal2xml, you need to run make in the directory hybridsal2xml/')
        print('***Perhaps the existing built version suffices, so continuing...')
    else:
        print('Successful. Found {0}'.format(output))
        #os.chdir(os.path.join(antlrpath))
        #subprocess.call([ 'make' ])
        #os.chdir('..')
        if not os.path.lexists( 'antlr' ):
            if iswin:
                os.rename( os.path.join( antlrpath, 'antlr'), 'antlr')
            else:
                os.symlink( os.path.join( antlrpath, 'antlr'), 'antlr')
            # antlr  = os.path.join(antlrpath, 'antlr')
            # subprocess.call([ 'ln', '-s', antlr, 'antlr' ])
        subprocess.call([ 'make' ])
    if os.path.isfile('HybridSal2Xml.class'):
        print("hybridsal2xml installation complete.")
    else:
        print("hybridsal2xml installation failed.")
        print("***See error messages above and fix***")
        return 1
    hybridsal2xml1 = os.path.join( '..', 'bin' )
    shutil.copy( hybridsal2xml, hybridsal2xml1 )
    print("Created script {0}; also saved in directory {1}.".format(hybridsal2xml, hybridsal2xml1))
    #subprocess.call([ 'jar', 'cfm', 'hybridsal2xml.jar', 'Manifest.txt', '*.class', 'antlr' ])
    shutil.copy( 'hybridsal2xml.jar', '..' )
    return os.path.join('hybridsal2xml', hybridsal2xml)

def installmodelica():
    print('Installing Modelica2HybridSal converter ...')
    print('-------------------------------------------')
    print("Testing dparser...", end=' ')
    make_dparser = checkProg('make_dparser')
    if not make_dparser:
        print('Failed.')
        print('***Could not find dparser installed in your system')
        print('***Install dparser AND python support for dparser')
        print('***From dparser.sourceforge.net')
        print('***Continuing with the hope that python support for dparser was installed')
    else:
        print('Successful.')
    print("Testing python support for dparser...", end=' ')
    try:
        import dparser
    except ImportError:
        print('Failed.')
        print('***Install python support for dparser')
        print('Checking if swig is installed...', end=' ')
        swig = checkProg('swig')
        if not swig:
            print('Failed.')
            print('***You need to install swig first: www.swig.org/download.html')
            print('***Swig installation steps: download swigwin-2.0.7.zip file; unzip; ./configure; make; make install')
            print('***Swig uses pcre from www.pcre.org; but it may not be required')
        else:
            print('Successful. Found {0}'.format(swig))
        print('***Install dparser and its python support from dparser.sourceforge.net')
        print('***modelica2hybridsal converter installation failed')
        print('My Notes: swig and dparser installation required configure and make utilities')
        print('***I installed mingw on my windows machine; and then installed unix-like utilities, such as,')
        print('***mingw-get install gcc g++ msys-base msys-make mingw32-autotools')
        print('***And then installed pcre; swig; dparser; dparser-python-support;')
        print('***For python support for dparser: In the python subdirectory of dparser: python setup.py build --compiler=mingw32')
        print('***Ideally, add --compiler=mingw32 at end of the python setup.py build in the Makefile in d/python/')
        print('***I had to edit Python27/Lib/distutils/cygwinccompiler.py and remove all -mno-cygwin from python')
        print('You can still use HybridSal Relational Abstractor, but you can not use the Modelica2HybridSal front-end')
        return 1
    else:
        print('Successful.')

    print('Testing modelica2hybridsal converter...', end=' ')
    # python  /export/u1/homes/tiwari/sal/sal-devel/ashish-tools/modelica2hsal/src/modelica2hsal.py $*
    #ex4 = os.path.join('modelica2hsal', 'examples','RCEngine.xml')
    ex4 = os.path.join('modelica2hsal', 'examples','MassSpringDamperTest.MassSpringDamperTest.xml')
    if not os.path.isfile(ex4):
        print('Failed.')
        print('***Unable to find modelica example file')
    else:
        hsal2hasal = 'modelica2hsal'
        if sys.platform.startswith('win'):
            hsal2hasal += '.bat'
        exe = os.path.join('bin',hsal2hasal)
        try:
            subprocess.call([ exe, ex4 ])
        except Exception, e:
            print('Failed.')
            print('***Failed to execute generated script {0} using python subprocess.call'.format(hsal2hasal))
            print('***Check if the script looks ok; and if you can execute it from command line with one argument {0}'.format(ex4))
        else:
            print('Successful.')
    print('HybridSal Relational Abstracter and Modelica front-end successfully installed.')
    print("-------------------------------------------------")

def installsal(args_dict):
    global parser
    print("Searching for sal installation...")
    output = checkProg('sal-inf-bmc')
    iswin = sys.platform.startswith('win')	# is windows
    if not(output) and not(iswin):	# not windows
        print("***Download and install SAL; otherwise you can create SAL abstractions of HybridSal models, but you won't be able to model check them")
        print("***Update PATH with location of sal-inf-bmc")
        return 1
    elif not(iswin):
        print('sal-inf-bmc found at {0}'.format(output))
    else:  # windows
        print('Checking for cygwin at c:\cygwin')
        cygwin = os.path.join('C:',os.path.sep,'cygwin')
        if 'cygwin' in args_dict.keys() and args_dict['cygwin']:
            cygwin = args_dict['cygwin']
        if not os.path.isdir(cygwin):
            print('***Unable to find cygwin; install and rerun script')
            print('***If installed in non-standard location, rerun script as')
            print('***  {0}'.format( parser.format_usage() ))
            return 1
        print('cygwin found at {0}'.format(cygwin))
        print('searching for sal...')
        saldir = None
        if 'sal' in args_dict.keys() and args_dict['sal']:
            saldir = args_dict['sal']
        if saldir == None or not(os.path.isdir(saldir)):
            for root, dirnames, filenames in os.walk(cygwin):
                for dirname in dirnames:
                    if dirname.startswith('sal-'):
                        saldir = os.path.join(root, dirname)
                        break
                if saldir != None:
                    break
        if saldir != None and os.path.isdir(saldir):
            print('sal found at {0}'.format(saldir))
        else:
            print('***Unable to find SAL; download from sal.csl.sri.com')
            print('***install and rerun script')
            print('***If installed in non-standard location, rerun script as')
            print('***  {0}'.format( parser.format_usage() ))
            return 1
        # now we have saldir and cygwin both set...
        outfile = createSALfile(cygwin, saldir)
        print('created file {0}'.format(outfile))
    print('sal-inf-bmc successfully installed.')
    print("-------------------------------------------------")

def main():
    global parser
    parser = argparse.ArgumentParser(description="HybridSal installer.",
                                     formatter_class=argparse.RawDescriptionHelpFormatter,
                                     epilog="""
  Do not use the option --withsal if SAL is already installed.
  Do not use the option --withmodelica if you are not interested in Modelica.""")
    parser.add_argument("--withmodelica", help="support Modelica", action="store_true")
    parser.add_argument("--withsal", help="install SAL", action="store_true")
    parser.add_argument("--cygwin", metavar="CYGWIN_ROOT", help="use given directory as the Cygwin installation (Windows only)")
    parser.add_argument("--sal", metavar="SAL_ROOT", help="use given directory as the SAL installation")
    parser.add_argument("--rtjar", metavar="RT_FAR_FILE", help="path to rt.jar file", type=file)
    parser.add_argument("type", help="create distribution for Unix ('dist') or Windows ('win32'). If omitted, simply perform installation.", choices=["dist","win32"], nargs='?')    
    args = parser.parse_args()

    if args.type == 'dist':
        createRelease('.',args.withmodelica)
        return 0

    if args.type == 'win32':
        createReleaseExe('.')
        return 0

    print("-------------------------------------------------")
    print("Installing HybridSal Relational Abstraction Tool.")
    print("     Copyright (c) SRI International 2011.       ")
    print("-------------------------------------------------")

    #
    # Search for java and JAVA_HOME
    #
    print('Searching for java...', end=' ')
    javapath = checkProg('java')
    if not javapath:
        print('Failed. Install java and retry.')
        return 1
    else:
        print('Successful. Found {0}'.format(javapath))

    # set value of shell
    shell = checkProg( 'sh' )
    if not shell and 'SHELL' in os.environ.keys():
        shell = os.environ['SHELL']
    if shell:
        print('Using shell at {0}'.format(shell))

    #
    # Check that we're in the right directory
    #
    print('Searching for hybridsal2xml...', end=' ')
    output = os.getcwd()
    if output == '' or output == '\n':
        print('Error: pwd failed!')
        return 1
    if output[-1] == '\n':
        pwd = output[0:-1]
    else:
        pwd = output
    #hybridsal2xml = os.path.normpath(pwd + "/hybridsal2xml")
    hybridsal2xml = os.path.normpath(pwd)
    jarfile = os.path.join(hybridsal2xml,'hybridsal2xml.jar')
    if not os.path.isfile( jarfile ):
        hybridsal2xml = os.path.normpath(os.path.join(pwd, "hybridsal2xml"))
        #        jarfile = os.path.join(hybridsal2xml,'hybridsal2xml.jar')
        if not os.path.isdir(hybridsal2xml):
            print('Failed to find hybridsal2xml.jar ')
            print('***Error: RUN THIS SCRIPT FROM THE HSAL ROOT DIRECTORY')
            print('***The HSAL root directory is the directory created by tar -xz or unzip')
            return 1
    if not os.path.isfile( jarfile ):
        rtjar = searchForrtjar( args.rtjar.name if args.rtjar else None , javapath )
        jikespath = searchForjikes()
        hybridsal2xml = compile_hybridsal2xml( hybridsal2xml, shell, rtjar, jikespath )
        os.chdir( pwd )
        hybridsal2xml = os.path.normpath(pwd)
        jarfile = os.path.join(hybridsal2xml,'hybridsal2xml.jar')
    if os.path.isfile( jarfile ):
        if os.environ.has_key('CLASSPATH'):
            os.environ['CLASSPATH'] = hybridsal2xml + os.pathsep + os.environ['CLASSPATH']
        else:
            os.environ['CLASSPATH'] = '.' + os.pathsep + hybridsal2xml + os.pathsep + os.pathsep
        print('Found hybridsal2xml.jar at {0}'.format(jarfile))
    else:
        print('***Failed to create hybridsal2xml. Giving up.')
        return 1

    # Test hybridsal2xml converter
    print('Testing hybridsal2xml...', end=' ')
    hsal = os.path.join('examples', 'Linear1.hsal')
    hxml = os.path.join('examples', 'Linear1.hxml')
    if not os.path.isfile( hsal ):
        print('***Failed to find an example (examples/Linear1.hsal) for testing')
    else:
        if os.path.isfile(hxml):
            os.remove(hxml)
        try:
            subprocess.call([ 'java', '-jar', jarfile, '-o', hxml, hsal ])
        except Exception, e:
            print('Failed.')
            print('***Failed to run hybridsal2xml using the precompiled jar file and antlr')
            print('***Trying to compile hybridsal2xml from sources...')
            if os.path.isfile(hxml):
                os.remove(hxml)
        if os.path.isfile( hxml ):
            print('Successful.')
            hybridsal2xml = create_hybridsal2xml_exe( pwd, shell, jarfile )

    #
    # Run a test
    #
    '''
    print "Testing hybridsal2xml...",
    ex4 = os.path.join('examples','SimpleThermo4.hxml')
    if os.path.isfile(ex4):
        os.remove( ex4 )
    # subprocess.call([ 'rm', '-f', 'examples/SimpleThermo4.xml'])
    exe = os.path.join('.', hybridsal2xml)
    ex4sal = os.path.join('examples','SimpleThermo4.hsal')
    if not os.path.isfile(ex4sal):
        print 'Failed.'
        print '***ERROR: File {0} is missing from installation'.format(ex4sal)
        print '***Download of HybridSal .tgz was incomplete???'
        print '***Continuing without testing hybridsal2xml...'
    else:
        subprocess.call([ exe, '-o', ex4, ex4sal ])
        if os.path.isfile( ex4 ):
            print 'Successful.'
        else:
            print 'Failed.'
            print '***hybridsal2xml installation failed. See earlier warnings/error messages to determine cause of failure.'
            return 1
    # os.chdir('..')
    '''

    #
    # create bin/ files
    #
    srcdir = os.path.join(pwd, 'src')
    if os.path.isdir(srcdir):
        # sources downloaded; so I will create more bin/ files just to give more tools to user
        print('Creating bin/ files...', end=' ')
        bindir = os.path.join(pwd, "bin")
        if not(os.path.isdir(bindir)):
            os.makedirs(bindir)
            #print 'Error: Directory %s does not exist; create it and rerun install.' % bindir
            #return 1
        #os.chdir(pwd + "/bin")
        topshell = '' if sys.platform.startswith('win') else '#!{0}'.format(shell)
        try:
            createBinFile(topshell, pwd, bindir, 'hxml2hsal', os.path.join('src','HSalXMLPP.py'))
            createBinFile(topshell, pwd, bindir, 'hsal2hasal', os.path.join('src','HSalRelAbsCons.py'))
            createBinFile(topshell, pwd, bindir, 'hasal2sal', os.path.join('src','HSalExtractRelAbs.py'))
            createBinFile(topshell, pwd, bindir, 'hsal2Tsal', os.path.join('src','HSalTimedRelAbsCons.py'))
            createBinFile(topshell, pwd, bindir, 'modelica2hsal', os.path.join('modelica2hsal', 'src','modelica2hsal.py'))
            createBinFile(topshell, pwd, bindir, 'modelica2sal', os.path.join('modelica2hsal', 'src','modelica2sal.py'))
        #filename = os.path.join(pwd, 'bin', 'hsal2hxml')
        #if os.path.isfile(filename):
            #os.remove(filename)
        #subprocess.call(['ln', '-s', os.path.join('..','hybridsal2xml',hybridsal2xml), os.path.join(pwd, 'bin', 'hsal2hxml')])
        except Exception, e:
            print('Failed.')
            print('***Unable to create executable scripts. Perhaps permission issues??')
            print(e)
            return 1
        else:
            print('Successful.')
    else:
        pass

    # 
    # python version test
    # 
    if sys.version < '2.6':
        print('****Warning: Preferably use python version 2.6 or higher****')

    # 
    # Test if numpy and scipy are installed
    # 
    print('Searching for numpy...', end=' ')
    try:
        import numpy
    except ImportError, e:
        print('Failed.')
        print('***Install python modules numpy and scipy first.')
        print('***On Ubuntu: sudo aptitude install python-numpy')
        print('***On Windows: get numpy-1.6.2-win32-superpack-python2.7.exe or Scipy-stack-13.4.9.win-amd64-py2.7.exe (includes numpy)')
        return 1
    else:
        print('Found.')
    print('Searching for scipy...', end=' ')
    try:
        import scipy
    except ImportError, e:
        print('Failed.')
        print('***Install python modules numpy and scipy first.')
        print('***On Ubuntu: sudo aptitude install python-scipy')
        print('***On Windows: get scipy-0.11.Orc1-superpack-python2.7.exe or Scipy-stack-13.4.9.win-amd64-py2.7.exe')
        return 1
    else:
        print('Found.')

    # evaluate other options:
    exe = 'hsalRA.exe'
    if not os.path.isfile(exe):
        if args.withmodelica:
            installmodelica()
    if args.withsal:
        installsal(vars(args))

    # 
    # Test relational abstracter itself
    # 
    ex4 = os.path.join('examples','Linear1.sal')
    if os.path.isfile(ex4):
        os.remove( ex4 )
    # subprocess.call([ 'rm', '-f', 'examples/SimpleThermo4.xml'])
    hsal2hasal = 'hsal2hasal'
    if sys.platform.startswith('win'):
        hsal2hasal += '.bat'
    exe = os.path.join('bin',hsal2hasal)
    if os.path.isfile(exe):
        print("Testing HybridSal relational abstracter...", end=' ')
        try:
            subprocess.call([ exe, os.path.join('examples','Linear1.hsal') ])
        except Exception, e:
            print('Failed.')
            print('***Failed to execute generated script {0} using python subprocess.call'.format(exe))
            print('***Check if the script looks ok; and if you can execute it from command line')
            return 1
        if os.path.isfile( ex4 ):
            print('Successful.')
        else:
            print('Failed.')
            print('***Executed the generated script {0}, but it did not generate expected output'.format(exe))
            return 1

    #
    # Test hsalRA.exe
    #
    exe = 'hsalRA.exe'
    ex1 = os.path.join('examples','MassSpringDamperTest.MassSpringDamperTest.xml')
    prop1 = os.path.join('examples','MassSpringDamperTest.property.json')
    if os.path.isfile(exe) and os.path.isfile(ex1) and os.path.isfile(prop1):
        print("Testing hsalRA.exe ...\n", end=' ')
        try:
            subprocess.check_call([ exe, ex1, prop1 ])
        except Exception, e:
            print('Failed.')
            print('***Failed to execute hsalRA.exe')
            return 1
        print('Testing hsalRA.exe was successful.')
    
    #
    # Test dparser and swig for modelica2hxml converter
    #

    print('HybridSal successfully installed.')
    if not sys.platform.startswith('win'):
        print('run make test to see it working.')
    return 0

def createSALfile(cygwin, saldir):
    binfile = 'sal-inf-bmc.bat'
    fp = open( binfile, 'w')
    cygwinbash = os.path.join(cygwin, 'bin', 'bash')
    salinfbmc = os.path.join(saldir, 'bin', 'sal-inf-bmc')
    print("{0} -li {1} %1 %2 %3 %4 %5 %6 %7 %8 %9".format( repr(cygwinbash)[1:-1], repr(salinfbmc)[1:-1]), file=fp)
    fp.close()
    os.chmod( binfile, 0775 )
    return binfile

def createBinFile(shell, pwd, bindir, filename, pythonfile):
    binfile = os.path.join(bindir, filename)
    if sys.platform.startswith('win'):
        binfile += '.bat'
    fp = open( binfile, 'w')
    if sys.platform.startswith('win'):
        pyfile = repr(os.path.join(pwd, pythonfile))
        print('python ', pyfile[1:-1], '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', file=fp)
    else:
        print(shell, file=fp)
        print('python ', os.path.join(pwd, pythonfile), '$*', file=fp) 
    fp.close()
    # subprocess.call(['chmod', '+x', binfile])
    os.chmod( binfile, 0775 )

def make_hybridsal2xmljar(currdir, h2xmldir, dstdir):
    # move directory antlr up one level as a symbolic link
    # jar cfm hybridsal2xml.jar Manifest.txt *.class antlr
    tmpdir = os.path.join(dstdir, 'tmp')
    shutil.copytree(h2xmldir, tmpdir, ignore=shutil.ignore_patterns('.svn'))
    os.chdir(tmpdir)
    if not os.path.isdir('antlr'):
        os.rename(os.path.join('antlr-2.7.1','antlr'),'antlr')
    subprocess.call(['jar','cfm','hybridsal2xml.jar','Manifest.txt','*.class','antlr'])
    os.rename('hybridsal2xml.jar',os.path.join('..','hybridsal2xml.jar'))
    os.chdir(currdir)
    shutil.rmtree( tmpdir )

def make_readme(currdir, dstdir):
    os.chdir(dstdir)
    with open('README','w') as fp:
        print('''
Prerequisites:
--------------
This is a precompiled version of HybridSal analysis tool
for Windows platform.  It was built on win32, but hopefully
it should work on other win platforms too.

Installation
-------------
Run "python install.py"

Running the above installation script should tell you what
is missing.  Most likely, SAL would be missing.

HybridSAL uses SAL model checkers. To install, follow these steps:
(1) Install cygwin: install the basic/default variety
    (Doesn't take too long, but need fast internet connection)
    (google on cygwin: www.cygwin.com)
(2) Download sal-3.2 tgz file for Windows/cygwin (sal.csl.sri.com)
(3) Move sal-3.2.tgz to a place inside cygwin, such as,
    c:\cygwin\home\<username>\
(4) Open a cygwin shell, Unpack sal and run install.sh script there
(5) Update PATH variable with path to sal (mentioned by install.sh)

Now, do "python install.py" for installing HybridSal (as above).



Running HybridSal:
------------------
Run
 ./hsalRA.exe <ModelicalXML-file> <Property-file>

For example, run:

./hsalRA.exe examples/MassSpringDamperTest.MassSpringDamperTest.xml examples/MassSpringDamperTest.property.json
''', file=fp)
    os.chdir(currdir)
    return

def createReleaseExe(srcdir):
    'create directory for win32 release'
    join = os.path.join
    isdir = os.path.isdir
    isfile = os.path.isfile
    distdir = join(srcdir, 'win32_HybridSal')
    # if isdir(distdir):
        # shutil.rmtree( distdir )
    #os.mkdir(distdir, 0755)
    # run python setup.py py2exe
    if (not isdir('dist') or not isfile(join('dist','hsalRA.exe'))) and (not isdir(distdir) or not isfile(join(distdir,'hsalRA.exe'))):
        # python26 = join('C:',os.path.sep,'Python26','python.exe')
        # subprocess.call([python26,'setup.py','py2exe'])
        python27 = join('C:',os.path.sep,'Python27','python.exe')
        subprocess.call([python27,'setup.py','py2exe'])
    if isdir('dist') and not isdir(distdir):
        os.rename('dist', distdir)
    assert isdir(distdir) and isfile(join(distdir,'hsalRA.exe'))
    # move directory antlr up one level as a symbolic link
    # jar cfm hybridsal2xml.jar Manifest.txt *.class antlr
    hybridsal2xmljar = join('hybridsal2xml','hybridsal2xml.jar')
    currdir = os.path.abspath('.')
    if not isfile(hybridsal2xmljar):
        make_hybridsal2xmljar(currdir,'hybridsal2xml',distdir)
    else:
        shutil.copy(hybridsal2xmljar, distdir)
    hybridsal2xmljar = join(distdir,'hybridsal2xml.jar')
    assert isfile(hybridsal2xmljar)
    # create sal-inf-bmc.bat and README
    os.chdir( distdir )
    installsal({})
    os.chdir( currdir )
    make_readme(currdir, distdir)
    # make examples/ directory
    allFiles1 = ['MassSpringDamperTest.MassSpringDamperTest.xml','MassSpringDamperTest.property.json']
    allFiles2 = ['Linear1.hsal','SimpleThermo4.hsal']
    examples_dir = os.path.join(distdir,'examples')
    if not isdir(examples_dir):
        os.mkdir( os.path.join(distdir,'examples'), 0755 )
    for i in allFiles1:
        src = os.path.join( currdir, 'modelica2hsal', 'examples', i)
        dst = os.path.join( distdir, 'examples', i)
        shutil.copy(src, dst)
    for i in allFiles2:
        src = os.path.join( currdir, 'examples', i)
        dst = os.path.join( distdir, 'examples', i)
        shutil.copy(src, dst)
    # copy install.py ! copy COPYRIGHT and doc
    shutil.copy('install.py', distdir)
    shutil.copy('COPYRIGHT', distdir)
    shutil.copy( os.path.join( currdir, 'hybridsal2xml', 'hybridsal2xml.bat'), distdir)
    print('Successfully created distribution {0}'.format(distdir))
 
def createRelease(srcdir,withmodelica):
    'create directory for release purposes'
    distdir = os.path.join(srcdir, 'HybridSal')
    distdirold = os.path.join(srcdir, 'HybridSal~')
    files = files_hsal
    if withmodelica:
        files += files_modelica
    allFiles = files.splitlines()
    if os.path.isdir(distdir):
        try:
            if os.path.isdir(distdirold):
                shutil.rmtree( distdirold )
            os.rename(distdir, distdirold)
            print('directory {0} exists. Renaming it to {1}'.format(distdir, distdirold))
        except Exception, e:
            print('Error: Exception raised', e)
            print('System level exception; Maybe a permission issue')
            return -1
    try:
        os.mkdir(distdir, 0755)
        for i in allFiles:
            src = os.path.join( srcdir, i)
            dst = os.path.join( distdir, i)
            if os.path.isdir( src ):
                if not os.path.isdir( dst ):
                    os.makedirs( dst, 0755 )
                continue
            dirname = os.path.dirname(i)
            dstdir = os.path.join( distdir, dirname )
            if not os.path.isdir( dstdir ):
                os.makedirs( dstdir )
            checkFile( src )
            shutil.copy2( src, dst )
    except Exception, e:
        print('Error: Exception raised', e)
        print('System level exception; Maybe a permission issue')
        return -1
    print('Successfully created distribution directory {0}'.format(distdir))
    import tarfile
    tgz = tarfile.open(distdir + '.tgz', 'w|gz')
    tgz.add(distdir)
    tgz.close()
    shutil.rmtree( distdir )
    print('Successfully created distribution {0}'.format(distdir+'.tgz'))

if __name__ == '__main__':
    main()

