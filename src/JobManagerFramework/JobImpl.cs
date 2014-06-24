using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using JobManager;

namespace JobManagerFramework
{
    public class JobImpl : Job
    {
        /// <summary>
        /// Global number of jobs.
        /// </summary>
        public static int NumJobs = 0;

        public bool RerunEnabled { get; set; }

        //TODO: Move this into library
        public JobManager.RemoteJobInfo remoteInfo { get; set; }

        public string GetURLOverride { get; set; }

        public Statistics Stat { get; set; }

        private JobServer Server { get; set; }

        public JobImpl(JobServer server)
        {
            this.Server = server;
            this.Id = NumJobs++;
            this.TimePosted = DateTime.Now;
            this.TimeInQueue = TimeSpan.Zero;
            this.TimeTotal = TimeSpan.Zero;

            this.Labels = DefaultLabels;
            this.BuildQuery = DefaultBuildQuery;
            //TODO: Move this into library
            //this.ResultsZip = Encoding.UTF8.GetString(Properties.Resources.zip);

            this.RerunEnabled = true;

            this.Stat = new Statistics()
            {
                Id = this.Id,
                JobReceived = DateTime.Now.ToString("u"),
            };
        }

        /// <summary>
        /// This funciton puts changes the job's status to queued if it was waiting for start.
        /// </summary>
        public void Start()
        {
            if (this.Status == Job.StatusEnum.WaitingForStart)
            {
                this.Status = StatusEnum.Ready;
            }
        }

        public delegate void JobActionHandler(JobImpl job, Job.StatusEnum status);
        public event JobActionHandler JobStatusChanged;

        private StatusEnum status = StatusEnum.Ready;
        public override StatusEnum Status
        {
            get { return status; }
            set
            {
                if (status != value)
                {
                    status = value;
                    Trace.TraceInformation("Name of Job : {0}, New Status : {1}", this.Title, status);

                    if (JobStatusChanged != null)
                    {
                        JobStatusChanged.Invoke(this, status);
                    }
                }

                // If something goes wrong here, catch any exception and print as trace warning
                try
                {
                    this.UpdateStatistics(status);
                    if (status == StatusEnum.Succeeded || this.IsFailed() || status == StatusEnum.Ready || status == StatusEnum.ZippingPackage)
                    {
                        this.SaveStatistics();
                    }
                }
                catch (Exception exc)
                {
                    Trace.TraceWarning(exc.ToString());
                }
            }
        }

        private void UpdateStatistics(StatusEnum newStatus)
        {
            Statistics.Execution thisExecution;

            if (newStatus == Job.StatusEnum.Ready)
            {
                thisExecution = new Statistics.Execution();
                this.Stat.Executions.Add(thisExecution);
                thisExecution.StartTime = DateTime.Now.ToString("u");
            }
            else
            {
                thisExecution = this.Stat.Executions.LastOrDefault();
                if (thisExecution == null)
                {
                    return;
                }
            }

            if (newStatus == StatusEnum.RunningLocal ||
                newStatus == StatusEnum.RunningOnServer)
            {
                TimeStart = DateTime.Now;
                TimeInQueue = TimeStart - TimePosted;
                thisExecution.StartTime = DateTime.Now.ToString("u");
            }
            else if (newStatus == StatusEnum.Succeeded || this.IsFailed())
            {
                TimeDone = DateTime.Now;
                if (TimeDone > TimeStart)
                    TimeTotal = TimeDone - TimeStart;
            }

            // update the execution statistics (queued -> new execution)

            if (thisExecution != null)
            {
                thisExecution.StatusTraces.Add(new Statistics.Execution.StatusTrace()
                {
                    Status = newStatus.ToString(),
                    TimeStamp = DateTime.Now.ToString("u")
                });
            }

        }

        public string SaveStatistics(bool saveToFile = true, string fileName = null)
        {

            this.Stat.JobName = this.Title;
            this.Stat.RunCommand = this.RunCommand;
            this.Stat.UserId = this.Server.UserName;
            this.Stat.IsRemote = this.Server.IsRemote;
            this.Stat.VFUrl = this.Server.JenkinsUrl;

            // This is only used if saving/reading to/from file
            if (string.IsNullOrEmpty(fileName))
            {
                fileName = Path.Combine(this.WorkingDirectory, "stat.json");
            }


            // Load stat-file to collect ToolSpecifics
            if (File.Exists(fileName))
            {
                using (StreamReader reader = new StreamReader(fileName))
                {
                    var readStatistics = Newtonsoft.Json.JsonConvert.DeserializeObject<Statistics>(
                        reader.ReadToEnd(),
                        new Newtonsoft.Json.Converters.IsoDateTimeConverter()
                        {
                            DateTimeStyles = System.Globalization.DateTimeStyles.AdjustToUniversal
                        });
                    if (readStatistics.ToolSpecifics != null)
                    {
                        //Trace.TraceInformation(readStatistics.ToolSpecifics.ToString());
                        this.Stat.ToolSpecifics = readStatistics.ToolSpecifics;
                        Trace.TraceInformation("Statistics.ToolSpecifics read in from {0}", fileName);
                    }
                    else
                    {
                        Trace.TraceInformation("Statistics.ToolSpecifics = null in stat-file.");
                    }
                }
            }

            var jsonContent = Newtonsoft.Json.JsonConvert.SerializeObject(
                this.Stat,
                Newtonsoft.Json.Formatting.Indented,
                new Newtonsoft.Json.Converters.IsoDateTimeConverter()
                {
                    DateTimeStyles = System.Globalization.DateTimeStyles.AdjustToUniversal
                });

            if (saveToFile)
            {
                using (StreamWriter writer = new StreamWriter(fileName))
                {
                    writer.WriteLine(jsonContent);
                    Trace.TraceInformation("Statistics saved to {0}", fileName);
                }
            }

            return jsonContent;
        }
    }
}
