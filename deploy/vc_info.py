import sys
import os
import os.path
import posixpath
import subprocess

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
meta_path = os.path.normpath(os.path.join(THIS_DIR, '..'))

def throw_if_modified():
    output = subprocess.check_output('git status -uno --porcelain'.split(), cwd=meta_path).rstrip()
    if len(output):
        raise Exception('Files are modified: %s\nDo `git commit` and try again' % output)

def repo_rev():
    return last_changed_rev('.')

def last_cad_rev():
    return last_changed_rev('src/CADAssembler', 'meta/CAD')

def last_mdl2mga_rev():
    return last_changed_rev('externals/HCDDES', 'meta/Cyber')
   
def last_changed_rev(*paths):
    output = subprocess.check_output('git log --format=%H -n 1 --'.split() + list(paths), cwd=meta_path)
    return str(commit_no(*paths)) + '-git' + output.rstrip()[0:8]

def commit_no(*paths):
    return int(subprocess.check_output('git rev-list HEAD --count --'.split() + (list(paths) or ['.']), cwd=meta_path).rstrip())
    
def update_version(version, last_version):
    version = version.split(".")
    version[-1] = str(last_version)
    return ".".join(version)
    
if __name__ == '__main__':
    print last_cad_rev()
    throw_if_modified()
