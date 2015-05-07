# Microsoft Developer Studio Project File - Name="SFCPrint" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SFCPrint - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SFCPrint.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SFCPrint.mak" CFG="SFCPrint - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SFCPrint - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SFCPrint - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SFCPrint - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./Udm" /I "../include" /I "$(UDM_PATH)/include" /I "$(UDM_PATH)/3rdparty/stl" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 UdmBase.lib UdmUtil.lib UdmDom.lib xerces-c_2.lib zlib.lib /nologo /subsystem:console /machine:I386 /out:"../../bin/SFCPrint.exe" /libpath:"$(UDM_PATH)/lib" /libpath:"$(UDM_PATH)/3rdParty/xerces/xerces-c2_2_0-win32/lib" /libpath:"$(UDM_PATH)/3rdparty/zlib"

!ELSEIF  "$(CFG)" == "SFCPrint - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./Udm" /I "../include" /I "$(UDM_PATH)/include" /I "$(UDM_PATH)/3rdparty/stl" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 UdmBaseD.lib UdmUtilD.lib UdmDomD.lib xerces-c_2D.lib zlibD.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../bin/SFCPrint_d.exe" /pdbtype:sept /libpath:"$(UDM_PATH)/lib" /libpath:"$(UDM_PATH)/3rdParty/xerces/xerces-c2_2_0-win32/lib" /libpath:"$(UDM_PATH)/3rdparty/zlib"

!ENDIF 

# Begin Target

# Name "SFCPrint - Win32 Release"
# Name "SFCPrint - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Udm\ECSL_DP.cpp
# End Source File
# Begin Source File

SOURCE=.\Udm\EDP2SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\Udm\LINKS.cpp
# End Source File
# Begin Source File

SOURCE=.\Udm\SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\SFCprint.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Udm\ECSL_DP.h
# End Source File
# Begin Source File

SOURCE=.\Udm\EDP2SFC.h
# End Source File
# Begin Source File

SOURCE=.\Udm\LINKS.h
# End Source File
# Begin Source File

SOURCE=.\Udm\SFC.h
# End Source File
# Begin Source File

SOURCE=..\include\SFCprint.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
