#pragma once
#ifndef JETBYTE_TOOLS_WIN32_MOCK_TICK_COUNT_64_PROVIDER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MOCK_TICK_COUNT_64_PROVIDER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: MockTickCount64Provider.h
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

#include "JetByteTools\Admin\Types.h"

#include "JetByteTools\TestTools\TestLog.h"

#include "JetByteTools\Win32Tools\IProvideTickCount64.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTickCount64Provider
///////////////////////////////////////////////////////////////////////////////

/// A mock object that implements IProvideTickCount64 and provides a tick count
/// that can be controlled from within the test.
/// See <a href="http://www.lenholgate.com/archives/000311.html">here</a> for
/// more details.
/// \ingroup Win32ToolsMocks

class CMockTickCount64Provider :
   public IProvideTickCount64,
   public JetByteTools::Test::CTestLog
{
   public :

      CMockTickCount64Provider();

      /// Construct a tick count provider with the specified tick count. All
      /// calls to GetTickCount54() will return this value.

      explicit CMockTickCount64Provider(
         const ULONGLONG tickCount);

      bool logMessages;

      bool logTickCount;

      /// Change the tick count that the provider will return from calls to
      /// GetTickCount64().

      void SetTickCount(
         const ULONGLONG tickCount);

      // Implement IProvideTickCount64

      ULONGLONG GetTickCount64() const override;

   private :

      volatile ULONGLONG m_tickCount;

      // No copies do not implement
      CMockTickCount64Provider(const CMockTickCount64Provider &rhs);
      CMockTickCount64Provider &operator=(const CMockTickCount64Provider &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MOCK_TICK_COUNT_64_PROVIDER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTickCount64Provider.h
///////////////////////////////////////////////////////////////////////////////
