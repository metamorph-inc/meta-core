using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CyPhyGUIs
{
    public class ConsoleTextWriter : System.IO.TextWriter
    {
        override public void WriteLine(string str)
        {
            Write(str + Environment.NewLine);
        }

        override public void Write(string str)
        {
            this.Write(SmartLogger.MessageType_enum.Info, str);
        }

        public void WriteLine(SmartLogger.MessageType_enum type, string msg)
        {
            this.Write(type, msg + Environment.NewLine);
        }

        public void Write(SmartLogger.MessageType_enum type, string msg)
        {
            var originalColor = Console.ForegroundColor;

            switch (type)
            {
                case SmartLogger.MessageType_enum.Debug:
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.Out.Write(string.Format("{0,-8} - {1}", type, msg));
                    break;
                case SmartLogger.MessageType_enum.Error:
                case SmartLogger.MessageType_enum.Failed:
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Error.Write(string.Format("{0,-8} - {1}", type, msg));
                    Console.Error.Flush();
                    break;
                case SmartLogger.MessageType_enum.Info:
                    Console.ForegroundColor = ConsoleColor.Cyan;
                    Console.Out.Write(string.Format("{0,-8} - ", type));
                    Console.ForegroundColor = originalColor;
                    Console.Out.Write(msg);
                    break;
                case SmartLogger.MessageType_enum.Success:
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.Out.Write(string.Format("{0,-8} - ", type));
                    Console.ForegroundColor = originalColor;
                    Console.Out.Write(msg);
                    break;
                case SmartLogger.MessageType_enum.Warning:
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.Out.Write(string.Format("{0,-8} - {1}", type, msg));
                    break;
                default:
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    break;
            }

            Console.ForegroundColor = originalColor;
        }

        override public Encoding Encoding
        {
            get { return Encoding.Unicode; }
        }
    }

    public class SmartLogger : IDisposable
    {
        public enum MessageType_enum
        {
            Debug,
            Success,
            Info,
            Warning,
            Failed,
            Error
        }

        protected List<TextWriter> m_textWriters { get; set; }

        public MessageType_enum LoggingLevel { get; set; }

        private List<string> m_filenames { get; set; }

        public List<string> LogFilenames
        {
            get
            {
                return this.m_filenames;
            }
        }

        public CyPhyCOMInterfaces.IMgaTraceability Traceability { get; set; }
        public SmartLogger()
        {
            this.Traceability = new META.MgaTraceability();
            this.LoggingLevel = MessageType_enum.Debug;
            this.m_textWriters = new List<TextWriter>();
            this.m_filenames = new List<string>();
        }

        public void AddWriter(TextWriter writer, bool useHtmlTags = false)
        {
            if (this.m_textWriters.Contains(writer) == false)
            {
                this.m_textWriters.Add(writer);
            }
        }

        public void AddWriter(string filename)
        {
            // TODO: what if this throws an exception?

            TextWriter tw = File.AppendText(filename);
            m_textWriters.Add(tw);
            m_filenames.Add(filename);
        }

        protected virtual void Write(TextWriter textWriter, MessageType_enum type, string message)
        {
            if (type >= this.LoggingLevel)
            {
                textWriter.WriteLine("{0} - [{1}] - {2}", DateTime.Now.ToString("s"), type, message);
                textWriter.Flush();
            }
        }

        protected virtual void WriteAll(MessageType_enum type, string message)
        {
            foreach (var textWriter in this.m_textWriters)
            {
                this.Write(textWriter, type, message);
            }
        }

        protected void WriteAll(MessageType_enum type, string format, params object[] args)
        {
            this.WriteAll(type, string.Format(format, args));
        }

        public void WriteError(string message)
        {
            this.WriteAll(MessageType_enum.Error, message);
        }

        public void WriteError(string format, params object[] args)
        {
            this.WriteAll(MessageType_enum.Error, format, args);
        }

        public void WriteWarning(string message)
        {
            this.WriteAll(MessageType_enum.Warning, message);
        }

        public void WriteWarning(string format, params object[] args)
        {
            this.WriteAll(MessageType_enum.Warning, format, args);
        }

        public void WriteInfo(string message)
        {
            this.WriteAll(MessageType_enum.Info, message);
        }

        public void WriteInfo(string format, params object[] args)
        {
            this.WriteAll(MessageType_enum.Info, format, args);
        }

        public void WriteDebug(string message)
        {
            this.WriteAll(MessageType_enum.Debug, message);
        }

        public void WriteDebug(string format, params object[] args)
        {
            this.WriteAll(MessageType_enum.Debug, format, args);
        }


        public void WriteFailed(string message)
        {
            this.WriteAll(MessageType_enum.Failed, message);
        }

        public void WriteFailed(string format, params object[] args)
        {
            this.WriteAll(MessageType_enum.Failed, format, args);
        }

        public void WriteSuccess(string message)
        {
            this.WriteAll(MessageType_enum.Success, message);
        }

        public void WriteSuccess(string format, params object[] args)
        {
            this.WriteAll(MessageType_enum.Success, format, args);
        }


        public virtual void Dispose()
        {
            foreach (var textWriter in this.m_textWriters)
            {
                textWriter.Dispose();
            }
        }
    }
}
