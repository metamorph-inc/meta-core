function CheckVersions(): Boolean;
var
  gme_path : String;
  oGME :  Variant;
  udm_path : String;
  MS, LS: Cardinal;
  great_path : String;
  vs_path : String;
begin
  Result := True;
#ifdef REQUIRES_GMEVERSION
  gme_path := GetEnv('GME_ROOT');
  // CASE #1: No GME installed
  if gme_path = '' then begin
    MsgBox('Unable to locate GME r{#REQUIRES_GMEVERSION}.  Please install GME r{#REQUIRES_GMEVERSION} first.', mbError, MB_OK);
    Result := False;
  // ELSE: wrong GME installed
  end else begin
	oGME := CreateOleObject('GME.Application');
	if oGME.Version <> '{#REQUIRES_GMEVERSION}' then begin
		Result := MsgBox('This version of {#APP_LONGNAME} ({#APP_VERSION}) is recommended for use with GME {#REQUIRES_GMEVERSION}. The currently installed GME version is '+oGME.Version+'.'#13#13+'Are you sure you wish to continue with the installation?', mbConfirmation, MB_YESNO) = idYes;
	end
	IDispatchInvoke(oGME, False, 'Exit', []);
  end;
#endif

#ifdef REQUIRES_UDMVERSION
  udm_path := GetEnv('UDM_PATH');
  if udm_path = '' then begin
    MsgBox('Unable to locate UDM {#REQUIRES_UDMVERSION}. Please install UDM first.', mbError, MB_OK);
    Result := False;
  end else begin
    if GetVersionNumbers(ExpandConstant(udm_path + '/bin/{#UDMDLL}'), MS, LS) then begin
      // MS is checked through dll name
      if LS < ${#UDM_PATCH} then begin
        Result := Result and (MsgBox('This version of {#APP_LONGNAME} ({#APP_VERSION}) requires UDM {#REQUIRES_UDMVERSION}. ' +
          'The current version of UDM is ' + Format('%d.%d.%d', [MS shr 16, MS and $FFFF, LS shr 16]) + '. ' +
          'Are you sure you wish to continue the installation?', mbConfirmation, MB_YESNO) = idYes);
      end
    end else begin
      Result := Result and (MsgBox('This version of {#APP_LONGNAME} ({#APP_VERSION}) is recommended for use with UDM {#REQUIRES_UDMVERSION}. ' +
        '{#APP_LONGNAME} setup could not find {#UDMDLL} and will not operate correctly without it.'#13#13 +
        'Are you sure you wish to continue the installation?', mbConfirmation, MB_YESNO) = idYes);
    end
  end
#endif

#ifdef REQUIRES_GREATVERSION
  great_path := GetEnv('GREAT_PATH');
  if great_path = '' then begin
    MsgBox('Unable to locate GReAT. Please install GReAT first.', mbError, MB_OK);
    Result := False;
  end
#endif

#ifdef REQUIRES_VS9
  RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\MicroSoft\VisualStudio\9.0', 'InstallDir', vs_path);
  if vs_path = '' then begin
	RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\MicroSoft\VCExpress\9.0', 'InstallDir', vs_path);
		if vs_path = '' then begin
		Result := Result and (MsgBox('Visual Studio 2008 is not installed. {#APP_LONGNAME} {#APP_VERSION} needs Visual Studio 2008 to run.'+#13#13 +
			'Do you really want to install {#APP_LONGNAME} {#APP_VERSION} on your machine?', mbConfirmation, MB_YESNO) = idYes);
	end
  end
#endif

end;


#ifndef CUSTOM_NEXTBUTTON_CLICK
function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  if (CurPageID = wpInfoBefore) and not WizardSilent() then begin
    Result := CheckVersions();
  end;
end;
#endif

