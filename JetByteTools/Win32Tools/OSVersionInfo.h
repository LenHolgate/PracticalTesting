#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_OS_VERSION_INFO_INCLUDED__
#define JETBYTE_TOOLS_WIN32_OS_VERSION_INFO_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: OSVersionInfo.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include <wtypes.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// COSVersionInfo
///////////////////////////////////////////////////////////////////////////////

/// A simple class that wraps, and initialises, an OSVERSIONINFO structure.
/// \ingroup Win32

class COSVersionInfo : private OSVERSIONINFOEX
{
   public:

      /// Calls ::GetVersionEx() on the structure that the class wraps and so
      /// fully constructs a usable OSVERSIONINFO structure.

#pragma warning(push)
#pragma warning(disable: 4996)

      COSVersionInfo()
      {
         dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
         ::GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(this));
      }

#pragma warning(pop)

      bool IsWindows2000OrLater() const
      {
         return dwMajorVersion >= 5;
      }

      bool IsVistaOrLater() const
      {
         return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion >= 6);
      }

      bool IsServer2008OrLater() const
      {
         return (wProductType != VER_NT_WORKSTATION) && 
                (dwPlatformId == VER_PLATFORM_WIN32_NT) &&
                (dwMajorVersion >= 6) &&
                (dwMinorVersion >= 1);
      }

      bool IsWindows7OrLater() const
      {
         return (dwPlatformId == VER_PLATFORM_WIN32_NT) && ((dwMajorVersion > 6) || (dwMajorVersion == 6 && dwMinorVersion >= 1));
      }

      bool IsWindows8OrLater() const
      {
         return (dwPlatformId == VER_PLATFORM_WIN32_NT) && ((dwMajorVersion > 6) || (dwMajorVersion == 6 && dwMinorVersion >= 2));
      }

      // Deliberately broken for Windows 8.1. and 10, they just return Windows 8 values UNLESS
      // the exe has been specifically manifested to be targetting 8.1. or 10.
      // IDIOTS!

      bool IsWindows81OrLater() const
      {
         // Should probably switch to using GetOsVersion for the main query that this class does...

         RTL_OSVERSIONINFOEXW versionInfo;

         if (GetOsVersion(&versionInfo))
         {
            return (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && ((versionInfo.dwMajorVersion > 6) || (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion >= 3));
         }

         return false;
      }


      bool IsWindows10OrLater() const
      {
         // Should probably switch to using GetOsVersion for the main query that this class does...

         RTL_OSVERSIONINFOEXW versionInfo;

         if (GetOsVersion(&versionInfo))
         {
            return (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 10);
         }

         return false;
      }

   private :

   // From http://www.codeproject.com/Articles/678606/Part-Overcoming-Windows-s-deprecation-of-GetVe?msg=5080848#xx5080848xx

   // RTL_OSVERSIONINFOEXW is defined in winnt.h
   BOOL GetOsVersion(RTL_OSVERSIONINFOEXW* pk_OsVer) const
   {
      typedef LONG(WINAPI* tRtlGetVersion)(RTL_OSVERSIONINFOEXW*);

      memset(pk_OsVer, 0, sizeof(RTL_OSVERSIONINFOEXW));
      pk_OsVer->dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);

      HMODULE h_NtDll = GetModuleHandleW(L"ntdll.dll");
      tRtlGetVersion f_RtlGetVersion = (tRtlGetVersion)GetProcAddress(h_NtDll, "RtlGetVersion");

      if (!f_RtlGetVersion)
         return FALSE; // This will never happen (all processes load ntdll.dll)

      LONG Status = f_RtlGetVersion(pk_OsVer);
      return Status == 0; // STATUS_SUCCESS;
   }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_OS_VERSION_INFO_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: OSVersionInfo.h
///////////////////////////////////////////////////////////////////////////////

