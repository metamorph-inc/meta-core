C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe build\msvc2010\muparser.sln /p:Configuration=Debug;Platform=x64
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe build\msvc2010\muparser.sln /p:Configuration=Release;Platform=x64
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe build\msvc2010\muparser.sln /p:Configuration=Debug;Platform=win32
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe build\msvc2010\muparser.sln /p:Configuration=Release;Platform=win32

powershell -ExecutionPolicy ByPass ipmo \"C:\Program Files (x86)\Outercurve Foundation\Modules\CoApp\CoApp.Powershell.Tools.dll\" ; Write-NuGetPackage .\muparser.autopkg
powershell -ExecutionPolicy ByPass ipmo \"C:\Program Files (x86)\Outercurve Foundation\Modules\CoApp\CoApp.Powershell.Tools.dll\" ; Write-NuGetPackage .\muparser-vc100.autopkg

rem nuget push -Source http://build.isis.vanderbilt.edu/  muparser-vc100.1.34.nupkg
rem nuget push -Source http://build.isis.vanderbilt.edu/  muparser.1.34.nupkg
