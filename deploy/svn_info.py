import sys
import os
import os.path
import posixpath
import subprocess

THIS_DIR = os.path.dirname(os.path.abspath(__file__))

def is_dev_branch(branch_name):
    return 'META-' in branch_name

def get_branch_name():
    try:
        svn_info = subprocess.check_output(['svn', 'info'], cwd=os.path.join(THIS_DIR, '..'))
    except subprocess.CalledProcessError as e:
        return 'export'
    svn_info_lines = svn_info.replace('\r', '').split('\n')
    return posixpath.basename([line for line in svn_info_lines if line.find('URL: ') == 0][0][5:])

def svnversion():
    version = subprocess.check_output(['svnversion.exe'], cwd=os.path.abspath(os.path.join(THIS_DIR, '..'))).strip()
    # memoize
    setattr(sys.modules[svnversion.__module__], 'svnversion', lambda: version)
    return version

def last_cad_rev():
    meta_path = os.path.normpath(os.path.join(THIS_DIR, '..'))
    return max((last_changed_rev(os.path.join(meta_path, path)) for path in ('src/CADAssembler', 'meta/CAD')))

def last_mdl2mga_rev():
    meta_path = os.path.normpath(os.path.join(THIS_DIR, '..'))
    return max((last_changed_rev(os.path.join(meta_path, path)) for path in ('externals', 'meta/Cyber')))
   
def last_changed_rev(path):
    output = subprocess.check_output(['svn.exe', 'info', path])
    import re
    return int(re.search('Last Changed Rev: (\\d+)', output).groups()[0])
    
def prerelease_suffix():
    branch = get_branch_name()
    if is_dev_branch(branch):
        return '-' + branch + 'r' + svnversion().split(':')[0]
    return ''

def update_version(version, svnversion):
    version = version.split(".")
    version[-1] = str(svnversion)
    return ".".join(version)
    
if __name__ == '__main__':
    print get_branch_name()
