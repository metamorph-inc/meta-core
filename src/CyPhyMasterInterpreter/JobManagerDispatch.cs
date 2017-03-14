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

namespace CyPhyMasterInterpreter
{
    public class JobManagerDispatch
    {
        public Uri JobServerConnection = new Uri("ipc://MetaJobManager/JobServer");

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
            ComComponent interpreter,
            Job.TypeEnum type = Job.TypeEnum.Command)
        {
            // TODO: cut down the number of input variables. interpreter and title should be enough
            JobServer manager;
            Job j = CreateJob(out manager, projectDirectory);

            j.RunCommand = runCommand;
            j.Title = title;
            j.TestBenchName = testbenchName;
            j.WorkingDirectory = workingDirectory;
            j.Type = type;

            // TODO: allow empty Labels
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
            return CreateOnServer(out manager, server => server.CreateJob(), projectDirectory);
        }

        private SoT CreateSoT(out JobServer manager, string projectDirectory)
        {
            return CreateOnServer(out manager, server => server.CreateSoT(), projectDirectory);
        }

        public void StartJobManager(string projectDirectory)
        {
            try
            {
                var manager = (JobServer)Activator.GetObject(typeof(JobServer), JobServerConnection.OriginalString);
                var tmp = manager.JenkinsUrl;
            }
            catch (RemotingException)
            {
                // n.b. Assembly.Location is wrong with Shadow Copy enabled
                string assemblyDir = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);
                string exe = Path.Combine(assemblyDir, "JobManager.exe");
                if (!File.Exists(exe))
                    exe = Path.Combine(assemblyDir, "..\\..\\..\\JobManager\\JobManager\\bin\\Release\\JobManager.exe");
                if (!File.Exists(exe))
                    exe = Path.Combine(assemblyDir, "..\\..\\..\\JobManager\\JobManager\\bin\\Debug\\JobManager.exe");
                if (File.Exists(exe))
                {
                    Process proc = new Process();
                    proc.StartInfo.UseShellExecute = false;
                    proc.StartInfo.FileName = exe;
                    proc.StartInfo.RedirectStandardOutput = true;
                    proc.StartInfo.WorkingDirectory = projectDirectory;
                    proc.Start();
                    proc.WaitForInputIdle(20 * 1000);
                    proc.StandardOutput.ReadLine(); // matches Console.Out.WriteLine("JobManager has started"); in JobManager
                    //System.Threading.Thread.Sleep(3 * 1000);
                }
                else
                {
                    throw new Exception("Job Manager was not found on your computer. Make sure your META installer is healthy.");
                }
            }
        }
    }
}
