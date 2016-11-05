#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_TSTRING__
#define JETBYTE_TOOLS_WIN32_TSTRING__
///////////////////////////////////////////////////////////////////////////////
// File: tstring.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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

#include <string>
#include <tchar.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

/// An STL string that is either wide or narrow depending on the usual Windows 
/// development _UNICODE #define.
/// \ingroup Unicode

#ifdef _UNICODE
typedef std::wstring _tstring;
#else
typedef std::string _tstring;
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_WIN32_TSTRING__

///////////////////////////////////////////////////////////////////////////////
// End of file: tstring.h
///////////////////////////////////////////////////////////////////////////////

