#pragma once
#ifndef JETBYTE_TOOLS_WIN32_TICK_COUNT_64_PROVIDER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TICK_COUNT_64_PROVIDER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TickCount64Provider.h
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

#include <WinBase.h>             // For GetTickCount()

#include "IProvideTickCount64.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CTickCount64Provider
///////////////////////////////////////////////////////////////////////////////

/// A class that implements IProvideTickCount64 and returns the tick count
/// directly from a call to the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms724411(VS.85).aspx">GetTickCount64()</a>
/// function.
/// \ingroup Timers

class CTickCount64Provider: public IProvideTickCount64
{
   public :

      CTickCount64Provider() = default;

      ULONGLONG GetTickCount64() const override
      {
         return ::GetTickCount64();
      }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TICK_COUNT_64_PROVIDER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TickCount64Provider.h
///////////////////////////////////////////////////////////////////////////////
