#!/usr/bin/python
import zipfile
import os
import sys
import os.path

path_join = os.path.join
if sys.platform == 'win32':
    def path_join(*args):
        return '\\\\?\\' + os.path.join(os.getcwd(), os.path.join(*args))

output_filename = 'source_data.zip'

if os.path.exists(output_filename):
    os.remove(output_filename)

with zipfile.ZipFile(output_filename, 'w', allowZip64=True) as z:
    parent_dir_name = os.path.basename(os.getcwd())
    os.chdir('..\\')
    for dirpath,dirs,files in os.walk(parent_dir_name):
        if '.git' in dirpath or '.svn' in dirpath:
            continue        
        for f in files:
            if output_filename == f or f.endswith('.suo'):
                continue
            fn = path_join(dirpath, f)
            #print fn
            z.write(fn, arcname=os.path.join(dirpath, f), compress_type=zipfile.ZIP_DEFLATED)

