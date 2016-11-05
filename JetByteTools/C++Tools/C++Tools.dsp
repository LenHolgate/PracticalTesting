# Microsoft Developer Studio Project File - Name="C++Tools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=C++Tools - Win32 Unicode Debug No PreComp
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "C++Tools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "C++Tools.mak" CFG="C++Tools - Win32 Unicode Debug No PreComp"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "C++Tools - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "C++Tools - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "C++Tools - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "C++Tools - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "C++Tools - Win32 Unicode Debug No PreComp" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "C++Tools - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /Zi /O2 /I "..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "C++Tools - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /GX /Zi /Od /I "..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "C++Tools - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "C++Tools___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "C++Tools___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Output\VC6\UDebug"
# PROP Intermediate_Dir "Output\VC6\UDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /GX /Zi /Od /I "..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WIN32" /D "_MBCS" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "C++Tools - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "C++Tools___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "C++Tools___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Output\VC6\URelease"
# PROP Intermediate_Dir "Output\VC6\URelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /Zi /O2 /I "..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "NDEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WIN32" /D "_MBCS" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "C++Tools - Win32 Unicode Debug No PreComp"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "C__Tools___Win32_Unicode_Debug_No_PreComp"
# PROP BASE Intermediate_Dir "C__Tools___Win32_Unicode_Debug_No_PreComp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Output\VC6\UDebugNoPreComp"
# PROP Intermediate_Dir "Output\VC6\UDebugNoPreComp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /GX /Zi /Od /I "..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WIN32" /D "_MBCS" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /GX /Zi /Od /I "..\..\\" /I "$(STLPORT_ROOT)\stlport" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /D "WIN32" /D "_MBCS" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "C++Tools - Win32 Release"
# Name "C++Tools - Win32 Debug"
# Name "C++Tools - Win32 Unicode Debug"
# Name "C++Tools - Win32 Unicode Release"
# Name "C++Tools - Win32 Unicode Debug No PreComp"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DebugTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DebugTrace.h
# End Source File
# Begin Source File

SOURCE=.\ExpandableBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ILogMessages.h
# End Source File
# Begin Source File

SOURCE=.\LinkedClass.h
# End Source File
# Begin Source File

SOURCE=.\MessageLog.h
# End Source File
# Begin Source File

SOURCE=.\NodeList.h
# End Source File
# Begin Source File

SOURCE=.\NullMessageLog.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceCounted.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceCountedSmartPointer.h
# End Source File
# Begin Source File

SOURCE=.\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\TypeSafeTypedef.h
# End Source File
# End Group
# Begin Group "PrecompiledHeader"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
