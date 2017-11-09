using System;
using System.Collections.Generic;
using System.Threading;
using JobManager;

namespace JobManagerFramework
{
    public class JobServerImpl : JobServer
    {
        public JobServerImpl()
        {
            this.JobCollectionDone += (job) =>
            {
                lock (this)
                {
                    JobCollectionById.Remove(job.id);
                }
            };
        }

        public bool WipeWorkspaceOnSuccess { get; set; }
        public bool DeleteJobOnSuccess { get; set; }

        public ManualResetEvent handlersAdded = new ManualResetEvent(false);

        public delegate void SoTActionHandler(SoTImpl sot);
        public event SoTActionHandler SoTAdded;

        public event JobImpl.JobActionHandler JobAdded;

        public event Action<JobCollectionImpl> JobCollectionAdded;
        public event Action<JobCollectionImpl> JobCollectionDone;

        public class JobCollectionImpl : JobCollection
        {
            private JobServerImpl server;
            internal string id;
            public JobCollectionImpl(JobServerImpl server, string id)
            {
                this.server = server;
                this.id = id;
            }

            public List<Job> Jobs = new List<Job>();
            public override void AddJob(Job job)
            {
                Jobs.Add(job);
            }

            public List<SoT> SoTs = new List<SoT>();

            public override void AddSoT(SoT sot)
            {
                SoTs.Add(sot);
            }

            public override void Done()
            {
                server.handlersAdded.WaitOne();
                if (server.JobCollectionDone != null)
                {
                    server.JobCollectionDone(this);
                }
            }
        }

        Dictionary<string, JobCollectionImpl> JobCollectionById = new Dictionary<string, JobCollectionImpl>();
        public override JobCollection CreateAndAddJobCollection(string id)
        {
            handlersAdded.WaitOne();

            lock (this)
            {
                JobCollectionImpl jobCollection;
                if (JobCollectionById.TryGetValue(id, out jobCollection))
                {
                    return jobCollection;
                }
                else
                {
                    jobCollection = new JobCollectionImpl(this, id);
                    JobCollectionById[id] = jobCollection;
                    if (this.JobCollectionAdded != null)
                    {
                        JobCollectionAdded(jobCollection);
                    }
                    return jobCollection;
                }
            }
        }

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

            JobAdded?.Invoke((JobImpl)job, job.Status);
        }


        public override void AddSoT(SoT sot)
        {
            if (SoTs == null)
            {
                SoTs = new List<SoT>();
            }
            SoTs.Add(sot);

            handlersAdded.WaitOne();

            SoTAdded?.Invoke((SoTImpl)sot);
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
    }
}
