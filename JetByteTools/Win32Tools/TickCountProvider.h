#pragma once
#ifndef JETBYTE_TOOLS_WIN32_TICK_COUNT_PROVIDER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_TICK_COUNT_PROVIDER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TickCountProvider.h
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

#include <WinBase.h>             // For GetTickCount()

#include "IProvideTickCount.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CTickCountProvider
///////////////////////////////////////////////////////////////////////////////

/// A class that implements IProvideTickCount and returns the tick count
/// directly from a call to the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms724408.aspx">GetTickCount()</a>
/// function.
/// See <a href="http://www.lenholgate.com/archives/000311.html">here</a> for
/// more details.
/// \ingroup Timers

class CTickCountProvider: public IProvideTickCount
{
   public :

      CTickCountProvider() = default;

      Milliseconds GetTickCount() const override
      {
         return ::GetTickCount();
      }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_TICK_COUNT_PROVIDER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TickCountProvider.h
///////////////////////////////////////////////////////////////////////////////
