#==========================================================================
# Result.zip
# Call by Jenkins/JobManager for zipping up a results directory after
# a remote job has finished.
# Components directory will not be copied back because it would already
# exist on the user's local machine.
# The search_meta.pro.local is renamed to search_meta.pro so it would
# reference local components directory.
#==========================================================================

import zipfile
import os
import os.path
import sys

output_filename = '{0}'
search_filename ='search_META.pro'
search_file_lc ='search_META.pro.local'

path_join = os.path.joinif sys.platform == 'win32':    def path_join(*args):        return '\\\\?\\' + os.path.join(os.getcwd(), os.path.join(*args))

# LS_Dyna workers have RHEL6. RHEL6 has Python2.6, which doesnt have zipfile.ZipFile.__exit__ http://bugs.python.org/issue5511 . So we dont use 'with'
z = zipfile.ZipFile(output_filename, 'w', allowZip64=True)
try:
    for dirpath,dirs,files in os.walk('{1}'):
        if 'components' in dirpath:   # skips components directory
          continue
        for f in files:
            if output_filename == f:
                continue
            if f == search_filename:
                continue
            if f == search_file_lc:
                z.write(os.path.join(dirpath, f), os.path.join(dirpath, search_filename), compress_type=zipfile.ZIP_DEFLATED)
                continue
            z.write(path_join(dirpath, f), arcname=os.path.join(dirpath, f), compress_type=zipfile.ZIP_DEFLATED)
finally:
    z.close()
