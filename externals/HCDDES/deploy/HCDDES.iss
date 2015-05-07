;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; InnoSetup Installer for the HCDDES project's main product.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define APP_LONGNAME 	= "ESMoL Tool Suite"
#define APP_SHORTNAME	= "ESMoL"
#define TRUNK_PATH		= SourcePath + "\.."

#define TURDPARTY_PATH GetEnv('HCDDES_THIRDPARTY_PATH')
#if TURDPARTY_PATH == ""
#define TURDPARTY_PATH	= TRUNK_PATH + "\..\3rdparty"
#endif

#define UDM_PATH GetEnv('UDM_PATH')
#if UDM_PATH == ""
#error UDM_PATH is not defined
#endif

;; Common, ISIS #defines
#include TRUNK_PATH + "\common-scripts\iss\defines.iss"

#define REQUIRES_MATLAB "7.13"
#define REQUIRES_MATLAB_STRING "R2011B"



[Setup]
AppID={{B9A3FACC-F2D1-451C-B1A4-48A84231E684}
#include TRUNK_PATH + "\common-scripts\iss\setup.iss"
DefaultDirName={pf32}\ISIS\{#APP_SHORTNAME}
ArchitecturesInstallIn64BitMode=x64
ChangesEnvironment=yes
RestartIfNeededByRun=false
SetupLogging=yes
InfoBeforeFile={#TRUNK_PATH}\readme.txt


[Dirs]
Name: {app}\bin
Name: {app}\config
Name: {app}\doc
Name: {app}\doc\papers
Name: {app}\examples
Name: {app}\examples\example1\
Name: {app}\examples\example2\
;ame: {app}\examples\example1\targets\hardware\build; Flags: deleteafterinstall
Name: {app}\meta
Name: {app}\meta\icons
Name: {app}\python
Name: {app}\templates
[Files]
;; =============================================================================
;; Misc Utilities
;; =============================================================================
Source: {#TRUNK_PATH}\common-scripts\gmepy.exe; DestDir: {app}\bin
; gmepy.exe has some dependencies. Install them, or the SxS context creation will fail
Source: {#TRUNK_PATH}\common-scripts\MetaGME2Uml.dll; DestDir: {app}\bin
Source: {#TRUNK_PATH}\common-scripts\ISIS.MetaGME2Uml.manifest; DestDir: {app}\bin
Source: {#TRUNK_PATH}\common-scripts\CSharpDSMLGenerator.dll; DestDir: {app}\bin
Source: {#TRUNK_PATH}\common-scripts\ISIS.GME.Common.dll; DestDir: {app}\bin
Source: {#TRUNK_PATH}\common-scripts\unzip.exe; DestDir: {app}\bin
Source: {#TRUNK_PATH}\common-scripts\zip.exe; DestDir: {app}\bin
;; =============================================================================
;; Primary Artifacts:
;; =============================================================================
Source: {#TRUNK_PATH}\src\models\ESMoL\ESMoL.xme; DestDir: {app}\meta; Flags: ignoreversion
Source: {#TRUNK_PATH}\generated\ESMoL\models\ESMoL.xmp; DestDir: {app}\meta; Flags: ignoreversion 
Source: {#TRUNK_PATH}\src\models\ESMoL\icons\*; DestDir: {app}\meta\icons; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\C_CodeGen.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\MDL2MGA.exe; DestDir: {app}\bin; Flags: ignoreversion; Check: not IsWin64
Source: {#TRUNK_PATH}\x64\bin\MDL2MGA.exe; DestDir: {app}\bin; Flags: ignoreversion; Check: IsWin64
; On x64, it's possible (but unlikely) the user has 32bit Matlab
Source: {#TRUNK_PATH}\bin\MDL2MGA.exe; DestDir: {app}\bin; DestName: MDL2MGA_x86.exe; Flags: ignoreversion; Check: IsWin64
Source: {#TRUNK_PATH}\bin\SchedResults.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\SchedTool.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\SchedViz.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\SF_CodeGen.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\SL_CodeGen.exe; DestDir: {app}\bin; Flags: ignoreversion 
;ource: {#TRUNK_PATH}\bin\SL_CodeGenD.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\Stage1.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\Stage2.exe; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\templates\*; DestDir: {app}\templates; Flags: ignoreversion 
Source: {#TRUNK_PATH}\config\frodo; DestDir: {app}\config; Flags: ignoreversion 
; Cheetah Templates
Source: {#TRUNK_PATH}\generated\BlockTemplate\Python\*; DestDir: {app}\python; Flags: recursesubdirs ignoreversion
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Simulink model updater/refresher component (requires Udm)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Source: {#TRUNK_PATH}\bin\ESMoLUpdate.dll; DestDir: {app}\bin; Flags: ignoreversion regserver

; Include Udm for the above
; n.b do not use ignoreversion here
Source: {#TRUNK_PATH}\bin\UdmDll_3_2_VS10.dll; DestDir: {syswow64}
Source: {#TRUNK_PATH}\bin\xerces-c_2_8.dll; DestDir: {syswow64}

; TODO: need a path for this (but ESMoL needs to build against 32bit MATLAB, and 64bit is installed by default on x64 machines)
;Source: {#UDM}\bin\UdmDll_3_2_v100_x64.dll; DestDir: {sys}; Flags: ignoreversion; Check: IsWin64
;Source: {#UDM}\bin\xerces-c_2_8_x64.dll; DestDir: {sys}; Flags: ignoreversion; Check: IsWin64
;Source: {#UDM}\bin\zlibwapi.dll; DestDir: {sys}; Flags: ignoreversion; Check: IsWin64

;; =============================================================================
;; 3rd-Party, Runtime Dependencies
;; =============================================================================

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Gecode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Source: {#TRUNK_PATH}\nuget\gecode-vc100.redist.3.7.3\build\native\bin\Win32\v100\Release\Gecode*.dll; DestDir: {app}\lib; Flags: ignoreversion 
; Gecode(cont): Note that the following 2 are deposited in {bin} rather than {lib} to avoid ${PATH} conflict with MATLAB
Source: {#TRUNK_PATH}\bin\QtGui4.dll; DestDir: {app}\bin; Flags: ignoreversion 
Source: {#TRUNK_PATH}\bin\QtCore4.dll; DestDir: {app}\bin; Flags: ignoreversion 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Google CTemplates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define CTEMPLATE_PATH GetEnv('CTEMPLATE_PATH')
#if     CTEMPLATE_PATH == ""
#define CTEMPLATE_PATH =  TURDPARTY_PATH + "\ctemplate-0.96"

#endif
Source: {#CTEMPLATE_PATH}\Release\libctemplate.dll; DestDir: {app}\lib; Flags: ignoreversion 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MATLAB dependencies (e.g. Aero SIMULINK model library)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Source: {#TRUNK_PATH}\sandbox\lib\aero_sim_lib.mdl; DestDir: {app}\lib; Flags: ignoreversion
Source: {#TRUNK_PATH}\bin\init_ttenv.m; DestDir: {app}\bin

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Python
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define PYTHON_PATH GetEnv('PYTHON_HOME')
#if 	PYTHON_PATH == ""
#define PYTHON_PATH =  "C:\Python27"
#endif
Source: {#TRUNK_PATH}\bin\python27.dll; DestDir: {app}\lib; Flags: ignoreversion 
Source: {#TRUNK_PATH}\generated\BlockTemplate\Python\*; DestDir: {app}\python\Cheetah; Flags: recursesubdirs ignoreversion
; Source: {#TRUNK_PATH}\py2exe\dist\*; DestDir: {app}\python; Flags: recursesubdirs ignoreversion

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TrueTime
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Source: {#TURDPARTY_PATH}\truetime-2.0-beta5.zip; DestDir: {app}\lib; Flags: nocompression

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Xerces
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Source: ..\bin\xerces-c_2_8.dll; DestDir: {app}\lib




;; =============================================================================
;; Documentation:
;; =============================================================================
Source: ..\doc\UsersManual.pdf; DestDir: {app}\doc
Source: ..\doc\papers\RSP10\RSP10.pdf; DestDir: {app}\doc\papers
Source: ..\doc\papers\ESWeek09\Scheduler.pdf; DestDir: {app}\doc\papers
Source: ..\doc\papers\TECSSynthesis\tecssynthesis.pdf; DestDir: {app}\doc\papers
Source: ..\doc\papers\TechReports\*.pdf; DestDir: {app}\doc\papers
Source: ..\doc\papers\ESMoL_TR\ESMoL_TR_opt.pdf; DestDir: {app}\doc\papers
;; =============================================================================
;; Examples:
;; =============================================================================
#define EXAMPLES_DIR = "{app}\examples"
;define EXAMPLES_DIR = "{commondocs}\ISIS\{#APP_SHORTNAME}\examples"
Source: ..\examples\QuadIntegrator\model\*; DestDir: {#EXAMPLES_DIR}\example1\model; Flags: recursesubdirs ignoreversion
Source: ..\examples\QuadIntegrator\targets\*; DestDir: {#EXAMPLES_DIR}\example1\targets; Flags: recursesubdirs ignoreversion
Source: ..\examples\QuadIntegrator\tests\test-all.cmd; DestDir: {#EXAMPLES_DIR}\example1\tests; Flags: ignoreversion
Source: ..\sandbox\quad_integrator\build\*; DestDir: {#EXAMPLES_DIR}\example1\targets\hardware2\build; Flags: recursesubdirs ignoreversion
Source: ..\sandbox\quad_integrator\pkgs\*; DestDir: {#EXAMPLES_DIR}\example1\targets\hardware2\pkgs; Flags: ignoreversion
Source: ..\sandbox\quad_integrator\qint_xpc.mdl; DestDir: {#EXAMPLES_DIR}\example1\targets\hardware2; Flags: ignoreversion
Source: ..\sandbox\quad_integrator\qint_xpc_acq.mdl; DestDir: {#EXAMPLES_DIR}\example1\targets\hardware2; Flags: ignoreversion
Source: ..\examples\QuadRotor\model\*; DestDir: {#EXAMPLES_DIR}\example2\model; Flags: recursesubdirs ignoreversion
Source: ..\examples\QuadRotor\targets\*; DestDir: {#EXAMPLES_DIR}\example2\targets; Flags: recursesubdirs ignoreversion
Source: ..\examples\QuadRotor\tests\*; DestDir: {#EXAMPLES_DIR}\example2\tests; Flags: recursesubdirs ignoreversion




[Run]
Filename: {app}\bin\unzip.exe; Parameters: -u *.zip; WorkingDir: {app}\lib; StatusMsg: Unpacking 3rd Party Dependencies
; This registers ESMoL in HKCU: other users will not have it registered. 64bit GME will have it registered (good for x64 MDL2MGA)
Filename: {app}\bin\gmepy.exe; Parameters: regxmp ESMoL.xmp; WorkingDir: {app}\meta; StatusMsg: Registering ESMoL Paradigm
Filename: {app}\bin\zip.exe; Parameters: -r cross-compiler.zip build; WorkingDir: {app}\examples\example1\targets\hardware2; StatusMsg: Preparing Gumstix/Robostix cross-compiler environment
;;Filename: cmd.exe; Parameters: /c del /s/q build; WorkingDir: {app}examples\example1\targets\hardware2; StatusMsg: Removing temporary files
;; TODO: uninstall for this
Filename: {code:getMatlabDir}\bin\matlab.exe; Parameters: "-nosplash -nodesktop -wait -r ""cd('{app}\bin');init_ttenv;savepath;exit;"""; StatusMsg: Initalizing TrueTime for {code:getMatlabDir}; Flags: skipifdoesntexist

[Registry]
Root: HKCU; Subkey: Environment; ValueType: string; ValueName: HCDDES_PATH; ValueData: {app}; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKCU; Subkey: Environment; ValueType: string; ValueName: HCDDES_CONFIG_PATH; ValueData: {app}; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKCU; Subkey: Environment; ValueType: string; ValueName: VCP_PATH; ValueData: {app}; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKCU; Subkey: Environment; ValueType: string; ValueName: VCP_MATLAB_PATH; ValueData: {code:getMatlabDir}\bin\win32; Flags: uninsdeletevalue; Check: not IsWin64
Root: HKCU; Subkey: Environment; ValueType: string; ValueName: VCP_MATLAB_PATH; ValueData: {code:getMatlabDir}\bin\win64; Flags: uninsdeletevalue; Check: IsWin64
; truetime requires TTKERNEL environment variable
Root: HKCU; Subkey: Environment; ValueType: string; ValueName: TTKERNEL; ValueData: {app}\lib\truetime-2.0-beta5\kernel; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKCU; Subkey: "Environment"; ValueType: expandsz; ValueName: Path; ValueData: "{olddata};%VCP_PATH%\bin;%VCP_PATH%\lib;%VCP_MATLAB_PATH%"; Check: "NeedsAddPath('%VCP_PATH%\bin;%VCP_PATH%\lib;%VCP_MATLAB_PATH%')"
Root: HKLM32; Subkey: Software\ESMoL; ValueType: string; ValueName: TTKERNEL; ValueData: {app}\lib\truetime-2.0-beta5\kernel; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKLM32; Subkey: Software\ESMoL; ValueType: string; ValueName: VCP_PATH; ValueData: {app}; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKLM64; Subkey: Software\ESMoL; ValueType: string; ValueName: TTKERNEL; ValueData: {app}\lib\truetime-2.0-beta5\kernel; Flags: createvalueifdoesntexist uninsdeletevalue; Check: IsWin64
Root: HKLM64; Subkey: Software\ESMoL; ValueType: string; ValueName: VCP_PATH; ValueData: {app}; Flags: createvalueifdoesntexist uninsdeletevalue; Check: IsWin64


[UninstallDelete]
Type: filesandordirs; Name: {app}\lib

[Code]
#include TRUNK_PATH + "\common-scripts\iss\code.iss"

function hasRecommendedMatlab(): Boolean;
var
  key : String;
  matlabVersions : TArrayOfString;
  latestMatlabVersion : String;
begin
  key := 'Software\Mathworks\MATLAB\';
  if RegGetSubkeyNames(HKEY_LOCAL_MACHINE, key, matlabVersions) then
  begin
    // FIXME: maybe HKLM\Software\Mathworks\MATLAB\ is empty
    latestMatlabVersion := matlabVersions[ GetArrayLength(matlabVersions)-1 ];
    case latestMatlabVersion of
    '7.8' :
      Result := True;
    '7.9' :
      Result := True;
    '7.10':
      Result := True;
    '7.11':
      Result := True;
    '7.12':
      Result := True;
    '7.13':
      Result := True;
    else
      Result := False;
    end;
  end else
  begin
    Result := False;
  end;
end;


function compareTwoVersionStrings(const v1, v2:String): String;
var
  major1 : Longint;
  major2 : Longint;
  minor1 : Longint;
  minor2 : Longint;
  ndx1 : Integer;
  ndx2 : Integer;
  dbg_msg : String;
begin
  ndx1 := Pos('.', v1);
  ndx2 := Pos('.', v2);
  // FIXME: maybe ndx isn't positive
  major1 := StrToIntDef( Copy(v1, 0, ndx1-1), 0 );
  major2 := StrToIntDef( Copy(v2, 0, ndx2-1), 0 );
  minor1 := StrToIntDef( Copy(v1, ndx1+1, Length(v1)-ndx1), 0 );
  minor2 := StrToIntDef( Copy(v2, ndx2+1, Length(v2)-ndx2), 0 );

  if major1 > major2 then
    Result := v1
  else if major2 > major1 then
    Result := v2
  else if minor1 > minor2 then
    Result := v1
  else
    Result := v2;

  //dbg_msg := IntToStr(major1)+'.'+IntToStr(minor1) +' <> '+ IntToStr(major2)+'.'+IntToStr(minor2) + ' --> '+ Result;
  //MsgBox(dbg_msg, mbInformation, MB_OK);
end;
function findBiggestVersionString(versions:TArrayOfString): String;
var
  ndx : Integer;
begin
  Result := versions[0];
  for ndx := 0 to GetArrayLength(versions) - 1 do
    Result := compareTwoVersionStrings(Result, versions[ndx]);
end;

function getMatlabDir(ignore:String): String;
var
  key : String;
  matlabVersions : TArrayOfString;
  latestMatlabVersion : String;
  debug_I: Integer;
  debug_S: String;
begin
  key := 'Software\Mathworks\MATLAB\';
  if RegGetSubkeyNames(HKEY_LOCAL_MACHINE, key, matlabVersions) then
  begin
    debug_S := '';
    for debug_I := 0 to GetArrayLength(matlabVersions)-1 do
      debug_S := debug_S + matlabVersions[debug_I] + #13#10;

    latestMatlabVersion := findBiggestVersionString(matlabVersions);

    //MsgBox('Found MATLAB Versions:'#13#10#13#10 + debug_S + 'CHOSE: ' + latestMatlabVersion, mbInformation, MB_OK);
  end else
  begin
    latestMatlabVersion := '{#REQUIRES_MATLAB}';
  end;

  key := 'Software\Mathworks\MATLAB\' + latestMatlabVersion;
  RegQueryStringValue(HKEY_LOCAL_MACHINE, key, 'MATLABROOT', Result);
end;


function InitializeSetup(): Boolean;
var
  foundPython : Boolean;
  foundMATLAB : Boolean;
begin
  Result := True;
  foundMATLAB := True;

  /////////////////////////////////////////////////////////
  // Check for MATLAB now
  /////////////////////////////////////////////////////////
  foundMATLAB := hasRecommendedMatlab();
  if (not(foundMATLAB)) then
  begin
    foundMATLAB := MsgBox('Warning: {#APP_LONGNAME} suggests MATLAB version {#REQUIRES_MATLAB} ({#REQUIRES_MATLAB_STRING}) to be installed.'+#13#13+'Are you sure you wish to continue with the installation?', mbConfirmation, MB_YESNO) = idYes;
  end
  Result := Result and foundMATLAB;

end;

function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'Path', OrigPath)
  then begin
    Result := True;
    exit;
  end;
  // look for the path with leading and trailing semicolon
  // Pos() returns 0 if not found
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  RemovePath, Path: String;
begin
  if (CurUninstallStep = usPostUninstall)
     and (RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'PATH', Path)) then
  begin
    RemovePath := '%VCP_PATH%\bin;%VCP_PATH%\lib;%VCP_MATLAB_PATH%'
    if Pos(LowerCase(RemovePath), Lowercase(Path)) <> 0 then
    begin
      StringChange(Path, RemovePath, '');
      StringChange(Path, ';;', ';');
      RegWriteStringValue(HKEY_CURRENT_USER, 'Environment', 'PATH', Path);
    end;
  end;
end;
