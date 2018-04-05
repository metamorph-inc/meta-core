from __future__ import print_function

import os
import os.path
import sys
import subprocess
import shutil
import zipfile
import py_compile

PYC_EXCLUDES = ('matlab_proxy', 'jinja2\\asyncsupport.py', 'jinja2\\asyncfilters.py', r'mpmath\libmp\exec_py3.py', r'mpmath\tests')

def pyc_exclude(filename):
    for exclude in PYC_EXCLUDES:
        if exclude in filename:
            return True
    return False


def print_zip_safe():
    safes = []

    for egg_dir in os.listdir('../bin/Python27/Lib/site-packages'):
        egg_dir = '../bin/Python27/Lib/site-packages/' + egg_dir
        if egg_dir.endswith('.dist-info'):
            continue
        if os.path.isfile(egg_dir):
            continue
        import setuptools.command.bdist_egg
        safe = setuptools.command.bdist_egg.analyze_egg(egg_dir, [])
        print('{} zip-safe? {}\n'.format(egg_dir, safe), file=sys.stderr)


def compileall():
    compiled = {}

    # ls_files = subprocess.Popen('git ls-files'.split() + [src], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # ls_files = subprocess.Popen('git grep -EL "(pkg_resources|__file__)"  ../bin/Python27/**.py', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    ls_files = subprocess.Popen('git ls-files  ../bin/Python27/Lib/site-packages/**.py', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = ls_files.communicate()
    exit_code = ls_files.poll()
    if exit_code != 0:
        raise Exception('failed: ' + err)

    for filename in list(line.replace("/", "\\") for line in out.splitlines()):
        # print filename
        if pyc_exclude(filename):
            continue
        if os.path.isdir(filename):
            continue
        dfile = 'META\\' +  os.path.relpath(filename, '..')

        # tmp_name = '__tmp.pyc'   'cfile': tmp_name,
        kwargs = {'file': filename, 'dfile': dfile, 'doraise': True}
        py_compile.compile(**kwargs)

        compiled[os.path.normpath(filename)] = os.path.normpath(filename) + 'c'

    return compiled


def zipall():
    zipped_files = {}
    # subprocess.check_call('git checkout ../bin/Python27/Scripts/Python27.zip')
    shutil.copyfile('../bin/Python27/Scripts/Python27.zip', 'Python27.zip')

    with zipfile.ZipFile('Python27.zip', 'a', compression=zipfile.ZIP_DEFLATED, allowZip64=True) as python_zip:
        i = 0
        # these packages have been determined to be zip-safe
        # TODO fix openmdao """with open(os.path.join(os.path.dirname(__file__), 'unit_library.ini')) as default_lib:"""
        # TODO add "markupsafe", "lazy_object_proxy" once .pyds are handled
        for package in ("dateutil", "excel_wrapper", "markdown", "mpl_toolkits", "mpmath", "networkx",
                "OMPython", "colorama", "wrapt", "pytz"):
            ls_files = subprocess.Popen('git ls-files  ../bin/Python27/Lib/site-packages/{}/**'.format(package), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            out, err = ls_files.communicate()
            exit_code = ls_files.poll()
            if exit_code != 0:
                raise Exception('failed: ' + err)

            for filename in list(line.replace("/", "\\") for line in out.splitlines()):
                i = i + 1
                dest_filename = filename[len('../bin/Python27/Lib/site-packages/'):]
                zipped_files[os.path.normpath(filename)] = None
                if filename.endswith('.py'):
                    if not pyc_exclude(filename):
                        filename = filename + 'c'
                        dest_filename = dest_filename + 'c'

                if '.egg\\' in dest_filename:
                    dest_filename = dest_filename[dest_filename.index('.egg\\') + len('.egg\\'):]
                # print(filename, dest_filename)
                python_zip.write(filename, dest_filename)
    # shutil.copyfile('Python27.zip', '../bin/Python27/Scripts/Python27.zip')
    return zipped_files


if __name__ == '__main__':
    print_zip_safe()
    # bin_file_map = compileall()
    # bin_file_map.update(zipall())
    # print(bin_file_map)
