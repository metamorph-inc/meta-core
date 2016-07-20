SetLocal EnableDelayedExpansion

"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" /p:"VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140";PlatformToolset=v140  build\msvc2010\muparser.sln /p:Configuration=Debug;Platform=x64 || exit /b !ERRORLEVEL!
"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" /p:"VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140";PlatformToolset=v140  build\msvc2010\muparser.sln /p:Configuration=Release;Platform=x64 || exit /b !ERRORLEVEL!
"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" /p:"VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140";PlatformToolset=v140  build\msvc2010\muparser.sln /p:Configuration=Debug;Platform=win32 || exit /b !ERRORLEVEL!
"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" /p:"VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140";PlatformToolset=v140  build\msvc2010\muparser.sln /p:Configuration=Release;Platform=win32 || exit /b !ERRORLEVEL!

powershell -ExecutionPolicy ByPass ipmo \"C:\Program Files (x86)\Outercurve Foundation\Modules\CoApp\CoApp.Powershell.Tools.dll\" ; Write-NuGetPackage .\muparser.autopkg || exit /b !ERRORLEVEL!
powershell -ExecutionPolicy ByPass ipmo \"C:\Program Files (x86)\Outercurve Foundation\Modules\CoApp\CoApp.Powershell.Tools.dll\" ; Write-NuGetPackage .\muparser-vc140.autopkg || exit /b !ERRORLEVEL!

rem nuget push -Source http://build.isis.vanderbilt.edu/  muparser-vc140.1.34.2.nupkg
rem nuget push -Source http://build.isis.vanderbilt.edu/  muparser.1.34.nupkg
