using System;
using System.Collections.Generic;

public class Statistics
{
    /// <summary>
    /// Vehicle forge user Id
    /// </summary>
    public string UserId { get; set; }

    public int Id { get; set; }

    public bool IsRemote { get; set; }

    /// <summary>
    /// VehicleForge instance where the job got submitted
    /// </summary>
    public string VFUrl { get; set; }

    public class Execution
    {
        /// <summary>
        /// Start time of this execution
        /// </summary>
        public string StartTime { get; set; }

        /// <summary>
        /// Job got into the queue
        /// </summary>
        public class StatusTrace
        {
            public string Status { get; set; }
            public string TimeStamp { get; set; }
        }
        public List<StatusTrace> StatusTraces { get; set; }

        public Execution()
        {
            this.StatusTraces = new List<StatusTrace>();
        }
    }

    public List<Execution> Executions { get; set; }

    /// <summary>
    /// Job received from the master interpreter
    /// </summary>
    public string JobReceived { get; set; }

    /// <summary>
    /// Name of the job
    /// </summary>
    public string JobName { get; set; }

    /// <summary>
    /// Command that is being executed
    /// </summary>
    public string RunCommand { get; set; }

    /// <summary>
    /// Size of source_data.zip
    /// </summary>
    public string SizeOfSourcePackage { get; set; }

    /// <summary>
    /// Creation time of this file
    /// </summary>
    public object ToolSpecifics { get; set; }

    public Statistics()
    {
        this.Executions = new List<Execution>();
    }
}
