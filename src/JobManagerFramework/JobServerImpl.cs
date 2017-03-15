using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using JobManager;

namespace JobManagerFramework
{
    public class JobServerImpl : JobServer
    {
        public bool WipeWorkspaceOnSuccess { get; set; }
        public bool DeleteJobOnSuccess { get; set; }

        public ManualResetEvent handlersAdded = new ManualResetEvent(false);

        public delegate void SoTActionHandler(SoTImpl sot);
        public event SoTActionHandler SoTAdded;

        public event JobImpl.JobActionHandler JobAdded;

        public override Job CreateJob()
        {
            Job job = new JobImpl(this);
            return job;
        }

        public override SoT CreateSoT()
        {
            SoT sot = new SoTImpl(this);
            return sot;
        }

        public override void AddJob(Job job)
        {
            if (Jobs == null)
            {
                Jobs = new List<Job>();
            }
            Jobs.Add(job);

            handlersAdded.WaitOne();

            if (JobAdded != null)
            {
                JobAdded.Invoke((JobImpl)job, job.Status);
            }
        }


        public override void AddSoT(SoT sot)
        {
            if (SoTs == null)
            {
                SoTs = new List<SoT>();
            }
            SoTs.Add(sot);

            handlersAdded.WaitOne();

            if (SoTAdded != null)
            {
                SoTAdded.Invoke((SoTImpl)sot);
            }
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
    }
}
