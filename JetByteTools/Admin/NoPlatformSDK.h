#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_NO_PLATFORM_SDK_INCLUDED__
#define JETBYTE_TOOLS_WIN32_NO_PLATFORM_SDK_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: NoPlatformSDK.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
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

#if (_MSC_VER < 1300)   // can't find std::min/max on VC6 stl for some reason...

#ifndef _UINTPTR_T_DEFINED
#ifdef  _WIN64
typedef unsigned __int64    uintptr_t;
#else
typedef /*_W64*/ unsigned int   uintptr_t;
#endif
#define _UINTPTR_T_DEFINED
#endif

#endif


/// \file NoPlatformSDK.h
/// It was, at some time, possible to build some of the code in VC6 without a 
/// copy of the Platform SDK installed. This becomes less and less likely to 
/// work as time goes on. Building with VC6 and without a copy of the Platform
/// SDK is no longer a supported option, but it may work.
/// See <a href="http://en.wikipedia.org/wiki/Microsoft_Platform_SDK">here</a>
/// for more details on how to get the latest copy of the Platform SDK.
/// \ingroup Admin

#if (JETBYTE_NO_PLATFORM_SDK == 1)
#if (_MSC_VER < 1300)         // Only need this rubbish for VC 6

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#if (JETBYTE_USE_STL_PORT == 1)
#undef _STLP_NEW_PLATFORM_SDK
#endif

inline PVOID InterlockedExchangePointer(PVOID volatile Target, PVOID Value)
{
   return reinterpret_cast<PVOID>(::InterlockedExchange(
      reinterpret_cast<LPLONG volatile>(Target), 
      reinterpret_cast<LONG>(Value)));
}

inline PVOID InterlockedCompareExchange(PVOID volatile Destination, LONG Exchange, LONG Comperand) 
{
   return reinterpret_cast<PVOID>(::InterlockedCompareExchange(
      reinterpret_cast<LPLONG volatile>(Destination), 
      Exchange, 
      Comperand));
}

#ifndef ULONG_PTR
#ifndef MAXULONG_PTR
#define ULONG_PTR    DWORD
#endif //MAXULONG_PTR
#endif //ULONG_PTR

#ifndef PCTSTR
#define PCTSTR LPCTSTR
#endif //PCTSTR

#ifndef INVALID_SET_FILE_POINTER 
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif // INVALID_SET_FILE_POINTER 

//
// Add Windows flavor DWORD_PTR types
//

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

#endif // _MSC_VER
#endif // JETBYTE_NO_PLATFORM_SDK

#endif // JETBYTE_TOOLS_WIN32_NO_PLATFORM_SDK_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: NoPlatformSDK.h
///////////////////////////////////////////////////////////////////////////////
