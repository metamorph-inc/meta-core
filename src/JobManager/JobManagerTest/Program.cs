using System;
using System.Collections.Generic;
using System.Text;
using JobManager;

namespace JobManagerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            JobServer manager = (JobServer)Activator.GetObject(typeof(JobServer),
                                 "tcp://localhost:35010/JobServer");
            Job j = manager.CreateJob();
            j.RunCommand = "cmd.exe";
            j.Title = "Test cmd.exe";
            j.WorkingDirectory = "c:\\";
            manager.AddJob(j);
        }
    }
}
