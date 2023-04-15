#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: MockTickCountProvider.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2004 JetByte Limited.
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

#include "JetByteTools/TestTools/TestLog.h"

#include "JetByteTools/CoreTools/Atomic.h"
#include "JetByteTools/CoreTools/IProvideTickCount.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
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
         Milliseconds tickCount);

      CMockTickCountProvider(
         const CMockTickCountProvider &rhs) = delete;

      CMockTickCountProvider &operator=(
         const CMockTickCountProvider &rhs) = delete;

      bool logMessages;

      bool logTickCount;

      /// Change the tick count that the provider will return from calls to
      /// GetTickCount().

      void SetTickCount(
         Milliseconds tickCount);

      // Implement IProvideTickCount

      Milliseconds GetTickCount() const override;

   private :

      TAtomic<Milliseconds> m_tickCount;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTickCountProvider.h
///////////////////////////////////////////////////////////////////////////////
