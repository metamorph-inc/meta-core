using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME;

namespace GME.CSharp
{

    /// <summary>
    /// Automatically redirects console messages to the GME console output, if GME is available.
    /// Otherwise prints the output to System console.
    /// </summary>
    public class GMEConsole
    {

        /// <summary>
        /// The GME application variable
        /// </summary>
        public IGMEOLEApp gme = null;
        private GMETextWriter error;
        private GMETextWriter warning;
        private GMETextWriter info;
        private TextWriter normal;

        public GMEConsole()
        {
            error = new GMETextWriter(msgtype_enum.MSG_ERROR, this);
            warning = new GMETextWriter(msgtype_enum.MSG_WARNING, this);
            info = new GMETextWriter(msgtype_enum.MSG_INFO, this);
            normal = new GMETextWriter(msgtype_enum.MSG_NORMAL, this);
        }

        /// <summary>
        /// Handles error messages
        /// The message to be written. GME Console does not handle special characters and trims white-spaces.
        /// Example: GMEConsole.Error.Write("RootFolder name error: {0}.", rf.Name);
        /// If console is initialized, the message appears in GME console, if not, then in standard error.
        /// If DEBUG is defined, it also appears in VS output window.
        /// </summary>
        public TextWriter Error
        {
            get { return error; }
        }

        /// <summary>
        /// Prints messages.
        /// The message to be written. GME Console does not handle special characters and trims white-spaces.
        /// Example: GMEConsole.Out.Write("RootFolder name : {0}.", rf.Name);
        /// </summary>
        public TextWriter Out
        {
            get { return normal; }
        }


        /// <summary>
        /// Prints warning messages.
        /// The message to be written. GME Console does not handle special characters and trims white-spaces.
        /// Example: GMEConsole.Warning.Write("RootFolder name is not changed : {0}.", rf.Name);
        /// </summary>
        public TextWriter Warning
        {
            get { return warning; }
        }


        /// <summary>
        /// Proints info messages.
        /// The message to be written. GME Console does not handle special characters and trims white-spaces.
        /// Example: GMEConsole.Info.Write("RootFolder name is changed : {0}.", rf.Name);
        /// </summary>
        public TextWriter Info
        {
            get { return info; }
        }

        /// <summary>
        /// Clear the console
        /// </summary>
        public void Clear()
        {
            if (gme != null)
                gme.ConsoleClear();
            else
                try
                {
                    System.Console.Clear();
                }
                catch (IOException) // fails if the console is redirected to a file
                {
                }
        }


        //[System.Diagnostics.DebuggerStepThrough]
        //[System.Diagnostics.DebuggerNonUserCode] // enable Just My Code in debugger settings
        public static GMEConsole CreateFromProject(GME.MGA.MgaProject project)
        {
            GMEConsole console = new GMEConsole();
            /* if (System.IO.Path.GetFileNameWithoutExtension(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName).Contains("xunit")
                || System.IO.Path.GetFileNameWithoutExtension(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName).Contains(".vshost"))
            {
                console.normal = new CyPhyGUIs.ConsoleTextWriter();
                return console;
            } */
            // Initializing console
            var gme = project.Clients.Cast<MGA.MgaClient>().Where(c => c.Name == "GME.Application").FirstOrDefault();
            if (gme != null)
            {
                console.gme = (IGMEOLEApp)gme.OLEServer;
            }
            else
            {
                // if GME is not present, the interpreter is called from standalone test application
                console.gme = null;

                console.normal = new CyPhyGUIs.ConsoleTextWriter();
            }
            //catch (ArgumentException ex)
            //{
            //    // missing paramter exception
            //    console.gme = null;

            //    console.normal = new CyPhyGUIs.ConsoleTextWriter();
            //}

            return console;
        }
    }

    public static class GmeConsoleHelper
    {
        public static string ToHyperLink<T>(this T subject, CyPhyCOMInterfaces.IMgaTraceability elaboratedObjects = null, bool useName = false)
            where T : ISIS.GME.Common.Interfaces.Base
        {
            StringBuilder sb = new StringBuilder();

            if (elaboratedObjects != null)
            {
                string id = string.Empty;
                if (elaboratedObjects.TryGetMappedObject(subject.ID, out id))
                {
                    GME.MGA.MgaFCO fco = subject.Impl.Project.GetFCOByID(id);
                    if (fco != null)
                    {
                        // TODO: maybe get it until we reach the original source???
                        sb.AppendFormat("<a href=\"mga:{0}\">{1}</a>", fco.ID, fco.Name);
                        return sb.ToString();
                    }
                }
            }

            if (useName)
            {
                sb.AppendFormat("<a href=\"mga:{0}\">{1}</a>", subject.ID, subject.Name);
            }
            else
            {
                sb.AppendFormat("<a href=\"mga:{0}\">{1}</a>", subject.ID, subject.Path);
            }
            return sb.ToString();
        }


        public static string ToMgaHyperLink<T>(this T subject, CyPhyCOMInterfaces.IMgaTraceability elaboratedObjects = null, bool useName = false)
    where T : GME.MGA.IMgaFCO
        {
            ISIS.GME.Common.Classes.Base fco = new ISIS.GME.Common.Classes.Base();
            fco.Impl = subject;
            return ToHyperLink<ISIS.GME.Common.Interfaces.Base>(fco, elaboratedObjects, useName);
        }
    }


    public class GMETextWriter : System.IO.TextWriter
    {
        private msgtype_enum type;
        private GMEConsole console;

        public GMETextWriter(msgtype_enum type, GMEConsole console)
        {
            this.type = type;
            this.console = console;
        }

        override public Encoding Encoding
        {
            get { return Encoding.Unicode; }
        }

        override public void WriteLine(string str)
        {
            Write(str + Environment.NewLine);
        }

        override public void Write(string str)
        {
            if (console.gme == null)
            {
                switch (type)
                {
                    case msgtype_enum.MSG_NORMAL:
                        Console.Out.Write(str);
                        break;
                    case msgtype_enum.MSG_INFO:
                        Console.Out.Write("Information: " + str);
                        break;
                    case msgtype_enum.MSG_WARNING:
                        Console.Out.Write("Warning: " + str);
                        break;
                    case msgtype_enum.MSG_ERROR:
                        Console.Error.Write(str);
#if(DEBUG)
                        System.Diagnostics.Debug.Write(str);
#endif
                        break;
                }
            }
            else
            {
                console.gme.ConsoleMessage(str, type);
            }
        }
    }
}
