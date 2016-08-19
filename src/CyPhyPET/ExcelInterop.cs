using System;
using Excel = Microsoft.Office.Interop.Excel;
using VSTOContrib.Core.Extensions;
using System.Collections;
using System.Runtime.InteropServices;

namespace CyPhyPET
{
    internal class ExcelInterop
    {
        internal static void GetExcelInputsAndOutputs(string xlFilename, Action<string, string> addOutput, Action<string, string> addInput, Action done)
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
            try
            {
                // n.b. Excel needs an absolute path
                using (var workbooks = excelApp.Workbooks.WithComCleanup())
                using (var workbook = workbooks.Resource.Open(xlFilename).WithComCleanup())
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
                            formula = range.Formula;
                            if (formula is string && ((string)formula).StartsWith("="))
                            {
                                addOutput(nameName, rt);
                            }
                            else
                            {
                                addInput(nameName, rt);
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
            }
        }


    }
}
