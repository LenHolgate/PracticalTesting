#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_I_PROVIDE_TICK_COUNT_64_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_PROVIDE_TICK_COUNT_64_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IProvideTickCount64.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
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
// IProvideTickCount64
///////////////////////////////////////////////////////////////////////////////

/// An interface that provides access to the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms724411(VS.85).aspx">GetTickCount64()</a>
/// call. Code can access this service via an instance of IProvideTickCount64 so
/// that the provider can be replaced (usually for testing purposes).
/// See <a href= "http://www.lenholgate.com/archives/000311.html">here</a>
/// for more details.
/// \ingroup Timers
/// \ingroup Interfaces
/// \ingroup ProtectedDestructors

class IProvideTickCount64
{
   public :

      /// Returns the current tick count.

      virtual ULONGLONG GetTickCount64() const = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      ~IProvideTickCount64() {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_PROVIDE_TICK_COUNT_64_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IProvideTickCount64.h
///////////////////////////////////////////////////////////////////////////////
