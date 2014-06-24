using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JobManagerFramework.Jenkins.Job
{
    public class Action
    {
    }

    public class Build
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class FirstBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class HealthReport
    {
        public string description { get; set; }
        public string iconUrl { get; set; }
        public int score { get; set; }
    }

    public class LastBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class LastCompletedBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class LastFailedBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class LastStableBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class LastSuccessfulBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class LastUnstableBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class LastUnsuccessfulBuild
    {
        public int number { get; set; }
        public string url { get; set; }
    }

    public class Scm
    {
    }

    public class Task
    {
        public string name { get; set; }
        public string url { get; set; }
    }

    public class QueueItem
    {
        public bool blocked { get; set; }
        public bool buildable { get; set; }
        public int id { get; set; }
        public long inQueueSince { get; set; }
        public string @params { get; set; }
        public bool stuck { get; set; }
        public Task task { get; set; }
        public string why { get; set; }
        public long buildableStartMilliseconds { get; set; }
    }

    public class Job
    {
        // public List<Action> actions { get; set; }
        // public string description { get; set; }
        // public string displayName { get; set; }
        // public object displayNameOrNull { get; set; }
        public string name { get; set; }
        public string url { get; set; }
        public bool buildable { get; set; }
        public List<Build> builds { get; set; }
        public string color { get; set; }
        public FirstBuild firstBuild { get; set; }
        public List<HealthReport> healthReport { get; set; }
        public bool inQueue { get; set; }
        public bool keepDependencies { get; set; }
        public LastBuild lastBuild { get; set; }
        public LastCompletedBuild lastCompletedBuild { get; set; }
        public LastFailedBuild lastFailedBuild { get; set; }
        public LastStableBuild lastStableBuild { get; set; }
        public LastSuccessfulBuild lastSuccessfulBuild { get; set; }
        public LastUnstableBuild lastUnstableBuild { get; set; }
        public LastUnsuccessfulBuild lastUnsuccessfulBuild { get; set; }
        public int nextBuildNumber { get; set; }
        public List<object> property { get; set; }
        public QueueItem queueItem { get; set; }
        public bool concurrentBuild { get; set; }
        public List<object> downstreamProjects { get; set; }
        public Scm scm { get; set; }
        public List<object> upstreamProjects { get; set; }
    }
}
