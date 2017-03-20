using System;

namespace JobManager
{
    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class SoT : MarshalByRefObject
    {
        public JobServer Server { get; set; }
        public string WorkingDirectory { get; set; }
        public string ProjectConnStr { get; set; }
    }


    public class JobFailure : ApplicationException
    {
        public JobFailure(string msg)
            : base(msg)
        {
        }

        public JobFailure(string msg, Exception inner)
            : base(msg, inner)
        {
        }
    }
}
