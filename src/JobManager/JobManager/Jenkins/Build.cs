using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JobManager.Jenkins.Build
{
    public class Caus
    {
        public string shortDescription { get; set; }
        public object userId { get; set; }
        public string userName { get; set; }
    }

    public class Action
    {
        public List<Caus> causes { get; set; }
        public int? failCount { get; set; }
        public int? skipCount { get; set; }
        public int? totalCount { get; set; }
        public string urlName { get; set; }
    }

    public class Artifact
    {
        public string displayPath { get; set; }
        public string fileName { get; set; }
        public string relativePath { get; set; }
    }

    public class Revision
    {
        public string module { get; set; }
        public int revision { get; set; }
    }

    public class ChangeSet
    {
        public List<object> items { get; set; }
        public string kind { get; set; }
        public List<Revision> revisions { get; set; }
    }

    public class Culprit
    {
        public string absoluteUrl { get; set; }
        public string fullName { get; set; }
    }

    public class Build
    {
        public List<Action> actions { get; set; }
        public List<Artifact> artifacts { get; set; }
        public bool building { get; set; }
        public object description { get; set; }
        public int duration { get; set; }
        public int estimatedDuration { get; set; }
        public object executor { get; set; }
        public string fullDisplayName { get; set; }
        public string id { get; set; }
        public bool keepLog { get; set; }
        public int number { get; set; }
        public string result { get; set; }
        public long timestamp { get; set; }
        public string url { get; set; }
        public string builtOn { get; set; }
        public ChangeSet changeSet { get; set; }
        public List<Culprit> culprits { get; set; }
    }
}
