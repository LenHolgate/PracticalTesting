#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TestCallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
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

#include "JetByteTools/CoreTools/CallbackTimerWheel.h"

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools
{
   namespace Core
   {
      class IProvideTickCount;
      class IMonitorCallbackTimerQueue;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestCallbackTimerWheel
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Win32ToolsMocks

class CTestCallbackTimerWheel : public CCallbackTimerWheel
{
   public :

      CTestCallbackTimerWheel();

      explicit CTestCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor);

      /// Create a timer queue that uses the provdided instance of
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system.

      explicit CTestCallbackTimerWheel(
         const IProvideTickCount &tickProvider);

      /// Create a timer queue that uses the provdided instance of
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system. Monitor it with the supplied monitor.

      CTestCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         const IProvideTickCount &tickProvider);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestCallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
