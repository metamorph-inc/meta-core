import glob
import shutil
import sys
from contextlib import contextmanager
import os
import subprocess
from subprocess import PIPE
import fnmatch
import fileinput


@contextmanager
def pushd(newDir):
    previousDir = os.getcwd()
    os.chdir(newDir)
    yield
    os.chdir(previousDir)


def call_xsd2code(abspath_xsd2code, path_xsd, cu_namespace=None):
    print("Generating for " + path_xsd)
    call = abspath_xsd2code + ' ' + path_xsd + ' /eit /is /xa'
    if cu_namespace is not None:
        call = call + ' /cu ' + cu_namespace
    subprocess.check_call(call.split(), shell=False, stdout=PIPE)


def rmdir(path):
    if os.path.exists(path):
        os.chmod(path, 493)
        shutil.rmtree(path)


def make():
    print("====== Generate Libraries ======")
    print("")
    print("== Copy Schema Files ==")
    # Prepare the /schema folder
    path_schema = "schema"
    if os.path.exists(path_schema):
        rmdir(os.path.abspath(path_schema))
    os.makedirs(path_schema)

    for xsd in glob.glob("EMF_Metamodel/model/*.xsd"):
        shutil.copy2(xsd, path_schema)

    os.remove(os.path.join(path_schema, 'AssemblyDetails.xsd'))
    shutil.copy2('iFAB/assemblyDetails.xsd', path_schema)
    os.rename(os.path.join(path_schema, 'assemblyDetails.xsd'), os.path.join(path_schema, 'AssemblyDetails.xsd'))
    shutil.copy2('iFAB/common.xsd', path_schema)
    os.remove(os.path.join(path_schema, 'XMLType.xsd'))

    for xsd in glob.glob(os.path.join(path_schema, "*.xsd")):
        print(xsd)

    # Generate C# classes
    print("")
    print("== Generate C# classes ==")
    if sys.platform.startswith('win32'):
        relpath_Xsd2Code = os.path.join('..', '..', '3rdParty', 'xsd2code', 'Xsd2Code.exe')
        abspath_Xsd2Code = os.path.abspath(relpath_Xsd2Code)
        with pushd('schema'):
            call_xsd2code(abspath_Xsd2Code, 'avm.xsd', 'iFAB')
            call_xsd2code(abspath_Xsd2Code, 'AssemblyDetails.xsd', 'iFAB')
            call_xsd2code(abspath_Xsd2Code, 'avm.cad.xsd')
            call_xsd2code(abspath_Xsd2Code, 'avm.manufacturing.xsd')
            call_xsd2code(abspath_Xsd2Code, 'avm.modelica.xsd')
            call_xsd2code(abspath_Xsd2Code, 'avm.cyber.xsd')
            call_xsd2code(abspath_Xsd2Code, 'avm.adamsCar.xsd')

            for cs in glob.glob('*.cs'):
                shutil.copy2(cs, os.path.join('..', 'lib', 'csharp', 'OpenMETA.Interchange', 'generated'))
    else:
        print("ERROR: Xsd2Code cannot run on a non-Windows platform; skipping C# generation")

    print("")
    print("== Generate Python library ==")
    relpath_pyxbgen = os.path.join('..', '..', 'bin', 'Python311', 'Scripts', 'pyxbgen')
    abspath_pyxbgen = os.path.abspath(relpath_pyxbgen)
    abspath_python_interpreter = sys.executable
    with pushd('schema'):
        command = abspath_python_interpreter + ' ' + abspath_pyxbgen + ' -u component.xsd -m component'
        command_ = command.split()
        subprocess.check_call(command_, shell=False)

        for py in glob.glob('*.py'):
            print(py)

    # clear old python libraries
    path_pylib = os.path.join('lib', 'python')
    rmdir(os.path.abspath(os.path.join(path_pylib, 'avm')))
    rmdir(os.path.abspath(os.path.join(path_pylib, 'iFAB')))

    os.makedirs(os.path.join(path_pylib, 'avm'))
    os.makedirs(os.path.join(path_pylib, 'avm', 'modelica'))
    os.makedirs(os.path.join(path_pylib, 'avm', 'cad'))
    os.makedirs(os.path.join(path_pylib, 'avm', 'manufacturing'))
    os.makedirs(os.path.join(path_pylib, 'avm', 'cyber'))
    os.makedirs(os.path.join(path_pylib, 'avm', 'adamsCar'))
    os.makedirs(os.path.join(path_pylib, 'iFAB'))

    # Copy files to library structure
    shutil.copy2(os.path.join(path_schema, '_avm.py'), os.path.join(path_pylib, 'avm', '__init__.py'))
    shutil.copy2(os.path.join(path_schema, '_modelica.py'), os.path.join(path_pylib, 'avm', 'modelica', '__init__.py'))
    shutil.copy2(os.path.join(path_schema, '_adamsCar.py'), os.path.join(path_pylib, 'avm', 'adamsCar', '__init__.py'))
    shutil.copy2(os.path.join(path_schema, '_cad.py'), os.path.join(path_pylib, 'avm', 'cad', '__init__.py'))
    shutil.copy2(os.path.join(path_schema, '_manufacturing.py'),
                 os.path.join(path_pylib, 'avm', 'manufacturing', '__init__.py'))
    shutil.copy2(os.path.join(path_schema, '_cyber.py'), os.path.join(path_pylib, 'avm', 'cyber', '__init__.py'))
    shutil.copy2(os.path.join(path_schema, '_iFAB.py'), os.path.join(path_pylib, 'iFAB', '__init__.py'))

    # Do find-and-replace to make names nicer
    abspath_schema = os.path.abspath(path_schema)
    for root, dirnames, filenames in os.walk(path_pylib):
        for filename in fnmatch.filter(filenames, '*.py'):
            py_path = os.path.join(root, filename)
            for line in fileinput.input(py_path, inplace=1):
                newline = line.replace('import _avm', 'import avm') \
                    .replace('import _modelica', 'import modelica') \
                    .replace('import _iFAB', 'import iFAB') \
                    .replace(abspath_schema.replace('\\', '\\\\') + '\\\\', '')
                sys.stdout.write(newline)

    print("")
    print("== Make Release ==")
    path_release = 'release'
    rmdir(os.path.abspath(path_release))
    os.makedirs(path_release)
    os.chmod(path_release, 493)

    shutil.copytree(path_schema, os.path.join(path_release, 'schema'))
    for f in glob.glob(os.path.join(path_release, 'schema', '*.*')):
        if f.find('.xsd') is -1:
            os.remove(f)

    shutil.copytree('lib', os.path.join(path_release, 'lib'))
    shutil.copytree('exampleprojects', os.path.join(path_release, 'exampleprojects'))
    shutil.copy2('doc/AVM_Component_Spec.pdf', path_release)

    path_emf_metamodel = os.path.join(path_release, 'EMF_Metamodel')
    os.makedirs(path_emf_metamodel)
    os.makedirs(os.path.join(path_emf_metamodel, 'model'))

    shutil.copy2('EMF_Metamodel/model/AVM.ecore', os.path.join(path_emf_metamodel, 'model'))
    shutil.copy2('EMF_Metamodel/model/AVM.ecorediag', os.path.join(path_emf_metamodel, 'model'))
    shutil.copy2('EMF_Metamodel/model/avm.genmodel', os.path.join(path_emf_metamodel, 'model'))
    shutil.copy2('EMF_Metamodel/model/XMLType.ecore', os.path.join(path_emf_metamodel, 'model'))


if __name__ == "__main__":
    path_script = os.path.realpath(__file__)
    path_scriptDir = os.path.dirname(path_script)
    with pushd(path_scriptDir):
        make()