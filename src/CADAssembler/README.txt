The makefiles for the CAD Assembler require that certain
environment parameters be set:

Software Dependencies
--------------------------

## UDM

The UDM tools will also be needed (as long as the XML reader is needed).

  http://repo.isis.vanderbilt.edu/UDM/3.2.12/
  UDM_x64-3.2.12.msi

UDM_x64-3.2.12.msi

## Creo 2.0
(If you need to build with Creo 1.0 see an 
earlier version of this file)

Currently, the build requires Creo 2.0
The installation consists of two ISO disks.
Upon running the setup.exe you will be presented with
the license screen.

[Robert O., Mike M. and Fred E. have a copy of the installer, it is quite large]
The you will need to run the custom installer to
get all the necessary components.
The following components need to be installed:

### Creo Customization

Activate the following...

"Applications" -> [Customize ...]  (a button)
  "Application Custom Settings"    (a window)
     "Creo Parametric"        (a list item)
       "API Toolkits"   (a menu heading)
         "J-Link"
         "WebLink"
         "Pro/Toolkit"
         "Object Toolkit"

     "Creo Simulate"   (a list item)


Environment 
-------------------

Here is how to set the value via PowerShell...

  $cptpd_name = "CREO_PARAMETRIC_TOOLKIT_PARENT_DIR"

  $fso = New-Object -ComObject Scripting.FileSystemObject
  $cptpd_value = $fso.GetFolder("C:\Program Files\PTC\Creo 2.0\Common Files\M030")

  [Environment]::SetEnvironmentVariable($cptpd_name, $cptpd_value.ShortPath,"User")


Note that the enviroment parameter uses the DOS 8.3 path.
This provides a path with no blanks.

This changes the environment parameter setting but not the 
environment for the current process.



Configure current console
----------------------------------

You can start a Visual Studio console or run the following script in your
current console...

  pushd 'c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC'
  cmd /c "vcvarsall.bat&set" |
  foreach {
    if ($_ -match "=") {
      $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
    }
  }
  popd
  write-host "`nVisual Studio 2010 Command Prompt variables set." -ForegroundColor Yellow



Version Management
------------------------

Each change should get a distinct revision.
The revision information is encoded in 
"...meta-dev/src/CADAssembler/CADCreoParametricCreateAssembly/ISISVersionNumber.h" 
and documented in 
"...meta-dev/src/CADAssembler/CADCreoParametricCreateAssembly/0Readme - CreateAssembly.txt".



Build Order
-------------

 MSBuild.exe ..\..\make_CAD.msbuild /t:MetaLink

 MSBuild.exe .\CADCreoParametricCreateAssembly.sln /target:Rebuild /property:Configuration=Release /property:Platform=x64

   CADCreoParametricCreateAssembly.vcxproj now contains Project Dependencies to its CADAssembly dependent projects, so MSBuild and Visual Studio know the correct build order.

Visual Studio IDE
---------------------

If you open the *.sln file for any of these solutions you will need 
to set the Configuration to "Release" and the Platform to "x64".



Prepare for deployment
--------------------------

In the case where the program needs to be run in an unlicense locked
environment (all the downstream users) the exe will need to be unlocked.

& 'C:\Program Files\PTC\Creo 2.0\Parametric\bin\protk_unlock.bat' .\CADCreoParametricCreateAssembly.exe

MSBuild.exe ..\..\make_CAD.msbuild /t:Push_NuGet


Notes
---------------

I found that Visual Studio 2010 without service pack 1 would not link.
http://stackoverflow.com/questions/12267158/failure-during-conversion-to-coff-file-invalid-or-corrupt?lq=1


