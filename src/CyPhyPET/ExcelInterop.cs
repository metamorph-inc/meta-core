using System;
using Excel = Microsoft.Office.Interop.Excel;
using VSTOContrib.Core.Extensions;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace CyPhyPET
{
    internal class ExcelInterop
    {
        [DllImport("kernel32.dll")]
        static extern int GetCurrentThreadId();

        public enum ExcelType
        {
            Float,
            Str,
            // Bool,
            // Int
        };

        internal static void GetExcelInputsAndOutputs(string xlFilename, Action<string, string, ExcelType> addOutput, Action<string, string, string, ExcelType> addInput, Action done)
        {
            int REGDB_E_CLASSNOTREG = unchecked((int)0x80040154);
            Excel.Application excelApp;
            try
            {
                var excelType = Type.GetTypeFromProgID("Excel.Application");
                if (excelType == null)
                {
                    throw new COMException("No type Excel.Application", REGDB_E_CLASSNOTREG);
                }
                excelApp = (Excel.Application)Activator.CreateInstance(excelType);
            }
            catch (COMException e)
            {
                if (e.ErrorCode == REGDB_E_CLASSNOTREG)
                {
                    throw new ApplicationException("Excel is not installed");
                }
                else
                {
                    throw;
                }
            }
            // Create a copy of the file, since excel opens with ShareMode: None and we may want to run in parallel
            string xlFilenameCopy = Path.Combine(Path.GetDirectoryName(xlFilename), Path.GetFileNameWithoutExtension(xlFilename) + "_" + GetCurrentThreadId().ToString() +
                "_" + Process.GetCurrentProcess().Id + Path.GetExtension(xlFilename));
            try
            {
                System.IO.File.Copy(xlFilename, xlFilenameCopy, true);
            }
            catch (IOException e)
            {
                throw new ApplicationException(String.Format("Could not copy \"{0}\": {1}", xlFilename, e.Message), e);
            }
            try
            {
                // n.b. Excel needs an absolute path
                using (var workbooks = excelApp.Workbooks.WithComCleanup())
                using (var workbook = workbooks.Resource.Open(xlFilenameCopy).WithComCleanup())
                {
                    // for (var workbook in excelApp.Workbooks
                    foreach (Excel.Name name in ((IEnumerable)workbook.Resource.Names).ComLinq<object>())
                    {
                        string nameName = name.Name;
                        dynamic rt = null;
                        dynamic range = null;
                        dynamic val = null;
                        dynamic formula = null;
                        try
                        {
                            rt = name.RefersToLocal;
                            // FIXME name.RefersToLocal may be c:\path\asdf\[proj.xlsx]Sheet!A2
                            // skip Hidden names
                            if (name.Visible == false || !(rt is string))
                            {
                                continue;
                            }

                            try
                            {
                                range = name.RefersToRange;
                            }
                            catch
                            {
                                try
                                {
                                    range = excelApp.Range[rt];
                                }
                                catch
                                {
                                    continue;
                                }
                            }

                            val = range.Value;
                            ExcelType type = ExcelType.Float;
                            if (excelApp.WorksheetFunction.IsText(val))
                            {
                                type = ExcelType.Str;
                            }
                            // TODO if (val == true || val == false)
                            formula = range.Formula;
                            if (formula is string && ((string)formula).StartsWith("="))
                            {
                                addOutput(nameName, rt, type);
                            }
                            else
                            {
                                addInput(nameName, rt, val.ToString(), type);
                            }
                        }
                        finally
                        {
                            ComCleanupExtensions.ReleaseComObject((object)rt);
                            ComCleanupExtensions.ReleaseComObject((object)range);
                            ComCleanupExtensions.ReleaseComObject((object)val);
                            ComCleanupExtensions.ReleaseComObject((object)formula);
                        }

                    }
                    done();
                }
            }
            finally
            {
                foreach (Excel.Workbook workbook in ((IEnumerable)excelApp.Workbooks).ComLinq<object>())
                {
                    workbook.Close(false);
                }
                excelApp.Quit();
                Marshal.FinalReleaseComObject(excelApp);
                File.Delete(xlFilenameCopy);
            }
        }


    }
}
