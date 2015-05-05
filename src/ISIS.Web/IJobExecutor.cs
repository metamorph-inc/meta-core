using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ISIS.Web
{
    public interface IJobExecutor
    {
        // CreateJob() should return jobId (jobId is used in the other 4 methods)
        // CreateJob should take an argument describing the location of the generated files on the file system
        IJob CreateJob(string dirPath, string jobTitle);

        bool Start(IJob job);

        string CheckStatus(IJob job);

        string GetResults(IJob job);

        bool Kill(IJob job);

        bool RemoveFromJobManager(IJob job);
    }

}
