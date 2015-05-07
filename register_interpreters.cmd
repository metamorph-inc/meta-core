: META_PATH=C:\Users\kevin\Documents\META\
Setlocal EnableDelayedExpansion
bin\Python27\Scripts\Python.exe -c "reg = __import__('win32com.client').client.DispatchEx('Mga.MgaRegistrar'); [reg.UnregisterComponent(progid, 2) for progid in reg.GetAssociatedComponentsDisp('CyPhyML', 7, 2)]" || exit /b !ERRORLEVEL!
bin\Python27\Scripts\Python.exe -c "import sys; import os.path; filename='generated\CyPhyML\models\CyPhyML.mta'; not os.path.isfile(filename) and sys.exit(0); reg = __import__('win32com.client').client.DispatchEx('Mga.MgaRegistrar'); reg.RegisterParadigmFromDataDisp('MGA=' + os.path.abspath(filename), 1)" || exit /b !ERRORLEVEL!
%windir%\SysWOW64\reg add HKLM\Software\META /v META_PATH /t REG_SZ /d "%~dp0\" /f || exit /b !ERRORLEVEL!
if exist "src\bin\CPMDecorator.dll" regsvr32 /s "src\bin\CPMDecorator.dll" || exit /b !ERRORLEVEL!
rem TODO: register Decorators?
if exist "src\CyPhyDecoratorAddon\bin\Release\CyPhyDecoratorAddon.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyDecoratorAddon\bin\Release\CyPhyDecoratorAddon.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyMdaoAddOn\bin\Release\CyPhyMdaoAddOn.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyMdaoAddOn\bin\Release\CyPhyMdaoAddOn.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyMetaLink\bin\Release\CyPhyMetaLink.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyMetaLink\bin\Release\CyPhyMetaLink.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhySignalBlocksAddOn\bin\Release\CyPhySignalBlocksAddOn.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhySignalBlocksAddOn\bin\Release\CyPhySignalBlocksAddOn.dll" || exit /b !ERRORLEVEL!
if exist "src\CLM_light\bin\Release\CLM_light.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CLM_light\bin\Release\CLM_light.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhy2CAD_CSharp\bin\Release\CyPhy2CAD_CSharp.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhy2CAD_CSharp\bin\Release\CyPhy2CAD_CSharp.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhy2Modelica_v2\bin\Release\CyPhy2Modelica_v2.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhy2Modelica_v2\bin\Release\CyPhy2Modelica_v2.dll" || exit /b !ERRORLEVEL!
if exist "src\bin\CyPhyCAExporter.dll" %windir%\SysWOW64\regsvr32 /s "src\bin\CyPhyCAExporter.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyComplexity\bin\Release\CyPhyComplexity.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyComplexity\bin\Release\CyPhyComplexity.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyComponentAuthoring\bin\Release\CyPhyComponentAuthoring.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyComponentAuthoring\bin\Release\CyPhyComponentAuthoring.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyComponentExporter\bin\Release\CyPhyComponentExporter.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyComponentExporter\bin\Release\CyPhyComponentExporter.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyComponentFidelitySelector\bin\Release\CyPhyComponentFidelitySelector.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyComponentFidelitySelector\bin\Release\CyPhyComponentFidelitySelector.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyComponentImporter\bin\Release\CyPhyComponentImporter.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyComponentImporter\bin\Release\CyPhyComponentImporter.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyComponentParameterEditor\bin\Release\CyPhyComponentParameterEditor.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyComponentParameterEditor\bin\Release\CyPhyComponentParameterEditor.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyDesignExporter\bin\Release\CyPhyDesignExporter.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyDesignExporter\bin\Release\CyPhyDesignExporter.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyDesignImporter\bin\Release\CyPhyDesignImporter.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyDesignImporter\bin\Release\CyPhyDesignImporter.dll" || exit /b !ERRORLEVEL!
if exist "src\bin\CyPhyDesignSpaceRefactor.dll" %windir%\SysWOW64\regsvr32 /s "src\bin\CyPhyDesignSpaceRefactor.dll" || exit /b !ERRORLEVEL!
if exist "src\bin\CyPhyDSRefiner.dll" %windir%\SysWOW64\regsvr32 /s "src\bin\CyPhyDSRefiner.dll" || exit /b !ERRORLEVEL!
if exist "src\bin\CyPhyElaborate.dll" %windir%\SysWOW64\regsvr32 /s "src\bin\CyPhyElaborate.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyElaborateCS\bin\Release\CyPhyElaborateCS.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyElaborateCS\bin\Release\CyPhyElaborateCS.dll" || exit /b !ERRORLEVEL!
if exist "src\bin\CyPhyFormulaEvaluator.dll" %windir%\SysWOW64\regsvr32 /s "src\bin\CyPhyFormulaEvaluator.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyMasterInterpreter\bin\Release\CyPhyMasterInterpreter.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyMasterInterpreter\bin\Release\CyPhyMasterInterpreter.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyMetaLink\bin\Release\CyPhyMetaLink.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyMetaLink\bin\Release\CyPhyMetaLink.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyMultiJobRun\bin\Release\CyPhyMultiJobRun.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyMultiJobRun\bin\Release\CyPhyMultiJobRun.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyPET\bin\Release\CyPhyPET.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyPET\bin\Release\CyPhyPET.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyPrepareIFab\bin\Release\CyPhyPrepareIFab.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyPrepareIFab\bin\Release\CyPhyPrepareIFab.dll" || exit /b !ERRORLEVEL!
if exist "src\Release\CyPhyPython.dll" %windir%\SysWOW64\regsvr32 /s "src\Release\CyPhyPython.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhyReliabilityAnalysis\bin\Release\CyPhyReliabilityAnalysis.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhyReliabilityAnalysis\bin\Release\CyPhyReliabilityAnalysis.dll" || exit /b !ERRORLEVEL!
if exist "src\CyPhySoT\bin\Release\CyPhySoT.dll" %windir%\Microsoft.NET\Framework\v4.0.30319\RegAsm.exe /codebase "src\CyPhySoT\bin\Release\CyPhySoT.dll" || exit /b !ERRORLEVEL!
if exist "src\bin\DesignSpaceHelper.dll" %windir%\SysWOW64\regsvr32 /s "src\bin\DesignSpaceHelper.dll" || exit /b !ERRORLEVEL!
