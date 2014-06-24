// -----------------------------------------------------------------------
// <copyright file="SoT.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace JobManager
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.IO;
    using System.Diagnostics;
    using System.Runtime.InteropServices;
    using GME.MGA;
    using System.Threading;

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
