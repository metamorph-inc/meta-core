
#define VCP_PATH GetEnv('VCP_PATH')
#if VCP_PATH == ""
#define VCP_PATH "C:\src\VCP"
#endif

#define GREAT_PATH GetEnv('GREAT_PATH')
#if GREAT_PATH == ""
#define GREAT_PATH "C:\Program Files\ISIS\GReAT"
#endif

#define UDM_PATH GetEnv('UDM_PATH')
#if UDM_PATH == ""
#define UDM_PATH "C:\Program Files\ISIS\UDM"
#endif

#define UDM_3RDPARTY_PATH GetEnv('UDM_3RDPARTY_PATH')
#if UDM_3RDPARTY_PATH == ""
#define UDM_3RDPARTY_PATH "C:\Program Files\ISIS\UDM\3rdParty"
#endif

#define MSVCDLL_PATH "C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86\Microsoft.VC80.CRT"

[Setup]
AppName=ECSL_DP2Java
AppVerName=ECSL_DP2Java version 1.1
DefaultDirName={pf}\ISIS\ECSL_DP2Java


[Files]
Source: "{#MSVCDLL_PATH}\*";											DestDir: "{app}\bin";
Source: "{#VCP_PATH}\bin\MDL2MGA.exe";									DestDir: "{app}\bin";
Source: "{#VCP_PATH}\MICTES-deploy\ECSL_DP_copy.bat";					DestDir: "{app}\bin";
Source: "{#VCP_PATH}\ECSL_DP2Java\ECSL_DP2Java.bat";					DestDir: "{app}\bin";
Source: "{#VCP_PATH}\ECSL_DP2Java\ECSL_DP2Java.pat";					DestDir: "{app}\bin";
Source: "{#UDM_PATH}\bin\UDMOclPat.exe";								DestDir: "{app}\bin";
Source: "{#UDM_PATH}\bin\UdmCopy.exe";									DestDir: "{app}\bin";
Source: "{#UDM_PATH}\lib\UdmDll.lib";									DestDir: "{app}\lib";
Source: "{#UDM_PATH}\bin\UdmDll_3_1_2.dll";								DestDir: "{app}\bin";
Source: "{#UDM_3RDPARTY_PATH}\xerces-c_2_5_0\bin\xerces-c_2_5_0.dll";	DestDir: "{app}\bin";
Source: "{#VCP_PATH}\autobuild\RegParadigmXmp.vbs";						DestDir: "{app}\etc";
Source: "{#VCP_PATH}\autobuild\CallRegParadigm.bat";					DestDir: "{app}\etc";
Source: "{#VCP_PATH}\ECSL_DP\ECSL_DP_udm.xml";							DestDir: "{app}\Meta";
Source: "{#VCP_PATH}\ECSL_DP\ECSL_DP.xmp";								DestDir: "{app}\Meta";
Source: "{#VCP_PATH}\ECSL_DP\icons\*.bmp";								DestDir: "{app}\Meta\icons";
Source: "{#VCP_PATH}\ECSL_DP\ECSL_DP.xsd";								DestDir: "{app}\Meta";
Source: "{#VCP_PATH}\ECSL_DP2Java\README.txt";							DestDir: "{app}";
Source: "{#VCP_PATH}\ECSL_DP2Java\fixedphone.mdl";						DestDir: "{app}\examples";
Source: "{#VCP_PATH}\ECSL_DP2Java\State.java";							DestDir: "{app}\examples";

[Code]
function ParsePathToArray( path: String ) : TArrayOfString;
var
  pathArr:  TArrayOfString;
  ix:       Integer;
  semPos:   Integer;

begin

  if path = '' then
  begin

    Result := pathArr;

  end else begin

    if Copy(  path, Length( path ) - 1, Length( path )  ) <> ';' then
    begin
      path := path + ';';
    end

    ix := 0;
    semPos := Pos(';', path);

    while ( semPos > 0 ) do
    begin
      SetArrayLength( pathArr, ix + 1 );
      pathArr[ix] := Copy( path, 1, semPos - 1 );
      path := Copy(  path, semPos + 1, Length( path )  );

      ix := ix + 1;
      semPos := Pos(';', path);
    end;

    Result := pathArr;
  end;

end;


function CatArrayToPath( pathArr: TArrayOfString ) : String;
var
  path:  String;
  ix:    Integer;

begin

  if GetArrayLength( pathArr ) = 0 then
  begin
  
    Result := '';
    
  end else begin

    path := pathArr[0];
    ix := 1;
    while(  ix < GetArrayLength( pathArr )  ) do
    begin
      path := path + ';' + pathArr[ix];
      ix := ix + 1;
    end;
  
    Result := path;
  end;

end;


procedure ModPath(inpath: String);
var
  inpathArr:  TArrayOfString;

  oldpath:    String;
  oldpathArr: TArrayOfString;
  
  ix:         Integer;
  jx:         Integer;
  kx:         Integer;
  
  addPath:    Boolean;
  
begin
  // Modify WinNT path
  if UsingWinNT() = true then
  begin

    // Get current path, split into an array
    RegQueryStringValue( HKEY_CURRENT_USER, 'Environment', 'PATH', oldpath );
    
    if oldpath = '' then begin
  
      if IsUninstaller() <> true then
      begin
        RegWriteStringValue( HKEY_CURRENT_USER, 'Environment', 'PATH', inpath );
      end;

    end else begin

      inpathArr := ParsePathToArray( inpath );
      oldpathArr := ParsePathToArray( oldpath );

      if IsUninstaller() = true then
      begin

        ix := 0; jx := 0;

        while ix < GetArrayLength( oldpathArr ) do
        begin

          kx := 0;
          while kx < GetArrayLength( inpathArr ) do
          begin
          
            if oldpathArr[jx] = inpathArr[kx] then
            begin
              ix := ix + 1;
              break;
            end;

            kx := kx + 1;
          end;

          oldpathArr[jx] := oldpathArr[ix];

          ix := ix + 1;
          jx := jx + 1;

        end;


        SetArrayLength( oldpathArr, jx - 1 );

      end else begin

        ix := 0;
        while ix < GetArrayLength( inpathArr ) do
        begin

          addPath := true;
          jx := 0;
          while jx < GetArrayLength( oldpathArr ) do
          begin
            if ( oldpathArr[jx] = inpathArr[ix] ) then
            begin
              addPath := false;
              break;
            end;
            jx := jx + 1;
          end;

          if addPath = true then
          begin
            SetArrayLength(  oldpathArr, GetArrayLength( oldpathArr ) + 1  );
            oldpathArr[GetArrayLength( oldpathArr ) - 1] := inpathArr[ix];
          end;

          ix := ix + 1;

        end;
      end;

      RegWriteStringValue(  HKEY_CURRENT_USER, 'Environment', 'PATH', CatArrayToPath( oldpathArr )  );
    end;
    
  end;
  
end;


procedure CurStepChanged( CurStep: TSetupStep);
var
  appPath: String;
begin
  if CurStep = ssInstall then
  begin
    appPath := ExpandConstant( '{app}' );
    ModPath( appPath + '\etc;' + appPath + '\bin' );
  end
end;


procedure CurUninstallStepChanged( CurUninstallStep: TUninstallStep);
var
  appPath: String;
begin
  if CurUninstallStep = usUninstall then
  begin
    appPath := ExpandConstant( '{app}' );
    ModPath( appPath + '\etc;' + appPath + '\bin' );
  end
end;


[Run]
Filename: {app}\etc\CallRegParadigm.bat; Parameters: """{app}\Meta\ECSL_DP.xmp"""; Flags: skipifdoesntexist runhidden
