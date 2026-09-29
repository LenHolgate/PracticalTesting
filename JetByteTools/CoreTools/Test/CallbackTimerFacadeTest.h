#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerFacadeTest.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2026 JetByte Limited.
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

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools
{
   namespace Test
   {
      class CTestMonitor;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerFacadeTest
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerFacadeTest
{
   public :

      static void TestAll(
         JetByteTools::Test::CTestMonitor &monitor);

      static void TestConstruct();
      static void TestHandleTimeoutsNoTimeouts();
      static void TestHandleTimeouts();
      static void TestBeginShutdownNoTimeouts();
      static void TestWaitForShutdownToCompleteNoTimeouts();
      static void TestWaitForShutdownToCompleteAfterBeginShutdownNoTimeouts();
      static void TestBeginShutdownTimersHandledInBeginShutdown();
      static void TestWaitForShutdownToCompleteTimersHandledInBeginShutdown();
      static void TestWaitForShutdownToCompleteAfterBeginShutdownTimersHandledInBeginShutdown();
      static void TestBeginShutdownTimersHandledInWaitForShutdownToComplete();
      static void TestWaitForShutdownToCompleteTimersHandledInWaitForShutdownToComplete();
      static void TestWaitForShutdownToCompleteAfterBeginShutdownTimersHandledInWaitForShutdownToComplete();
      static void TestWaitForShutdownDuringDestructionNoTimeouts();
      static void TestWaitForShutdownDuringDestruction();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerFacadeTest.h
///////////////////////////////////////////////////////////////////////////////
