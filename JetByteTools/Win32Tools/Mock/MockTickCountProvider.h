#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_MOCK_TICK_COUNT_PROVIDER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MOCK_TICK_COUNT_PROVIDER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: MockTickCountProvider.h
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

#include "JetByteTools\TestTools\TestLog.h"

#include "..\IProvideTickCount.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTickCountProvider
///////////////////////////////////////////////////////////////////////////////

/// A mock object that implements IProvideTickCount and provides a tick count
/// that can be controlled from within the test.
/// See <a href="http://www.lenholgate.com/archives/000311.html">here</a> for
/// more details.
/// \ingroup Win32ToolsMocks

class CMockTickCountProvider :
   public IProvideTickCount,
   public JetByteTools::Test::CTestLog
{
   public :

      CMockTickCountProvider();

      /// Construct a tick count provider with the specified tick count. All
      /// calls to GetTickCount() will return this value.

      explicit CMockTickCountProvider(
         const Milliseconds tickCount);

      bool logMessages;

      bool logTickCount;

      /// Change the tick count that the provider will return from calls to
      /// GetTickCount().

      void SetTickCount(
         const Milliseconds tickCount);

      // Implement IProvideTickCount

      virtual Milliseconds GetTickCount() const;

   private :

      volatile Milliseconds m_tickCount;

      // No copies do not implement
      CMockTickCountProvider(const CMockTickCountProvider &rhs);
      CMockTickCountProvider &operator=(const CMockTickCountProvider &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MOCK_TICK_COUNT_PROVIDER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTickCountProvider.h
///////////////////////////////////////////////////////////////////////////////
