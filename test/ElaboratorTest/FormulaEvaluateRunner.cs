// -----------------------------------------------------------------------
// <copyright file="ElaboratorRunner.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ElaboratorTest
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using Xunit;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class FormulaEvaluateRunner
    {
        public static void RunFormulaEvaluate(string projectPath, string absPath, bool automation)
        {
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();
                MgaFCOs fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

                Type tFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
                var formulaEval = Activator.CreateInstance(tFormulaEval) as GME.MGA.IMgaComponentEx;
                formulaEval.ComponentParameter["automation"] = automation ? "true" : "false";
                formulaEval.ComponentParameter["console_messages"] = "on";
                formulaEval.ComponentParameter["expanded"] = "true";

                //formulaEval.Initialize(project);
                formulaEval.InvokeEx(project, testObj, fcos, 16);
            }
            finally
            {
                project.Close(true);
            }
        }
    }
}
