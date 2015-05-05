using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Runtime.InteropServices;

namespace CyPhy2ComponentModel
{
    public static class Utilities
    {
        public static void RunFormulaEvaluator(this ISIS.GME.Dsml.CyPhyML.Interfaces.Component component)
        {
            var project = component.Impl.Project;
            var currentobj = component.Impl as MgaFCO;

            // create formula evaluator type
            // FIXME: calling the elaborator is faster than calling the formula evaluator
            Type typeFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
            IMgaComponentEx formulaEval = Activator.CreateInstance(typeFormulaEval) as IMgaComponentEx;

            // empty selected object set
            Type typeMgaFCOs = Type.GetTypeFromProgID("Mga.MgaFCOs");
            MgaFCOs selectedObjs = Activator.CreateInstance(typeMgaFCOs) as MgaFCOs;

            // initialize formula evauator
            formulaEval.Initialize(project);

            // automation means no UI element shall be shown by the interpreter
            formulaEval.ComponentParameter["automation"] = "true";

            // do not write to the console
            formulaEval.ComponentParameter["console_messages"] = "off";

            // do not expand nor collapse the model
            formulaEval.ComponentParameter["expanded"] = "true";

            // do not generate the post processing python scripts
            // FIXME: Why should we generate them ???
            formulaEval.ComponentParameter["do_not_generate_post_processing"] = "true";

            // call the formula evaluator and update all parameters starting from the current object
            try
            {
                formulaEval.InvokeEx(project, currentobj, selectedObjs, 128);
            }
            catch (COMException)
            {
                // FIXME: handle this exception properly
                // success = false;
                // this.Logger.WriteError(exceptionMessage);
                // this.Logger.WriteError("CyPhyFormulaEvaluator 1.0 finished with errors");
            }
        }
    }
}
