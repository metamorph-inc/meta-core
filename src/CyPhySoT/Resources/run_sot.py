import time
import subprocess
import sys
import os
import _winreg as wr

def _query_registry():
    """
    Queries the Windows registry for META_PATH in order to get the location of
    python virtual environment containing all necessary packages and modules. 
    """

    try:
        # try to obtain META_PATH from the registry
        key = wr.OpenKey(wr.HKEY_LOCAL_MACHINE, r'software\meta', 0, wr.KEY_READ)
        meta_path = wr.QueryValueEx(key, 'META_PATH')[0]
        py_path = os.path.join(meta_path, r'bin\Python27\Scripts\python')
    except WindowsError:
        sys.stderr.write('Could not find META_PATH in registry, attempting to use default python.')
        py_path = 'python'
    
    return py_path


def main():
    py_path = _query_registry()
    command = '"{0}" sot.py'.format(py_path)
    
    print 'Calling "{0}" as a subprocess.'.format(command)
    try:
        popen = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=None)
        (stdoutdata, _) = popen.communicate()
        print stdoutdata
        popen.wait()
        if popen.returncode != 0:
            raise subprocess.CalledProcessError(popen.returncode, command)
        
        #execution_time = time.time() - t_1
        #_write_out_stat(execution_time, iso_time)
    except subprocess.CalledProcessError, err:
        sys.stderr.write('Out-print : {0}\n\n{1}\n'.format(err, err.output))
        sys.stderr.write('Failed calling {0}\n'.format(command))
        sys.exit(5)
        
    return 0

if __name__ == '__main__':
    sys.exit(main())
