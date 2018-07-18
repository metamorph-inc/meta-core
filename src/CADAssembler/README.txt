Software Dependencies
--------------------------

## Visual Studio

Versions 2012 and 2015 should be installed.

## Java JDK

Install Oracle JDK version 8 or later.

## UDM

The UDM tools are also be needed.

  http://repo.isis.vanderbilt.edu/UDM/3.2.15/
  UDM_x64-3.2.15.msi

UDM_x64-3.2.15.msi

## Creo 3.0

Currently, the build requires Creo 3.0

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

The .sln for the CAD Assembler require that certain
environment variable be set. make_CAD.msbuild sets this variable automatically.

Here is how to set the value via PowerShell...

  $cptpd_name = "CREO_PARAMETRIC_TOOLKIT_PARENT_DIR"

  $fso = New-Object -ComObject Scripting.FileSystemObject
  $cptpd_value = $fso.GetFolder("C:\Program Files\PTC\Creo 3.0\M060\Common Files")

  [Environment]::SetEnvironmentVariable($cptpd_name, $cptpd_value.ShortPath,"User")

This changes the environment parameter setting but not the 
environment for the current process.


Version Management
------------------------

Each change should get a distinct revision.
The revision information is encoded in 
"src/CADAssembler/CADCreoParametricCreateAssembly/ISISVersionNumber.h"
and documented in 
"src/CADAssembler/CADCreoParametricCreateAssembly/0Readme - CreateAssembly.txt".



Build
-------------

"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" make_CAD.msbuild /m

Visual Studio IDE
---------------------

If you open the *.sln file for any of these solutions you will need 
to set the Configuration to "Release" and the Platform to "x64".



Prepare for deployment
--------------------------

The make_CAD.msbuild file calls 'protk_unlock.bat' on the compiled executables.

