import zipfile
import os
import sysimport os.path
path_join = os.path.joinif sys.platform == 'win32':    def path_join(*args):        return '\\\\?\\' + os.path.join(os.getcwd(), os.path.join(*args))

output_filename = '{0}'

# LS_Dyna workers have RHEL6. RHEL6 has Python2.6, which doesnt have zipfile.ZipFile.__exit__ http://bugs.python.org/issue5511 . So we dont use 'with'
z = zipfile.ZipFile(output_filename, 'w', allowZip64=True)
try:
    for dirpath,dirs,files in os.walk('{1}'):
      for f in files:
        if output_filename == f:
            continue
        z.write(path_join(dirpath, f), arcname=os.path.join(dirpath, f), compress_type=zipfile.ZIP_DEFLATED)
finally:
    z.close()
