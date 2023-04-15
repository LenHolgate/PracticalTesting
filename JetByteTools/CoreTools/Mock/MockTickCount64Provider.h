#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: MockTickCount64Provider.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/CoreTools/Types.h"
#include "JetByteTools/CoreTools/IProvideTickCount64.h"

#include "JetByteTools/TestTools/TestLog.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
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
         ULONGLONG tickCount);

      CMockTickCount64Provider(
         const CMockTickCount64Provider &rhs) = delete;

      CMockTickCount64Provider &operator=(
         const CMockTickCount64Provider &rhs) = delete;

      bool logMessages;

      bool logTickCount;

      /// Change the tick count that the provider will return from calls to
      /// GetTickCount64().

      void SetTickCount(
         ULONGLONG tickCount);

      // Implement IProvideTickCount64

      ULONGLONG GetTickCount64() const override;

   private :

      volatile ULONGLONG m_tickCount;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTickCount64Provider.h
///////////////////////////////////////////////////////////////////////////////
