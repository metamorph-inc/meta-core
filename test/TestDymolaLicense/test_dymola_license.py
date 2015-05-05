import os
import os.path
import sys

import win32api
import win32process
import win32event
import win32job
import win32file
import win32security

def get_python_exe():
    import _winreg
    with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\Meta', 0, _winreg.KEY_READ) as key:
        meta_path = _winreg.QueryValueEx(key, 'META_PATH')[0]
        return os.path.join(meta_path, r'bin\Python27\Scripts\python.exe')

if __name__=='__main__':
    command_line = "\"" + get_python_exe() + "\" scripts\simulate.py --tool Dymola"
    sa = win32security.SECURITY_ATTRIBUTES()
    sa.bInheritHandle = True
    startup = win32process.STARTUPINFO()
    startup.dwFlags += win32process.STARTF_USESTDHANDLES
    startup.hStdError = startup.hStdOutput = win32file.CreateFile("test_dymola_output.txt", win32file.GENERIC_WRITE, 0, sa, win32file.CREATE_ALWAYS, win32file.FILE_ATTRIBUTE_NORMAL, None)
    startup.hStdInput = win32api.GetStdHandle(win32api.STD_INPUT_HANDLE)

    (hProcess, hThread, processId, threadId) = win32process.CreateProcess(None, command_line, None, None, True, win32process.CREATE_BREAKAWAY_FROM_JOB, None, None, startup)

    assert not win32job.IsProcessInJob(hProcess, None)

    hJob = win32job.CreateJobObject(None, "")
    extended_info = win32job.QueryInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation)
    extended_info['BasicLimitInformation']['LimitFlags'] = win32job.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
    win32job.SetInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation, extended_info)
    win32job.AssignProcessToJobObject(hJob, hProcess)

    status = win32event.WaitForSingleObject(hProcess, 45000)
    win32api.CloseHandle(startup.hStdError)
    if status == win32event.WAIT_TIMEOUT:
        print "Dymola timed out. Likely the license could not be acquired"
        sys.exit(2)

    if win32process.GetExitCodeProcess(hProcess) != 0:
        print "Dymola exited with code " + str(win32process.GetExitCodeProcess(hProcess))
        sys.exit(1)

    print "Test executed ok"
    sys.exit(0)
