using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml.Linq;

namespace InterchangeXmlComparator
{
    class Program
    {
        static int Main(string[] args)
        {
            var options = new Options();
            if (!CommandLine.Parser.Default.ParseArguments(args, options)) Environment.Exit(1);

            var exportedXmlFi = new FileInfo(options.ExportedFile);
            var desiredResultXmlFi = new FileInfo(options.DesiredResultFile);

            if (!exportedXmlFi.Exists)
            {
                Console.Error.WriteLine("File doesn't exist: {0}", exportedXmlFi.FullName);
                Environment.Exit(-1);
            }
            if (!desiredResultXmlFi.Exists)
            {
                Console.Error.WriteLine("File doesn't exist: {0}", desiredResultXmlFi.FullName);
                Environment.Exit(-1);
            }

            var exportedXdoc = XDocument.Load(exportedXmlFi.FullName);
            var desiredXdoc = XDocument.Load(desiredResultXmlFi.FullName);

            if (options.ComparerMode == ComparerModes.Component)
            {
                var compComparator = new ComponentComparator(exportedXdoc, desiredXdoc);
                compComparator.Check();
            }
            else if (options.ComparerMode == ComparerModes.Design)
            {
                var designComparator = new DesignComparator(exportedXdoc, desiredXdoc);
                designComparator.Check();
            }

            #region Feedbacks

            if (!Feedback.FeedBacklist.Any())
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("The two files are identical: {0} == {1}", Path.GetFileName(options.ExportedFile), Path.GetFileName(options.DesiredResultFile));
                Environment.Exit(0);
            }
            else
            {
                foreach (var feedback in Feedback.FeedBacklist)
                {
                    Console.ForegroundColor = ConsoleColor.White;

                    Console.WriteLine(feedback.Message);
                    switch (feedback.FeedbackType)
                    {
                        case FeedbackType.Warning:
                            Console.ForegroundColor = ConsoleColor.Yellow;
                            break;
                        case FeedbackType.Error:
                            Console.ForegroundColor = ConsoleColor.Red;
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }

                    Console.Error.WriteLine("  Exported: {0}\n    Path: {1}\n  Desired: {2}\n    Path: {3}", feedback.ExportedNodeName,
                                      feedback.ExportedNodePath, feedback.DesiredNodeName, feedback.DesiredNodePath);
                    Console.Error.WriteLine();
                }
                Environment.Exit(-1);
            }

            #endregion
            Console.ForegroundColor = ConsoleColor.White;
            return 0;
        }

        

       

        

        

        
    }
}
