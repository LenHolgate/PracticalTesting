#pragma once
#ifndef JETBYTE_TOOLS_TEST_RUN_TEST_INCLUDED__
#define JETBYTE_TOOLS_TEST_RUN_TEST_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: RunTest.h
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

#pragma warning(disable: 4127)   // conditional expression is constant

#include "TestException.h"
#include "TestMonitor.h"

#include "JetByteTools/Win32Tools/Exception.h"
#include "JetByteTools/Win32Tools/Win32Exception.h"
#include "JetByteTools/Win32Tools/SEHException.h"
#include "JetByteTools/Win32Tools/StringConverter.h"

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
   const JetByteTools::Win32::_tstring functionName(_T(#f) _T("()"));         \
                                                                              \
   try                                                                        \
   {                                                                          \
      JetByteTools::Win32::Output(functionName + _T(" - start"));             \
                                                                              \
      f();                                                                    \
                                                                              \
      JetByteTools::Win32::Output(functionName + _T(" - stop"));              \
   }                                                                          \
   catch (JetByteTools::Test::CTestException &e)                              \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: CTestException - ") + e.GetWhere() +                \
         _T(" - ") + e.GetMessage());                                         \
   }                                                                          \
   catch (JetByteTools::Win32::CWin32Exception &e)                            \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: CTestException - ") + e.GetMessage());              \
   }                                                                          \
   catch (JetByteTools::Win32::CException &e)                                 \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: CException - ") + e.GetWhere() +                    \
         _T(" - ") + e.GetMessage());                                         \
   }                                                                          \
   catch (JetByteTools::Win32::CSEHException &e)                              \
   {                                                                          \
      JetByteTools::Test::CCallStackCreator stackWalker;                      \
                                                                              \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: CSEHException - ") + e.GetWhere() +                 \
         _T(" - ") + e.GetMessage() +                                         \
         _T(" - ") + stackWalker.GetStack(&e.GetContext()));                  \
   }                                                                          \
   catch (std::exception &e)                                                  \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: std::exception - ") +                               \
         JetByteTools::Win32::CStringConverter::AtoT(e.what()));              \
   }                                                                          \
   catch (const char *pE)                                                     \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: Exception - ") +                                    \
         JetByteTools::Win32::CStringConverter::AtoT(pE));                    \
   }                                                                          \
   catch (const wchar_t *pE)                                                  \
   {                                                                          \
      throw JetByteTools::Test::CTestException(                               \
         functionName,                                                        \
         _T("Test failed: Exception - ") +                                    \
         JetByteTools::Win32::CStringConverter::WtoT(pE));                    \
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
   const JetByteTools::Win32::_tstring functionName(_T(#f) _T("()"));         \
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
      const JetByteTools::Win32::_tstring t_(t);                              \
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
      const JetByteTools::Win32::_tstring t_(t);                              \
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
      const JetByteTools::Win32::_tstring t_(t);                              \
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
      const JetByteTools::Win32::_tstring t_(t);                              \
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

#endif // JETBYTE_TOOLS_TEST_RUN_TEST_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: RunTest.h
///////////////////////////////////////////////////////////////////////////////
