import os
import os.path
import sys

import win32api
import win32process
import win32event
import win32job
import base64

if __name__=='__main__':
    if hasattr(sys, "frozen"):
        this_dir = os.path.dirname(win32api.GetModuleFileName(None))
    else:
        this_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

    startup = win32process.STARTUPINFO()
    # FIXME: this arg quoting won't work with trailing slashes. See MSDN: CommandLineToArgvW
    (hProcess, hThread, processId, threadId) = win32process.CreateProcess(None, ' '.join(['"' + arg + '"' for arg in sys.argv[1:]]), None, None, True, win32process.CREATE_SUSPENDED | win32process.CREATE_BREAKAWAY_FROM_JOB, None, None, startup)

    # assert not win32job.IsProcessInJob(hProcess, None)

    hJob = win32job.CreateJobObject(None, "")
    extended_info = win32job.QueryInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation)
    extended_info['BasicLimitInformation']['LimitFlags'] = win32job.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | win32job.JOB_OBJECT_LIMIT_BREAKAWAY_OK
    win32job.SetInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation, extended_info)
    win32job.AssignProcessToJobObject(hJob, hProcess)
    # n.b. intentionally leak hJob. Otherwise, when running on Windows Server 2008R2 SP1, AssignProcessToJobObject closes hJob (try !handle
    # in windbg before and after), and we crash with invalid handle in CloseHandle on exit
    hJob.Detach()

    win32process.ResumeThread(hThread)

    status = win32event.WAIT_TIMEOUT
    while status == win32event.WAIT_TIMEOUT:
        status = win32event.WaitForSingleObject(hProcess, 500) # timeout so ctrl-c works
    #win32process.TerminateProcess(hProcess, 1)
    sys.exit(win32process.GetExitCodeProcess(hProcess))
