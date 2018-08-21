using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using JobManager;
using META;
using System.IO;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.Remoting;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Threading;
using System.Security.Principal;

namespace CyPhyMasterInterpreter
{
    public class JobManagerDispatch
    {
        public JobManagerDispatch(string JobCollectionID)
        {
            this.JobCollectionID = JobCollectionID;
        }

        Uri JobServerConnection
        {
            get
            {
                return new Uri("ipc://MetaJobManager" + WindowsIdentity.GetCurrent().User + "/JobServer");
            }
        }

        public Queue<KeyValuePair<JobServer, Job>> jobsToAdd = new Queue<KeyValuePair<JobServer, Job>>();
        public void AddJobs()
        {
            try
            {
                foreach (var j in jobsToAdd)
                {
                    j.Value.Status = Job.StatusEnum.Ready;
                    j.Key.AddJob(j.Value);
                }
            }
            finally
            {
                jobsToAdd.Clear();
            }
        }

        public Queue<KeyValuePair<JobServer, SoT>> sotsToAdd = new Queue<KeyValuePair<JobServer, SoT>>();
        public void AddSoTs()
        {
            try
            {
                foreach (var sot in sotsToAdd)
                {
                    sot.Key.AddSoT(sot.Value);
                }
            }
            finally
            {
                sotsToAdd.Clear();
            }
        }

        public bool EnqueueJob(
            string runCommand,
            string title,
            string testbenchName,
            string workingDirectory,
            string projectDirectory,
            ComComponent interpreter)
        {
            // TODO: cut down the number of input variables. interpreter and title should be enough
            JobServer manager;
            Job j = CreateJob(out manager, projectDirectory);

            j.RunCommand = runCommand;
            j.Title = title;
            j.TestBenchName = testbenchName;
            j.WorkingDirectory = workingDirectory;

            j.Labels = String.IsNullOrWhiteSpace(interpreter.result.Labels) ?
                Job.DefaultLabels :
                interpreter.result.Labels;

            j.BuildQuery = String.IsNullOrWhiteSpace(interpreter.result.BuildQuery) ?
                Job.DefaultBuildQuery :
                interpreter.result.BuildQuery;

            if (String.IsNullOrWhiteSpace(interpreter.result.ZippyServerSideHook) == false)
            {
                j.ResultsZip = interpreter.result.ZippyServerSideHook as string;
            }

            jobsToAdd.Enqueue(new KeyValuePair<JobServer, Job>(manager, j));
            return true;
        }

        public bool EnqueueSoT(string workingDirectory, string projectDirectory)
        {
            JobServer manager;
            SoT sot = CreateSoT(out manager, projectDirectory);
            sot.WorkingDirectory = workingDirectory;

            sotsToAdd.Enqueue(new KeyValuePair<JobServer, SoT>(manager, sot));
            return true;
        }

        private T CreateOnServer<T>(out JobServer manager, Func<JobServer, T> create, string projectDirectory)
        {
            T sot;
            manager = (JobServer)Activator.GetObject(typeof(JobServer), JobServerConnection.OriginalString);
            sot = create(manager);
            return sot;
        }

        private Job CreateJob(out JobServer manager, string projectDirectory)
        {
            var job = CreateOnServer(out manager, server => server.CreateJob(), projectDirectory);
            JobCollection.AddJob(job);
            return job;
        }

        private SoT CreateSoT(out JobServer manager, string projectDirectory)
        {
            var sot = CreateOnServer(out manager, server => server.CreateSoT(), projectDirectory);
            JobCollection.AddSoT(sot);
            return sot;
        }

        public void Done()
        {
            JobCollection.Done();
        }

        JobServer Server;
        public JobCollection JobCollection;
        private string JobCollectionID;

        public bool Started { get; private set; }

        public void StartJobManager(string projectDirectory)
        {
            try
            {
                Server = (JobServer)Activator.GetObject(typeof(JobServer), JobServerConnection.OriginalString);
                // the proxy won't throw until property access/method call
                JobCollection = Server.CreateAndAddJobCollection(JobCollectionID);
                Started = true;
            }
            catch (RemotingException)
            {
                // n.b. Assembly.Location is wrong with Shadow Copy enabled
                string assemblyDir = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);
                string exe = Path.Combine(assemblyDir, "JobManager.exe");
                if (!File.Exists(exe))
                {
                    exe = Path.GetFullPath(Path.Combine(assemblyDir, "..\\..\\..\\JobManager\\JobManager\\bin\\Release\\JobManager.exe"));
                }

                if (!File.Exists(exe))
                {
                    exe = Path.Combine(assemblyDir, "..\\..\\..\\JobManager\\JobManager\\bin\\Debug\\JobManager.exe");
                }

                if (File.Exists(exe))
                {
                    CreateProcessWithExplicitHandles(exe, exe, projectDirectory);

                    Server = (JobServer)Activator.GetObject(typeof(JobServer), JobServerConnection.OriginalString);
                    JobCollection = Server.CreateAndAddJobCollection(JobCollectionID);
                    Started = true;
                }
                else
                {
                    throw new Exception("Job Manager was not found on your computer. Make sure your META installer is healthy.");
                }
            }
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct HandleList
        {
            public IntPtr handle0;
            public IntPtr handle1;
            public IntPtr handle2;
            public IntPtr handle3;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool UpdateProcThreadAttribute(IntPtr lpAttributeList, uint dwFlags, IntPtr Attribute, [In] ref HandleList handles, IntPtr cbSize, IntPtr lpPreviousValue, IntPtr lpReturnSize);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool InitializeProcThreadAttributeList(IntPtr lpAttributeList, uint dwAttributeCount, uint dwFlags, ref IntPtr lpSize);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool DeleteProcThreadAttributeList(IntPtr lpAttributeList);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct STARTUPINFO
        {
            public Int32 cb;
            public string lpReserved;
            public string lpDesktop;
            public string lpTitle;
            public Int32 dwX;
            public Int32 dwY;
            public Int32 dwXSize;
            public Int32 dwYSize;
            public Int32 dwXCountChars;
            public Int32 dwYCountChars;
            public Int32 dwFillAttribute;
            public Int32 dwFlags;
            public Int16 wShowWindow;
            public Int16 cbReserved2;
            public IntPtr lpReserved2;
            public IntPtr hStdInput;
            public IntPtr hStdOutput;
            public IntPtr hStdError;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct STARTUPINFOEX
        {
            public STARTUPINFO StartupInfo;
            public IntPtr lpAttributeList;
        }


        private const uint PROC_THREAD_ATTRIBUTE_ADDITIVE = 0x00040000;

        private const uint PROC_THREAD_ATTRIBUTE_INPUT = 0x00020000;

        private const uint PROC_THREAD_ATTRIBUTE_NUMBER = 0x0000FFFF;

        private const uint PROC_THREAD_ATTRIBUTE_THREAD = 0x00010000;

        private static IntPtr ProcThreadAttributeValue(PROC_THREAD_ATTRIBUTE_NUM Number, bool Thread, bool Input, bool Additive) =>
            (IntPtr)(((uint)Number & PROC_THREAD_ATTRIBUTE_NUMBER) | (Thread ? PROC_THREAD_ATTRIBUTE_THREAD : 0) |
            (Input ? PROC_THREAD_ATTRIBUTE_INPUT : 0) | (Additive ? PROC_THREAD_ATTRIBUTE_ADDITIVE : 0));

        private const int STARTF_USESTDHANDLES = 0x00000100;

        private const uint EXTENDED_STARTUPINFO_PRESENT = 0x00080000;

        private enum PROC_THREAD_ATTRIBUTE_NUM : uint
        {
            ProcThreadAttributeParentProcess = 0,
            ProcThreadAttributeHandleList = 2,
            ProcThreadAttributeGroupAffinity = 3,
            ProcThreadAttributePreferredNode = 4,
            ProcThreadAttributeIdealProcessor = 5,
            ProcThreadAttributeUmsThread = 6,
            ProcThreadAttributeMitigationPolicy = 7,
            ProcThreadAttributeSecurityCapabilities = 9,
            ProcThreadAttributeProtectionLevel = 11,
            ProcThreadAttributeJobList = 13,
            ProcThreadAttributeChildProcessPolicy = 14,
            ProcThreadAttributeAllApplicationPackagesPolicy = 15,
            ProcThreadAttributeWin32kFilter = 16,
            ProcThreadAttributeSafeOpenPromptOriginClaim = 17,
            ProcThreadAttributeDesktopAppPolicy = 18,
        }

        public static readonly IntPtr PROC_THREAD_ATTRIBUTE_HANDLE_LIST = ProcThreadAttributeValue(PROC_THREAD_ATTRIBUTE_NUM.ProcThreadAttributeHandleList, false, true, false);

        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool CreateProcess(
            string lpApplicationName,
            string lpCommandLine,
            /* ref SECURITY_ATTRIBUTES lpProcessAttributes, */ IntPtr lpProcessAttributes,
            /* ref SECURITY_ATTRIBUTES lpThreadAttributes, */ IntPtr lpThreadAttributes,
            bool bInheritHandles,
            uint dwCreationFlags,
            IntPtr lpEnvironment,
            string lpCurrentDirectory,
            [In] ref STARTUPINFOEX lpStartupInfo,
            out PROCESS_INFORMATION lpProcessInformation);

        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES
        {
            public int nLength;
            // public unsafe byte* lpSecurityDescriptor;
            public IntPtr lpSecurityDescriptor;
            public int bInheritHandle;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct PROCESS_INFORMATION
        {
            public IntPtr hProcess;
            public IntPtr hThread;
            public int dwProcessId;
            public int dwThreadId;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern UInt32 WaitForSingleObject(IntPtr hHandle, UInt32 dwMilliseconds);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool CreatePipe(out IntPtr hReadPipe, out IntPtr hWritePipe, [In] ref SECURITY_ATTRIBUTES lpPipeAttributes, uint nSize);

        IntPtr INVALID_HANDLE_VALUE = (IntPtr)(-1);

        void CreateProcessWithExplicitHandles(string lpApplicationName, string lpCommandLine, string lpCurrentDirectory)
        {
            IntPtr lpAttributeList = IntPtr.Zero;
            IntPtr size = IntPtr.Zero;
            InitializeProcThreadAttributeList(lpAttributeList, 1, 0, ref size);
            lpAttributeList = Marshal.AllocHGlobal(size.ToInt32());
            if (!InitializeProcThreadAttributeList(lpAttributeList, 1, 0, ref size))
            {
                throw new Win32Exception(Marshal.GetLastWin32Error(), "InitializeProcThreadAttributeList failed");
            }
            try
            {
                STARTUPINFOEX info = new STARTUPINFOEX();
                info.StartupInfo.cb = Marshal.SizeOf(info);
                info.lpAttributeList = lpAttributeList;
                info.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
                IntPtr readPipe;
                IntPtr writePipe;
                SECURITY_ATTRIBUTES security_attributes = new SECURITY_ATTRIBUTES();
                security_attributes.nLength = Marshal.SizeOf(security_attributes);
                security_attributes.lpSecurityDescriptor = IntPtr.Zero;
                security_attributes.bInheritHandle = 1;

                if (!CreatePipe(out readPipe, out writePipe, ref security_attributes, 0))
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error(), "CreatePipe failed");
                }

                IntPtr readPipe2, writePipe2;
                if (!CreatePipe(out readPipe2, out writePipe2, ref security_attributes, 0))
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error(), "CreatePipe failed");
                }
                info.StartupInfo.hStdError = writePipe;
                info.StartupInfo.hStdOutput = writePipe;
                info.StartupInfo.hStdInput = INVALID_HANDLE_VALUE;

                HandleList handles = new HandleList()
                {
                    handle0 = writePipe
                };
                if (!UpdateProcThreadAttribute(lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
                       ref handles, (IntPtr)(1 * Marshal.SizeOf(typeof(IntPtr))), IntPtr.Zero, IntPtr.Zero))
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error(), "UpdateProcThreadAttribute failed");
                }

                PROCESS_INFORMATION lpProcessInformation;
                if (!CreateProcess(lpApplicationName,
                               lpCommandLine,
                               IntPtr.Zero,
                               IntPtr.Zero,
                               true,
                               /* CREATE_UNICODE_ENVIRONMENT | */ EXTENDED_STARTUPINFO_PRESENT,
                               IntPtr.Zero,
                               lpCurrentDirectory,
                               ref info,
                               out lpProcessInformation))
                {
                    CloseHandle(readPipe);
                    CloseHandle(writePipe);
                    throw new Win32Exception(Marshal.GetLastWin32Error(), "CreateProcess " + lpApplicationName + " failed");
                }
                CloseHandle(writePipe);
                CloseHandle(lpProcessInformation.hThread);
                CloseHandle(lpProcessInformation.hProcess);

                StringBuilder output = new StringBuilder();
                using (var stream = new FileStream(readPipe, FileAccess.Read))
                {
                    byte[] buffer = new byte[4096];
                    while (true)
                    {
                        int n = stream.Read(buffer, 0, buffer.Length);
                        if (n == 0)
                        {
                            break;
                        }
                        output.Append(Encoding.Default.GetString(buffer, 0, n));

                        if (buffer.Where(c => c == '\n').Count() > 0) // matches Console.Out.WriteLine("JobManager has started"); in JobManager
                        {
                            break;
                        }
                    }
                }
            }
            finally
            {
                DeleteProcThreadAttributeList(lpAttributeList);
                Marshal.FreeHGlobal(lpAttributeList);
            }
        }

    }
}
