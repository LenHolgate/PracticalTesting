#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: RunTest.h
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

#pragma warning(disable: 4127)   // conditional expression is constant

#include "TestException.h"
#include "TestMonitor.h"

#include "JetByteTools/CoreTools/Exception.h"
#include "JetByteTools/CoreTools/StringConverter.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// RUN_TEST()
// Usage: RUN_TEST(TestFunction);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TEST(f)                                                           \
do                                                                            \
{                                                                             \
   const JetByteTools::Core::_tstring functionName(_T(#f) _T("()"));          \
                                                                              \
   try                                                                        \
   {                                                                          \
      JetByteTools::Core::Output(functionName + _T(" - start"));              \
                                                                              \
      f();                                                                    \
                                                                              \
      JetByteTools::Core::Output(functionName + _T(" - stop"));               \
   }                                                                          \
   catch (JetByteTools::Test::CTestException &e)                              \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: CTestException - ") + e.GetWhere() +                \
         _T(" - ") + e.GetWhat());                                            \
   }                                                                          \
   catch (JetByteTools::Core::CException &e)                                  \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: CException - ") + e.GetWhere() +                    \
         _T(" - ") + e.GetWhat());                                            \
   }                                                                          \
   catch (std::exception &e)                                                  \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: std::exception - ") +                               \
         JetByteTools::Core::CStringConverter::AtoT(e.what()));               \
   }                                                                          \
   catch (const char *pE)                                                     \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: Exception - ") +                                    \
         JetByteTools::Core::CStringConverter::AtoT(pE));                     \
   }                                                                          \
   catch (const wchar_t *pE)                                                  \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: Exception - ") +                                    \
         JetByteTools::Core::CStringConverter::WtoT(pE));                     \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: Unexpected exception"));                            \
   }                                                                          \
}while(0)

#define RUN_PERFORMANCE_TEST(f, r)                                            \
do                                                                            \
{                                                                             \
   const JetByteTools::Core::_tstring functionName(_T(#f) _T("()"));          \
                                                                              \
   if (r)                                                                     \
   {                                                                          \
      RUN_TEST(f);                                                            \
   }                                                                          \
   else                                                                       \
   {                                                                          \
      throw JetByteTools::Test::CTestSkippedException(                        \
         functionName,                                                        \
         _T("Skipped performance test"));                                     \
   }                                                                          \
                                                                              \
}while(0)

///////////////////////////////////////////////////////////////////////////////
// RUN_TIMED_TEST_EX()
// Usage: RUN_TIMED_TEST_EX(Monitor, Class, TestFunction, timeoutMillis);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TIMED_TEST_EX(m, c, f, t)                                         \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      m.StartTest(_T(#c), _T(#f), t);                                         \
                                                                              \
      c::f();                                                                 \
                                                                              \
      m.TestComplete();                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

///////////////////////////////////////////////////////////////////////////////
// RUN_TEST_EX()
// Usage: RUN_TEST_EX(Monitor, Class, TestFunction);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TEST_EX(m, c, f) RUN_TIMED_TEST_EX(m, c, f, 0)

///////////////////////////////////////////////////////////////////////////////
// RUN_TIMED_PERFORMANCE_TEST_EX()
// Usage: RUN_TIMED_PERFORMANCE_TEST_EX(Monitor, Class, TestFunction, timeoutMillis);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TIMED_PERFORMANCE_TEST_EX(m, c, f, t)                             \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      if (m.StartPerformanceTest(_T(#c), _T(#f), t))                          \
      {                                                                       \
         c::f();                                                              \
                                                                              \
         m.TestComplete();                                                    \
      }                                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

///////////////////////////////////////////////////////////////////////////////
// RUN_PERFORMANCE_TEST_EX()
// Usage: RUN_TEST_EX(Monitor, Class, TestFunction);
///////////////////////////////////////////////////////////////////////////////

#define RUN_PERFORMANCE_TEST_EX(m, c, f)  RUN_TIMED_PERFORMANCE_TEST_EX(m, c, f, 0)

///////////////////////////////////////////////////////////////////////////////
// RUN_TEMPLATE_TEST_EX()
// Note that T should be a _tstring
// Usage: RUN_TEMPLATE_TEST_EX(Monitor, Class, T, TestFunction);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TEMPLATE_TEST_EX(m, c, t, f)                                      \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      m.StartTest(t, _T(#f));                                                 \
                                                                              \
      c::f();                                                                 \
                                                                              \
      m.TestComplete();                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

#define RUN_TEMPLATE_PERFORMANCE_TEST_EX(m, c, t, f)                          \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      const JetByteTools::Core::_tstring t_(t);                               \
                                                                              \
      if (m.StartPerformanceTest(t, _T(#f)))                                  \
      {                                                                       \
         c::f();                                                              \
                                                                              \
         m.TestComplete();                                                    \
      }                                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

///////////////////////////////////////////////////////////////////////////////
// RUN_TEMPLATE_TEST_EX_2()
// Note that T should be a _tstring
// Usage: RUN_TEMPLATE_TEST_EX_2(Monitor, Class, P1, P2, T, TestFunction);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TEMPLATE_TEST_EX_2(m, c, p1, p2, t, f)                            \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      m.StartTest(t, _T(#f));                                                 \
                                                                              \
      c<p1,p2>::f();                                                          \
                                                                              \
      m.TestComplete();                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

#define RUN_TEMPLATE_PERFORMANCE_TEST_EX_2(m, c, p1, p2, t, f)                \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      const JetByteTools::Core::_tstring t_(t);                               \
                                                                              \
      if (m.StartPerformanceTest(t, _T(#f)))                                  \
      {                                                                       \
         c<p1,p2>::f();                                                       \
                                                                              \
         m.TestComplete();                                                    \
      }                                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

///////////////////////////////////////////////////////////////////////////////
// RUN_TEMPLATE_TEST_EX_3()
// Note that T should be a _tstring
// Usage: RUN_TEMPLATE_TEST_EX_3(Monitor, Class, P1, P2, P3, T, TestFunction);
///////////////////////////////////////////////////////////////////////////////

#define RUN_TEMPLATE_TEST_EX_3(m, c, p1, p2, p3, t, f)                        \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      m.StartTest(t, _T(#f));                                                 \
                                                                              \
      c<p1,p2,p3>::f();                                                       \
                                                                              \
      m.TestComplete();                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

#define RUN_TEMPLATE_PERFORMANCE_TEST_EX_3(m, c, p1, p2, p3, t, f)            \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      const JetByteTools::Core::_tstring t_(t);                               \
                                                                              \
      if (m.StartPerformanceTest(t, _T(#f)))                                  \
      {                                                                       \
         c<p1,p2,p3>::f();                                                    \
                                                                              \
         m.TestComplete();                                                    \
      }                                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

#define RUN_TIMED_TEMPLATE_PERFORMANCE_TEST_EX_3(m, c, p1, p2, p3, t, f, ms)  \
do                                                                            \
{                                                                             \
   try                                                                        \
   {                                                                          \
      const JetByteTools::Core::_tstring t_(t);                               \
                                                                              \
      if (m.StartPerformanceTest(t, _T(#f), ms))                              \
      {                                                                       \
         c<p1,p2,p3>::f();                                                    \
                                                                              \
         m.TestComplete();                                                    \
      }                                                                       \
   }                                                                          \
   catch (...)                                                                \
   {                                                                          \
      m.TestException();                                                      \
   }                                                                          \
}while(0)

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: RunTest.h
///////////////////////////////////////////////////////////////////////////////
