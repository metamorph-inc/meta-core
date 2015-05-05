$cptpd_name = "CREO_PARAMETRIC_TOOLKIT_PARENT_DIR"
$fso = New-Object -ComObject Scripting.FileSystemObject
$cptpd_value = $fso.GetFolder("C:\Program Files\PTC\Creo 2.0\Common Files\M030")
[Environment]::SetEnvironmentVariable($cptpd_name, $cptpd_value.ShortPath,"User")

pushd 'c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC'
cmd /c "vcvarsall.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
write-host "`nVisual Studio 2010 Command Prompt variables set." -ForegroundColor Yellow

# pushd CADCommonFunctions
# MSBuild.exe .\CADCommonFunctions.sln /target:Rebuild /property:Configuration=Release /property:Platform=x64
# popd

# pushd CADCreoParametricCommonFunctions
# MSBuild.exe .\CADCreoParametricCommonFunctions.sln /target:Rebuild /property:Configuration=Release /property:Platform=x64
# popd

# pushd CADCreoParametricToolkitFunctions
# MSBuild.exe .\CADCreoParametricToolkitFunctions.sln /target:Rebuild /property:Configuration=Release /property:Platform=x64
# popd

pushd CADCreoParametricCreateAssembly
MSBuild.exe .\CADCreoParametricCreateAssembly.sln /target:Rebuild /property:Configuration=Release /property:Platform=x64
& 'C:\Program Files\PTC\Creo 2.0\Parametric\bin\protk_unlock.bat' ./x64/Release/CADCreoParametricCreateAssembly.exe
copy-item ./x64/Release/CADCreoParametricCreateAssembly.exe $Env:PROE_ISIS_EXTENSIONS/bin/
popd


