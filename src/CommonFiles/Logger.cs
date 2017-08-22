// -----------------------------------------------------------------------
// <copyright file="Logger.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace META
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Diagnostics;
    using System.IO;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public static class Logger
    {
        private static Dictionary<string, TraceListener> LoggingFiles = new Dictionary<string, TraceListener>();

        public static string Header(GME.MGA.MgaProject project = null)
        {
            StringBuilder logFileHeader = new StringBuilder();

            logFileHeader.AppendLine("=== HEADER ===================================================");
            logFileHeader.AppendLine(string.Format("Date/Time:       {0}", DateTime.Now));
            logFileHeader.AppendLine(string.Format("Windows Version: {0}", System.Environment.OSVersion.VersionString));
            logFileHeader.AppendLine(string.Format("x64 bit OS:      {0}", System.Environment.Is64BitOperatingSystem));
            logFileHeader.AppendLine(string.Format("GME version:     {0}", META.VersionInfo.GmeVersion));
            logFileHeader.AppendLine(string.Format("META version:    {0}", META.VersionInfo.MetaVersion));
            logFileHeader.AppendLine(string.Format("META install:    {0}", META.VersionInfo.MetaPath));
            logFileHeader.AppendLine(string.Format("CyPhyML version: {0}", META.VersionInfo.CyPhyML));
            logFileHeader.AppendLine(string.Format("CyPhyML GUID:    {0}", META.VersionInfo.CyPhyMLGuid));

            logFileHeader.AppendLine(string.Format("Python27 VE exe: {0}", META.VersionInfo.PythonVEnvExe));

            logFileHeader.AppendLine(string.Format("CreateAssembly v {0}", META.VersionInfo.CADCreoParametricCreateAssemblyVersion));

            logFileHeader.AppendLine(string.Format("Username:        {0}", Environment.UserName));

            logFileHeader.AppendLine("Currently opened paradigm information");

            if (project != null)
            {
                bool transactionAlreadyOpen = (project.ProjectStatus & 8) == 0;
                try
                {
                    if (transactionAlreadyOpen)
                    {
                        project.BeginTransactionInNewTerr();
                    }

                    logFileHeader.AppendLine(string.Format("  MetaName    : {0}", project.MetaName));
                    logFileHeader.AppendLine(string.Format("  MetaVersion : {0}", project.MetaVersion));

                    //logFileHeader.AppendLine(string.Format("  MetaGuid : {0}", ByteArrayToString((project.MetaGUID as IEnumerable<byte>).ToList())));
                }
                finally
                {
                    if (transactionAlreadyOpen)
                    {
                        project.AbortTransaction();
                    }
                }
            }
            else
            {
                logFileHeader.AppendLine(string.Format("  Not available."));
            }

            return logFileHeader.ToString();
        }

        public static void AddFileListener(string loggingFileName, string name, GME.MGA.MgaProject project = null)
        {
            if (string.IsNullOrWhiteSpace(name))
            {
                name = System.Reflection.Assembly.GetExecutingAssembly().FullName;
            }

            if (string.IsNullOrWhiteSpace(loggingFileName))
            {
                // get temp path and temp file name
                loggingFileName = System.IO.Path.GetTempPath() +
                    System.Reflection.Assembly.GetExecutingAssembly().FullName +
                    ".trace.txt";
            }

            loggingFileName = Path.GetFullPath(loggingFileName);

            var dirName = Path.GetDirectoryName(loggingFileName);
            if (Directory.Exists(dirName) == false)
            {
                Directory.CreateDirectory(dirName);
            }

            if (LoggingFiles.Keys.Contains(loggingFileName) == false)
            {
                // set up tracing

                // print header and version info
                var fs = new FileStream(loggingFileName, FileMode.Create);
                TraceListener fileTL = new TextWriterTraceListener(fs)
                {
                    //TraceOutputOptions = TraceOptions.DateTime,
                    Name = name,
                };
                // use TraceXXX to get timestamp per http://stackoverflow.com/questions/863394/add-timestamp-to-trace-writeline

                Trace.AutoFlush = true;
                Trace.Listeners.Add(fileTL);

                LoggingFiles.Add(loggingFileName, fileTL);

                Trace.TraceInformation("{0} trace file listener was created.", loggingFileName);
                Trace.TraceInformation("{0}", Header(project));
            }
            else
            {
                Console.WriteLine("{0} already exists for logging.", loggingFileName);
            }
        }

        public static void RemoveFileListener(string name)
        {
            TraceListener tl = null;
            if (LoggingFiles.TryGetValue(name, out tl))
            {
                Trace.Listeners.Remove(tl);
                Trace.TraceInformation("{0} was removed.", name);
                LoggingFiles.Remove(name);
                tl.Close();
            }
        }
    }
}
