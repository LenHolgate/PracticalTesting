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

class COSVersionInfo : public OSVERSIONINFO 
{
   public:
      
      /// Calls ::GetVersionEx() on the structure that the class wraps and so
      /// fully constructs a usable OSVERSIONINFO structure.

      COSVersionInfo() 
      { 
         dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
         ::GetVersionEx(this); 
      }

      bool IsVistaOrLater() const
      {
         return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion >= 6); 
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

