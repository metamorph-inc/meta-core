using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace CyPhyGUIs
{
    public class JobObjectPinvoke
    {

        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES
        {
            public int nLength;
            // public unsafe byte* lpSecurityDescriptor;
            public IntPtr lpSecurityDescriptor;
            public int bInheritHandle;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct JOBOBJECT_ASSOCIATE_COMPLETION_PORT
        {
            public IntPtr CompletionKey;
            public IntPtr /* HANDLE */ CompletionPort;
        }

        public enum JOBOBJECTINFOCLASS
        {
            // AssociateCompletionPortInformation = 7,
            // BasicLimitInformation = 2,
            // BasicUIRestrictions = 4,
            // EndOfJobTimeInformation = 6,
            ExtendedLimitInformation = 9,
            // SecurityLimitInformation = 5,
            // GroupInformation = 11
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool QueryInformationJobObject(IntPtr hJob, JOBOBJECTINFOCLASS type, ref JOBOBJECT_EXTENDED_LIMIT_INFORMATION limitInfo, Int32 cbJobObjectInfoLength, ref Int32 lpReturnLength);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool AssignProcessToJobObject(IntPtr hJob, IntPtr hProcess);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool SetInformationJobObject(IntPtr hJob,
           JOBOBJECTINFOCLASS JobObjectInfoClass,
           /* IntPtr lpJobObjectInfo, */
           // ref JOBOBJECT_ASSOCIATE_COMPLETION_PORT info,
           [In] ref JOBOBJECT_EXTENDED_LIMIT_INFORMATION info,
           uint cbJobObjectInfoLength);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        // static extern IntPtr CreateJobObject([In] ref SECURITY_ATTRIBUTES lpJobAttributes, string lpName);
        static extern IntPtr CreateJobObject(IntPtr lpJobAttributes, string lpName);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr CreateIoCompletionPort(IntPtr FileHandle,
           IntPtr ExistingCompletionPort, UIntPtr CompletionKey,
           uint NumberOfConcurrentThreads);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GetQueuedCompletionStatus(IntPtr CompletionPort, out uint
           lpNumberOfBytes, out IntPtr lpCompletionKey, out IntPtr lpOverlapped,
           uint dwMilliseconds);

        static IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
        const UInt32 INFINITE = 0xFFFFFFFF;


        public static IntPtr AssignProcessToKillOnCloseJob(Process process)
        {
            IntPtr job = CreateKillOnCloseJob();
            AssignProcessToJobObject(process, job);

            return job;
        }

        public static void AssignProcessToJobObject(Process process, IntPtr job)
        {
            if (!AssignProcessToJobObject(job, process.Handle))
            {
                var error = Marshal.GetLastWin32Error();
                CloseHandle(job);
                throw new Win32Exception(error);
            }
        }

        public static IntPtr CreateKillOnCloseJob()
        {
            IntPtr job = CreateJobObject(IntPtr.Zero, null);
            if (job == IntPtr.Zero)
            {
                throw new Win32Exception(Marshal.GetLastWin32Error());
            }
            var info = new JOBOBJECT_EXTENDED_LIMIT_INFORMATION();
            int returnLength = 0;
            if (!QueryInformationJobObject(job, JOBOBJECTINFOCLASS.ExtendedLimitInformation, ref info, Marshal.SizeOf(info), ref returnLength))
            {
                var error = Marshal.GetLastWin32Error();
                CloseHandle(job);
                throw new Win32Exception(error);
            }
            info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_BREAKAWAY_OK;

            if (!SetInformationJobObject(job, JOBOBJECTINFOCLASS.ExtendedLimitInformation, ref info, (uint)Marshal.SizeOf(info)))
            {
                var error = Marshal.GetLastWin32Error();
                CloseHandle(job);
                throw new Win32Exception(error);
            }

            return job;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 8)]
        public struct NativeOverlapped
        {
            private IntPtr InternalLow;
            private IntPtr InternalHigh;
            public long Offset;
            public IntPtr EventHandle;
        }

        public enum JobObjectMsg : uint
        {
            END_OF_JOB_TIME = 1,
            END_OF_PROCESS_TIME = 2,
            ACTIVE_PROCESS_LIMIT = 3,
            ACTIVE_PROCESS_ZERO = 4,
            NEW_PROCESS = 6,
            EXIT_PROCESS = 7,
            ABNORMAL_EXIT_PROCESS = 8,
            PROCESS_MEMORY_LIMIT = 9,
            JOB_MEMORY_LIMIT = 10,
            NOTIFICATION_LIMIT = 11,
            JOB_CYCLE_TIME_LIMIT = 12,
        }


        [StructLayout(LayoutKind.Sequential)]
        public struct IO_COUNTERS
        {
            public UInt64 ReadOperationCount;
            public UInt64 WriteOperationCount;
            public UInt64 OtherOperationCount;
            public UInt64 ReadTransferCount;
            public UInt64 WriteTransferCount;
            public UInt64 OtherTransferCount;
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct JOBOBJECT_BASIC_LIMIT_INFORMATION
        {
            public Int64 PerProcessUserTimeLimit;
            public Int64 PerJobUserTimeLimit;
            public Int32 LimitFlags;
            public UIntPtr MinimumWorkingSetSize;
            public UIntPtr MaximumWorkingSetSize;
            public Int32 ActiveProcessLimit;
            public IntPtr Affinity;
            public Int32 PriorityClass;
            public Int32 SchedulingClass;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct JOBOBJECT_EXTENDED_LIMIT_INFORMATION
        {
            public JOBOBJECT_BASIC_LIMIT_INFORMATION BasicLimitInformation;
            public IO_COUNTERS IoInfo;
            public UIntPtr ProcessMemoryLimit;
            public UIntPtr JobMemoryLimit;
            public UIntPtr PeakProcessMemoryUsed;
            public UIntPtr PeakJobMemoryUsed;
        }

        const Int32 JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE = 0x00002000;
        const Int32 JOB_OBJECT_LIMIT_BREAKAWAY_OK = 0x00000800;
    }
}
