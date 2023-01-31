﻿// ------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version: 14.0.0.0
//  
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
// ------------------------------------------------------------------------------
namespace CyPhy2CAD_CSharp.Template
{
    using System;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "C:\Users\kevin\Documents\meta-core\src\CyPhy2CAD_CSharp\Template\run_bat_cfd.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "14.0.0.0")]
    public partial class run_bat_cfd : run_bat_cfdBase
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public virtual string TransformText()
        {
            this.Write(@"

REM	See ""C:\Program Files (x86)\META\bin\CAD\Creo\0Readme - CreateAssembly.txt"" for the complete setup instructions.

SetLocal EnableExtensions
if not defined SystemRoot set SystemRoot=C:\WINDOWS

set WORKING_DIR="".""
set ERROR_CODE=0

set ERROR_MSG=""""

FOR /F ""skip=2 tokens=2,*"" %%A IN ('%SystemRoot%\SysWoW64\REG.exe query ""HKLM\software\META"" /v ""META_PATH""') DO set MetaPath=%%B

set CADPython=%MetaPath%\bin\CAD

Rem ****************************
REM Create Creo Assembly
Rem ****************************

set EXE_FILE_NAME=CADCreoParametricCreateAssembly.exe
set EXE=""%MetaPath%\bin\CAD\Creo\bin\%EXE_FILE_NAME%""

set ASSEMBLY_XML_FILE=""CADAssembly.xml""
set LOG_FILE=%ASSEMBLY_XML_FILE%.log


if exist %EXE% goto  :EXE_FOUND
@echo off
echo		Error: Could not find %EXE_FILE_NAME%.
echo		Your system is not properly configured to run %EXE_FILE_NAME%.
echo		Please see For instructions on how to configure your system, please see ""0Readme - CreateAssembly.txt""
echo		which is typically located at ""C:\Program Files\META\Proe ISIS Extensions""
set ERROR_CODE=2
set ERROR_MSG=""Error from runCADJob.bat: Could not find CADCreoParametricCreateAssembly.exe.""
goto :ERROR_SECTION
:EXE_FOUND

%EXE% -w %WORKING_DIR%  -i %ASSEMBLY_XML_FILE% -l %LOG_FILE% ");
            
            #line 41 "C:\Users\kevin\Documents\meta-core\src\CyPhy2CAD_CSharp\Template\run_bat_cfd.tt"
 if (!Automation) { 
            
            #line default
            #line hidden
            this.Write("-p");
            
            #line 41 "C:\Users\kevin\Documents\meta-core\src\CyPhy2CAD_CSharp\Template\run_bat_cfd.tt"
 } 
            
            #line default
            #line hidden
            this.Write(@"
set ERROR_CODE=%ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 (
set ERROR_MSG=""Error from runCADJob.bat: %EXE_FILE_NAME% encountered error during execution, error level is %ERROR_CODE%""
goto :ERROR_SECTION
)


REM ****************************
REM CFD Convert to STL
REM ****************************
FOR /F ""skip=2 tokens=2,*"" %%A IN ('%SystemRoot%\SysWoW64\REG.exe query ""HKLM\software\META"" /v ""META_PATH""') DO ""%%B\bin\Python311\Python.exe"" scripts\convert2ascii.py

set ERROR_CODE=%ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 (
set ERROR_MSG=""Error from runCADJob: Encountered error during Inventor conversion to STL, error level is %ERROR_CODE%""
goto :ERROR_SECTION
)


if exist TestBench_PreProcess.cmd ""%SystemRoot%\System32\cmd.exe"" /c TestBench_PreProcess.cmd

set ERROR_CODE=%ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 (
set ERROR_MSG=""Error from runCADJob.bat: Encountered error during execution of TestBench_PreProcess.cmd, error level is %ERROR_CODE%""
goto :ERROR_SECTION
)

");
            
            #line 70 "C:\Users\kevin\Documents\meta-core\src\CyPhy2CAD_CSharp\Template\run_bat_cfd.tt"
 if (Automation) { 
            
            #line default
            #line hidden
            this.Write(@"@echo off


Rem ****************************
REM Python Metric Update Script
Rem ****************************

set RESULT_XML_FILE=""ComputedValues.xml""
set PY_SCRIPT_NAME=UpdateReportJson_CAD.py
set PY_SCRIPT=""%CADPython%\%PY_SCRIPT_NAME%""
set PYTHONPATH=%PYTHONPATH%;%MetaPath%

if exist %PY_SCRIPT% goto  :PY_FOUND
@echo off
echo		Error: Could not find %PY_SCRIPT_NAME%.
echo		Your system is not properly configured to run %PY_SCRIPT_NAME%.
echo		Please see For instructions on how to configure your system, please see ""0Readme - CreateAssembly.txt""
echo		which is typically located at ""C:\Program Files (x86)\META\bin\CAD\Creo""
set ERROR_CODE=2
set ERROR_MSG=""Error from runCADJob.bat: Could not find UpdateReportJson_CAD.py.""
goto :ERROR_SECTION

:PY_FOUND
""%MetaPath%\bin\Python311\Python.exe"" %PY_SCRIPT% -m %RESULT_XML_FILE%

set ERROR_CODE=%ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 (
set ERROR_MSG=""Error from runCADJob.bat: %PY_SCRIPT_NAME% encountered error during execution, error level is %ERROR_CODE%""
goto :ERROR_SECTION
)

if exist TestBench_PostProcess.cmd ""%SystemRoot%\System32\cmd.exe"" /c TestBench_PostProcess.cmd

set ERROR_CODE=%ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 (
set ERROR_MSG=""Error from runCADJob.bat: Encountered error during execution of TestBench_PostProcess.cmd, error level is %ERROR_CODE%""
goto :ERROR_SECTION
)
");
            
            #line 109 "C:\Users\kevin\Documents\meta-core\src\CyPhy2CAD_CSharp\Template\run_bat_cfd.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\r\nexit 0\r\n\r\n:ERROR_SECTION\r\necho %ERROR_MSG% >>_FAILED.txt\r\necho \"\"\r\necho \"See Er" +
                    "ror Log: _FAILED.txt\"\r\nexit /b %ERROR_CODE%\r\n\r\n\r\n");
            return this.GenerationEnvironment.ToString();
        }
        
        #line 120 "C:\Users\kevin\Documents\meta-core\src\CyPhy2CAD_CSharp\Template\run_bat_cfd.tt"
  
public bool Automation {get;set;}

        
        #line default
        #line hidden
    }
    
    #line default
    #line hidden
    #region Base class
    /// <summary>
    /// Base class for this transformation
    /// </summary>
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "14.0.0.0")]
    public class run_bat_cfdBase
    {
        #region Fields
        private global::System.Text.StringBuilder generationEnvironmentField;
        private global::System.CodeDom.Compiler.CompilerErrorCollection errorsField;
        private global::System.Collections.Generic.List<int> indentLengthsField;
        private string currentIndentField = "";
        private bool endsWithNewline;
        private global::System.Collections.Generic.IDictionary<string, object> sessionField;
        #endregion
        #region Properties
        /// <summary>
        /// The string builder that generation-time code is using to assemble generated output
        /// </summary>
        protected System.Text.StringBuilder GenerationEnvironment
        {
            get
            {
                if ((this.generationEnvironmentField == null))
                {
                    this.generationEnvironmentField = new global::System.Text.StringBuilder();
                }
                return this.generationEnvironmentField;
            }
            set
            {
                this.generationEnvironmentField = value;
            }
        }
        /// <summary>
        /// The error collection for the generation process
        /// </summary>
        public System.CodeDom.Compiler.CompilerErrorCollection Errors
        {
            get
            {
                if ((this.errorsField == null))
                {
                    this.errorsField = new global::System.CodeDom.Compiler.CompilerErrorCollection();
                }
                return this.errorsField;
            }
        }
        /// <summary>
        /// A list of the lengths of each indent that was added with PushIndent
        /// </summary>
        private System.Collections.Generic.List<int> indentLengths
        {
            get
            {
                if ((this.indentLengthsField == null))
                {
                    this.indentLengthsField = new global::System.Collections.Generic.List<int>();
                }
                return this.indentLengthsField;
            }
        }
        /// <summary>
        /// Gets the current indent we use when adding lines to the output
        /// </summary>
        public string CurrentIndent
        {
            get
            {
                return this.currentIndentField;
            }
        }
        /// <summary>
        /// Current transformation session
        /// </summary>
        public virtual global::System.Collections.Generic.IDictionary<string, object> Session
        {
            get
            {
                return this.sessionField;
            }
            set
            {
                this.sessionField = value;
            }
        }
        #endregion
        #region Transform-time helpers
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void Write(string textToAppend)
        {
            if (string.IsNullOrEmpty(textToAppend))
            {
                return;
            }
            // If we're starting off, or if the previous text ended with a newline,
            // we have to append the current indent first.
            if (((this.GenerationEnvironment.Length == 0) 
                        || this.endsWithNewline))
            {
                this.GenerationEnvironment.Append(this.currentIndentField);
                this.endsWithNewline = false;
            }
            // Check if the current text ends with a newline
            if (textToAppend.EndsWith(global::System.Environment.NewLine, global::System.StringComparison.CurrentCulture))
            {
                this.endsWithNewline = true;
            }
            // This is an optimization. If the current indent is "", then we don't have to do any
            // of the more complex stuff further down.
            if ((this.currentIndentField.Length == 0))
            {
                this.GenerationEnvironment.Append(textToAppend);
                return;
            }
            // Everywhere there is a newline in the text, add an indent after it
            textToAppend = textToAppend.Replace(global::System.Environment.NewLine, (global::System.Environment.NewLine + this.currentIndentField));
            // If the text ends with a newline, then we should strip off the indent added at the very end
            // because the appropriate indent will be added when the next time Write() is called
            if (this.endsWithNewline)
            {
                this.GenerationEnvironment.Append(textToAppend, 0, (textToAppend.Length - this.currentIndentField.Length));
            }
            else
            {
                this.GenerationEnvironment.Append(textToAppend);
            }
        }
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void WriteLine(string textToAppend)
        {
            this.Write(textToAppend);
            this.GenerationEnvironment.AppendLine();
            this.endsWithNewline = true;
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void Write(string format, params object[] args)
        {
            this.Write(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void WriteLine(string format, params object[] args)
        {
            this.WriteLine(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Raise an error
        /// </summary>
        public void Error(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Raise a warning
        /// </summary>
        public void Warning(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            error.IsWarning = true;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Increase the indent
        /// </summary>
        public void PushIndent(string indent)
        {
            if ((indent == null))
            {
                throw new global::System.ArgumentNullException("indent");
            }
            this.currentIndentField = (this.currentIndentField + indent);
            this.indentLengths.Add(indent.Length);
        }
        /// <summary>
        /// Remove the last indent that was added with PushIndent
        /// </summary>
        public string PopIndent()
        {
            string returnValue = "";
            if ((this.indentLengths.Count > 0))
            {
                int indentLength = this.indentLengths[(this.indentLengths.Count - 1)];
                this.indentLengths.RemoveAt((this.indentLengths.Count - 1));
                if ((indentLength > 0))
                {
                    returnValue = this.currentIndentField.Substring((this.currentIndentField.Length - indentLength));
                    this.currentIndentField = this.currentIndentField.Remove((this.currentIndentField.Length - indentLength));
                }
            }
            return returnValue;
        }
        /// <summary>
        /// Remove any indentation
        /// </summary>
        public void ClearIndent()
        {
            this.indentLengths.Clear();
            this.currentIndentField = "";
        }
        #endregion
        #region ToString Helpers
        /// <summary>
        /// Utility class to produce culture-oriented representation of an object as a string.
        /// </summary>
        public class ToStringInstanceHelper
        {
            private System.IFormatProvider formatProviderField  = global::System.Globalization.CultureInfo.InvariantCulture;
            /// <summary>
            /// Gets or sets format provider to be used by ToStringWithCulture method.
            /// </summary>
            public System.IFormatProvider FormatProvider
            {
                get
                {
                    return this.formatProviderField ;
                }
                set
                {
                    if ((value != null))
                    {
                        this.formatProviderField  = value;
                    }
                }
            }
            /// <summary>
            /// This is called from the compile/run appdomain to convert objects within an expression block to a string
            /// </summary>
            public string ToStringWithCulture(object objectToConvert)
            {
                if ((objectToConvert == null))
                {
                    throw new global::System.ArgumentNullException("objectToConvert");
                }
                System.Type t = objectToConvert.GetType();
                System.Reflection.MethodInfo method = t.GetMethod("ToString", new System.Type[] {
                            typeof(System.IFormatProvider)});
                if ((method == null))
                {
                    return objectToConvert.ToString();
                }
                else
                {
                    return ((string)(method.Invoke(objectToConvert, new object[] {
                                this.formatProviderField })));
                }
            }
        }
        private ToStringInstanceHelper toStringHelperField = new ToStringInstanceHelper();
        /// <summary>
        /// Helper to produce culture-oriented representation of an object as a string
        /// </summary>
        public ToStringInstanceHelper ToStringHelper
        {
            get
            {
                return this.toStringHelperField;
            }
        }
        #endregion
    }
    #endregion
}
