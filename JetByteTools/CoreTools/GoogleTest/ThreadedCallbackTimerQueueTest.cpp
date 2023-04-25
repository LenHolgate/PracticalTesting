///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2023 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"

#include "JetByteTools/ThirdPartyCode/GoogleTest/gtest.h"
#include "JetByteTools/ThirdPartyCode/GoogleTest/gmock.h"

#include "JetByteTools/CoreTools/CallbackTimerQueue.h"

#include "JetByteTools/CoreTools/Mock/MockTickCount64Provider.h"
#include "JetByteTools/CoreTools/Mock/MockTimerQueue.h"
#include "JetByteTools/CoreTools/Mock/MockThreadedCallbackTimerQueueMonitor.h"
#include "JetByteTools/CoreTools/Mock/TestThreadedCallbackTimerQueue.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::Mock::CMockTickCount64Provider;
using JetByteTools::Core::Mock::CMockThreadedCallbackTimerQueueMonitor;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {


TEST(ThreadedCallbackTimerQueue, TestConstruct)
{
   {
      CThreadedCallbackTimerQueue timerQueue;
   }

   CMockThreadedCallbackTimerQueueMonitor monitor;

   {
      CThreadedCallbackTimerQueue timerQueue(monitor);
   }

   monitor.CheckNoResults();

   CMockTickCount64Provider tickProvider;

   {
      CThreadedCallbackTimerQueue timerQueue(tickProvider);
   }

   tickProvider.CheckNoResults();

   {
      CThreadedCallbackTimerQueue timerQueue(monitor, tickProvider);
   }

   monitor.CheckNoResults();

   tickProvider.CheckNoResults();
}

class CGMockTickCount64Provider : public IProvideTickCount64
{
   public :

   MOCK_METHOD(ULONGLONG, GetTickCount64, (), (const override));
};

TEST(ThreadedCallbackTimerQueue, TestConstructWithGMocks)
{
   CGMockTickCount64Provider tickProvider;

   EXPECT_CALL(tickProvider, GetTickCount64()).Times(0);

   {
      CThreadedCallbackTimerQueue timerQueue(tickProvider);
   }

   //tickProvider.CheckNoResults();
}

#ifndef SHORT_TIME_NON_ZERO
#define SHORT_TIME_NON_ZERO 100
#endif

TEST(ThreadedCallbackTimerQueue, TestBeginShutdown)
{
   {
      CThreadedCallbackTimerQueue timerQueue;

      timerQueue.BeginShutdown();

      EXPECT_TRUE(timerQueue.WaitForShutdownToComplete(SHORT_TIME_NON_ZERO));
   }
}



///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools


///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueueTest.cpp
///////////////////////////////////////////////////////////////////////////////
