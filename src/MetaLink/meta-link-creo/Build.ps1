

pushd 'c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC'
cmd /c "vcvarsall.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
write-host "`nVisual Studio 2010 Command Prompt variables set." -ForegroundColor Yellow

pushd premake4
#  MSBuild.exe .\MetaLinkLib.vcxproj /p:Configuration=Release /property:Platform=x64
  MSBuild.exe .\MetaLinkTest.vcxproj /p:Configuration=Release /property:Platform=x64
popd
