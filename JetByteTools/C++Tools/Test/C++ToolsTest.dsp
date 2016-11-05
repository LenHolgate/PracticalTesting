# Microsoft Developer Studio Project File - Name="C++ToolsTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=C++ToolsTest - Win32 Unicode Debug No PreComp
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "C++ToolsTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "C++ToolsTest.mak" CFG="C++ToolsTest - Win32 Unicode Debug No PreComp"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "C++ToolsTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "C++ToolsTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "C++ToolsTest - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "C++ToolsTest - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "C++ToolsTest - Win32 Unicode Debug No PreComp" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "C++ToolsTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Output\VC6\Release"
# PROP Intermediate_Dir "Output\VC6\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"$(STLPORT_ROOT)\lib"

!ELSEIF  "$(CFG)" == "C++ToolsTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Output\VC6\Debug"
# PROP Intermediate_Dir "Output\VC6\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(STLPORT_ROOT)\lib"

!ELSEIF  "$(CFG)" == "C++ToolsTest - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "C++ToolsTest___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "C++ToolsTest___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Output\VC6\UDebug"
# PROP Intermediate_Dir "Output\VC6\UDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(STLPORT_ROOT)\lib"

!ELSEIF  "$(CFG)" == "C++ToolsTest - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "C++ToolsTest___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "C++ToolsTest___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Output\VC6\URelease"
# PROP Intermediate_Dir "Output\VC6\URelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "NDEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"$(STLPORT_ROOT)\lib"

!ELSEIF  "$(CFG)" == "C++ToolsTest - Win32 Unicode Debug No PreComp"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "C__ToolsTest___Win32_Unicode_Debug_No_PreComp"
# PROP BASE Intermediate_Dir "C__ToolsTest___Win32_Unicode_Debug_No_PreComp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Output\VC6\UDebugNoPreComp"
# PROP Intermediate_Dir "Output\VC6\UDebugNoPreComp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(STLPORT_ROOT)\lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(STLPORT_ROOT)\lib"

!ENDIF 

# Begin Target

# Name "C++ToolsTest - Win32 Release"
# Name "C++ToolsTest - Win32 Debug"
# Name "C++ToolsTest - Win32 Unicode Debug"
# Name "C++ToolsTest - Win32 Unicode Release"
# Name "C++ToolsTest - Win32 Unicode Debug No PreComp"
# Begin Group "Test Harness"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Test.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExpandableBufferTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpandableBufferTest.h
# End Source File
# Begin Source File

SOURCE=.\NodeListTest.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeListTest.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceCountedSmartPointerTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ReferenceCountedSmartPointerTest.h
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Docs\Groups.txt
# End Source File
# End Group
# End Target
# End Project
