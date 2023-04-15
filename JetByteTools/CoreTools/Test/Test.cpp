///////////////////////////////////////////////////////////////////////////////
// File: Test.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/ToBool.h"
#include "JetByteTools/CoreTools/GlobalErrorHandler.h"

#include "JetByteTools/TestTools/TestMonitor.h"
#include "JetByteTools/TestTools/TestException.h"

#include "CallbackTimerQueueTest.h"
#include "CallbackTimerWheelTest.h"
#include "IntrusiveMultiMapTest.h"
#include "IntrusiveRedBlackTreeTest.h"
#include "ThreadedCallbackTimerQueueTest.h"

#include "JetByteTools/CoreTools/DebugTrace.h"
#include "JetByteTools/CoreTools/Exception.h"
#include "JetByteTools/CoreTools/StringConverter.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::_tstring;
using JetByteTools::Core::OutputEx;
using JetByteTools::Core::CDebugTrace;
using JetByteTools::Core::CException;
using JetByteTools::Core::CStringConverter;
using JetByteTools::Core::ToBool;
using JetByteTools::Core::CGlobalErrorHandler;

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestMonitor;

using namespace JetByteTools::Core::Test;

///////////////////////////////////////////////////////////////////////////////
// Program Entry Point
///////////////////////////////////////////////////////////////////////////////

int main(int /*argc*/, char ** /*argv*/)
{
   CGlobalErrorHandler handler;

   CDebugTrace::Instance().SetLogName("CoreToolsTest.log");

   bool ok = false;

   try
   {
      #ifdef _DEBUG
      const bool includePerformanceTests = false;
      #else
      const bool includePerformanceTests = true;
      #endif

      const bool stopOnFailure = ToBool(IsDebuggerPresent());

      CTestMonitor monitor(_T("Core Tools"), includePerformanceTests, stopOnFailure);

      CIntrusiveRedBlackTreeTest::TestAll(monitor);
      CIntrusiveMultiMapTest::TestAll(monitor);

      CCallbackTimerWheelTest::TestAll(monitor);
      CCallbackTimerQueueTest::TestAll(monitor);
      CThreadedCallbackTimerQueueTest::TestAll(monitor);

      size_t expectedTests = 124;

      ok = monitor.Report(expectedTests);
   }
   catch(const CTestException &e)
   {
      OutputEx(_T("Test Exception: ") + e.GetDetails());

      ok = false;
   }
   catch(const CException &e)
   {
      OutputEx(_T("Exception: ") + e.GetDetails());

      ok = false;
   }
   catch(const char *p)
   {
      OutputEx(_T("Exception: ") + CStringConverter::AtoT(p));
   }
   JETBYTE_TESTS_CATCH_ALL_IF_ENABLED
   {
      OutputEx(_T("Unexpected exception"));

      ok = false;
   }

   OutputEx(_tstring(_T("Test ")) + (ok ? _T("Passed") : _T("Failed")));

   return ok ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////
// End of file: Test.cpp
///////////////////////////////////////////////////////////////////////////////

