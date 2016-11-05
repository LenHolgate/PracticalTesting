#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_USING_MFC_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_USING_MFC_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: UsingMFC.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using the software. In no event will JetByte Limited be liable for any 
// lost revenue, profit or data, or for direct, indirect, special, 
// consequential, incidental or punitive damages, however caused and regardless 
// of the theory of liability, arising out of the use of or inability to use 
// software, even if JetByte Limited has been advised of the possibility of 
// such damages.
//
///////////////////////////////////////////////////////////////////////////////

/// \file UsingMFC.h
/// If you're building with the JetByte Tools libraries and mixing the code
/// with MFC then you may have problems. 
/// See <a href="http://support.microsoft.com/kb/148652">here</a> for more 
/// details on the likely problems. 
/// Including this file is likely to fix those problems as it fixes up the
/// library link order. You only have to include this file once, usually in 
/// the 'main()' .cpp file.
/// NOTE THAT YOU MUST INCLUDE THIS FILE BEFORE Admin.h
/// \ingroup Admin

#ifndef _AFX_NOFORCE_LIBS

/////////////////////////////////////////////////////////////////////////////
// Win32 libraries

#ifndef _AFXDLL
	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "nafxcwd.lib")
		#else
			#pragma comment(lib, "nafxcw.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "uafxcwd.lib")
		#else
			#pragma comment(lib, "uafxcw.lib")
		#endif
	#endif
#else
   #if (_MSC_VER == 1200)
      #ifndef _UNICODE
	   	#ifdef _DEBUG
		   	#pragma comment(lib, "mfc42d.lib")
			   #pragma comment(lib, "mfcs42d.lib")
   		#else
	   		#pragma comment(lib, "mfc42.lib")
		   	#pragma comment(lib, "mfcs42.lib")
   		#endif
	   #else
		   #ifdef _DEBUG
   			#pragma comment(lib, "mfc42ud.lib")
	   		#pragma comment(lib, "mfcs42ud.lib")
		   #else
   			#pragma comment(lib, "mfc42u.lib")
	   		#pragma comment(lib, "mfcs42u.lib")
		   #endif
   	#endif
   #elif (_MSC_VER == 1300)
	   #ifndef _UNICODE
		   #ifdef _DEBUG
			   #pragma comment(lib, "mfc70d.lib")
			   #pragma comment(lib, "mfcs70d.lib")
		   #else
			   #pragma comment(lib, "mfc70.lib")
			   #pragma comment(lib, "mfcs70.lib")
		   #endif
	   #else
		   #ifdef _DEBUG
			   #pragma comment(lib, "mfc70ud.lib")
			   #pragma comment(lib, "mfcs70ud.lib")
		   #else
			   #pragma comment(lib, "mfc70u.lib")
			   #pragma comment(lib, "mfcs70u.lib")
		   #endif
	   #endif
   #elif (_MSC_VER == 1310)
	   #ifndef _UNICODE
		   #ifdef _DEBUG
			   #pragma comment(lib, "mfc71d.lib")
			   #pragma comment(lib, "mfcs71d.lib")
		   #else
			   #pragma comment(lib, "mfc71.lib")
			   #pragma comment(lib, "mfcs71.lib")
		   #endif
	   #else
		   #ifdef _DEBUG
			   #pragma comment(lib, "mfc71ud.lib")
			   #pragma comment(lib, "mfcs71ud.lib")
		   #else
			   #pragma comment(lib, "mfc71u.lib")
			   #pragma comment(lib, "mfcs71u.lib")
		   #endif
	   #endif
   #elif (_MSC_VER == 1400)
	   #ifndef _UNICODE
		   #ifdef _DEBUG
			   #pragma comment(lib, "mfc80d.lib")
			   #pragma comment(lib, "mfcs80d.lib")
		   #else
			   #pragma comment(lib, "mfc80.lib")
			   #pragma comment(lib, "mfcs80.lib")
		   #endif
	   #else
		   #ifdef _DEBUG
			   #pragma comment(lib, "mfc80ud.lib")
			   #pragma comment(lib, "mfcs80ud.lib")
		   #else
			   #pragma comment(lib, "mfc80u.lib")
			   #pragma comment(lib, "mfcs80u.lib")
		   #endif
	   #endif
   #elif  (_MSC_VER == 1500)
   	#ifndef _UNICODE
	   	#ifdef _DEBUG
   			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "d.lib")
	   		#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "d.lib")
		   #else
   			#pragma comment(lib, "mfc" _MFC_FILENAME_VER ".lib")
	   		#pragma comment(lib, "mfcs" _MFC_FILENAME_VER ".lib")
		   #endif
   	#else
	   	#ifdef _DEBUG
		   	#pragma comment(lib, "mfc" _MFC_FILENAME_VER "ud.lib")
   			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "ud.lib")
	   	#else
		   	#pragma comment(lib, "mfc" _MFC_FILENAME_VER "u.lib")
   			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "u.lib")
         #endif
      #endif
   #endif
#endif

#ifdef _DLL
	#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
		#pragma comment(lib, "msvcrtd.lib")
	#else
		#pragma comment(lib, "msvcrt.lib")
	#endif
#else
#ifdef _MT
	#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
		#pragma comment(lib, "libcmtd.lib")
	#else
		#pragma comment(lib, "libcmt.lib")
	#endif
#else
	#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
		#pragma comment(lib, "libcd.lib")
	#else
		#pragma comment(lib, "libc.lib")
	#endif
#endif
#endif

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")

// force inclusion of NOLIB.OBJ for /disallowlib directives
#pragma comment(linker, "/include:__afxForceEXCLUDE")

// force inclusion of DLLMODUL.OBJ for _USRDLL
#ifdef _USRDLL
#pragma comment(linker, "/include:__afxForceUSRDLL")
#endif

// force inclusion of STDAFX.OBJ for precompiled types
#ifdef _AFXDLL
#pragma comment(linker, "/include:__afxForceSTDAFX")
#endif

#endif //!_AFX_NOFORCE_LIBS



#endif // JETBYTE_TOOLS_ADMIN_USING_MFC_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: UsingMFC.h
///////////////////////////////////////////////////////////////////////////////
