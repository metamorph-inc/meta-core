'''
Apparently symstore.exe /r inspects every file. This program will hard link files of interest to a new temp dir, then call symstore on that.
'''

import sys
import os
import os.path
import datetime
import win32file
import win32process
import win32api
import tempfile
import pywintypes

def log(msg):
    print "%s: %s" % (os.path.basename(sys.argv[0]), msg)

def main(source, destination, job_name, version):
    extensions = set(('.pdb', '.dll', '.exe', '.ocx'))
    files_copied = 0

    starttime = datetime.datetime.now()
    log("Start " + starttime.isoformat())
    destdir = tempfile.mkdtemp()
    log("destination %s" % destdir)


    for root, dirs, files in os.walk(source):
        #print dirs
        for filename in (f for f in files if f[-4:] in extensions):
            pdb_dir = os.path.join(destdir, os.path.relpath(root, source))
            try:
                os.makedirs(pdb_dir)
            except WindowsError as err:
                if err.winerror != 183:
                    raise
            # print os.path.join(pdb_dir, filename)
            win32file.CreateHardLink(os.path.join(pdb_dir, filename), os.path.join(root, filename))
            files_copied += 1
            #except pywintypes.error as err:
            # if err.winerror != 183: #pywintypes.error (183, 'CreateHardLink', 'Cannot create a file when that file already exists.')
        for exclude in ('.git', '.svn', 'CVS', '.hg', '3rdParty', 'Python27', 'Python26'):
            if exclude in dirs:
                dirs.remove(exclude)

    log("%d files copied" % files_copied)

    startup = win32process.STARTUPINFO()
    startup.dwFlags += win32process.STARTF_USESTDHANDLES
    startup.hStdInput = win32file.INVALID_HANDLE_VALUE
    security_attributes = pywintypes.SECURITY_ATTRIBUTES()
    security_attributes.bInheritHandle = 1
    log_path = os.path.join(destdir, "log")
    startup.hStdOutput = startup.hStdError = win32file.CreateFile(log_path, win32file.GENERIC_WRITE, win32file.FILE_SHARE_READ, security_attributes, win32file.CREATE_ALWAYS, 0, None)
    win32file.WriteFile(startup.hStdOutput, 'log started\n')

    dtw_locations = (os.path.join(os.environ.get('ProgramFiles', r'C:\Program Files (x86)'), 'Debugging Tools for Windows (x86)'),
        os.path.join(os.environ.get('ProgramFiles', r'C:\Program Files'), 'Debugging Tools for Windows (x64)'),
        os.path.join(os.environ.get('ProgramFiles(x86)', r'C:\Program Files (x86)'), 'Debugging Tools for Windows (x86)'),
        os.path.join(os.environ.get('ProgramW6432', r'C:\Program Files'), 'Debugging Tools for Windows (x64)'), )
    for dtw_location in dtw_locations:
        symstore = os.path.join(dtw_location, 'symstore.exe')
        if os.path.isfile(symstore):
            break
    else:
        raise Exception("Couldn't find symstore.exe in " + ' '.join(dtw_locations) + ". Is Debugging Tools for Windows installed?")

    (hProcess, hThread, processId, threadId) = win32process.CreateProcess(symstore, 
        "symstore.exe add /r /f \"%s\" /s \"%s\" /t \"%s\" /v \"%s\"" % (destdir, destination, job_name, version), None, None, True, win32process.CREATE_BREAKAWAY_FROM_JOB, None, None, startup)
    win32api.CloseHandle(hThread)
        
    # Don't need to wait here, but it doesn't take long, and we can remove the temp dir
    import win32event
    win32event.WaitForSingleObject(hProcess, win32event.INFINITE)
    exit_code = win32process.GetExitCodeProcess(hProcess)
    log("symstore exited with code " + str(exit_code))
    win32api.CloseHandle(startup.hStdOutput)
    with file(log_path, "r") as log_file:
        for line in log_file:
            log("symstore: " + line.rstrip('\r\n'))
    import shutil
    shutil.rmtree(destdir)

    win32api.CloseHandle(hProcess)

    # print "\n".join(open(os.path.join(destdir, 'log'), 'r'))

    log("finish: %s" % datetime.datetime.now().isoformat())
    log("elapsed time: %d seconds" % (datetime.datetime.now() - starttime).seconds)
    return exit_code

if __name__=='__main__':
    import argparse
    parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]))
    parser.add_argument("--destination", "-s", help='symstore /s switch', required=True)
    parser.add_argument("--source", help='Source directory. Defaults to cwd', default=os.getcwd())
    parser.add_argument("--job_name", "-t", help='symstore /t switch', default='copy_pdbs.py')
    parser.add_argument("--version", "-v", help='symstore /v switch', default='0')

    args = vars(parser.parse_args())
    sys.exit(main(**args))
