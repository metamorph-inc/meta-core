using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Linq;
using System.Threading;

namespace JobManager
{
    public abstract class Job : MarshalByRefObject
    {
        // TODO: change this id to guid
        public int Id { get; protected set; }
        public string Title { get; set; }
        public string TestBenchName { get; set; }
        public string WorkingDirectory { get; set; }
        public string RunCommand { get; set; }

        public DateTime TimePosted { get; set; }
        public DateTime TimeStart { get; set; }
        public DateTime TimeDone { get; set; }

        public TimeSpan TimeInQueue { get; set; }
        public TimeSpan TimeTotal { get; set; }

        public enum StatusEnum
        {
            WaitingForStart,
            Ready,
            QueuedLocal,
            RunningLocal,
            UploadPackage,
            ZippingPackage,
            PostedToServer,
            StartedOnServer,
            QueuedOnServer,
            RunningOnServer,
            DownloadResults,
            RedownloadQueued,
            AbortOnServerRequested,
            Succeeded,
            FailedToUploadServer,
            FailedToDownload,
            FailedAbortOnServer,
            FailedExecution,
            Failed,
            FailedAbortByUser,
        }

        public abstract StatusEnum Status
        {
            get;
            set;
        }

        public enum TypeEnum
        {
            Command,
            Matlab,
            CAD,
        }

        public bool IsFailed()
        {
            return Job.IsFailedStatus(Status);
        }

        public static bool IsFailedStatus(Job.StatusEnum status)
        {
            return status == StatusEnum.FailedExecution ||
                   status == StatusEnum.FailedAbortOnServer ||
                   status == StatusEnum.FailedAbortByUser ||
                   status == StatusEnum.FailedToUploadServer ||
                   status == StatusEnum.FailedToDownload ||
                   status == StatusEnum.Failed;
        }

        public TypeEnum Type { get; set; }

        public string Labels { get; set; }

        public string BuildQuery { get; set; }

        /// <summary>
        /// zip.py server side hook
        /// </summary>
        public string ResultsZip { get; set; }

        public const string LabelVersion = "14.13";
        public const string DefaultLabels = "Windows" + LabelVersion;
        public const string DefaultBuildQuery = "";
    }

    public abstract class JobServer : MarshalByRefObject
    {
        public string JenkinsUrl { get; set; }
        public string UserName { get; set; }
        public bool IsRemote { get; set; }

        public abstract Job CreateJob();

        public abstract SoT CreateSoT();

        public abstract JobCollection CreateAndAddJobCollection();

        public List<Job> Jobs { get; set; }
        public List<SoT> SoTs { get; set; }

        public abstract void AddJob(Job job);

        public abstract void AddSoT(SoT sot);
    }

    /*
     * JobCollection corresponds to one run of the MasterInterpreter 
     */
    public abstract class JobCollection : MarshalByRefObject
    {
        public abstract void AddJob(Job job);
        public abstract void AddSoT(SoT sot);

        public abstract void Done();
    }
}
