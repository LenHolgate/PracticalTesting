#pragma once
#ifndef JETBYTE_TOOLS_TEST_TEST_EXCEPTION_INCLUDED__
#define JETBYTE_TOOLS_TEST_TEST_EXCEPTION_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TestException.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
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

#include "JetByteTools\Win32Tools\Exception.h"
#include "JetByteTools\Win32Tools\SEHException.h"
#include "JetByteTools\Win32Tools\ToString.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestException
///////////////////////////////////////////////////////////////////////////////

class CTestException : public JetByteTools::Win32::CException
{
   public :

      explicit CTestException(
         const JetByteTools::Win32::_tstring &message);

      CTestException(
         const JetByteTools::Win32::_tstring &where,
         const JetByteTools::Win32::_tstring &message);

   protected :

      CTestException(
         const JetByteTools::Win32::_tstring &where,
         const JetByteTools::Win32::_tstring &message,
         const bool nonFatal);
};

class CNonFatalTestException : public CTestException
{
   public :

      explicit CNonFatalTestException(
         const JetByteTools::Win32::_tstring &message);

      CNonFatalTestException(
         const JetByteTools::Win32::_tstring &where,
         const JetByteTools::Win32::_tstring &message);

};

class CTestSkippedException : public JetByteTools::Win32::CException
{
   public :

      explicit CTestSkippedException(
         const JetByteTools::Win32::_tstring &message);

      CTestSkippedException(
         const JetByteTools::Win32::_tstring &where,
         const JetByteTools::Win32::_tstring &message);
};

#define SKIP_TEST_EX(m) do{throw JetByteTools::Test::CTestSkippedException(m);}while(0)

#define SKIP_TEST(w, m) do{throw JetByteTools::Test::CTestSkippedException(w, m);}while(0)

#define THROW_IF_DONT_MATCH(w, s1, s2) do{if((s1) != (s2)){JetByteTools::Win32::Output(_T("Expected: ") + JetByteTools::Win32::_tstring(s1));JetByteTools::Win32::Output(_T("     got: ") + JetByteTools::Win32::_tstring(s2));throw CTestException(w, _T("Test Failed: \"") + JetByteTools::Win32::_tstring(s1) + _T("\" != \"") + JetByteTools::Win32::_tstring(s2));}}while(0)

#define THROW_ON_FAILURE(w, t) do{if(!(t)){throw JetByteTools::Test::CTestException(w, _T("Test Failed:") _T(#t));}}while(0)

#define THROW_IF_STRINGS_DONT_MATCH(w, s1, s2) do{if(!(s1 == s2)){throw JetByteTools::Test::CTestException(w, _T("Test Failed:\n \"") + s1 + _T("\" !=\n \"") + s2 + _T("\")"));}}while(0)

#define THROW_ON_NO_EXCEPTION(w, e) do{try{e();throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("()"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_1(w, e, p) do{try{e(p);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_SEH_EXCEPTION_1(w, e, p1) do{try{e(p1);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){}catch(...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_2(w, e, p1, p2) do{try{e(p1, p2);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_3(w, e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_SEH_EXCEPTION_3(w, e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){}catch(...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_4(w, e, p1, p2, p3, p4) do{try{e(p1, p2, p3, p4);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_5(w, e, p1, p2, p3, p4, p5) do{try{e(p1, p2, p3, p4, p5);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_6(w, e, p1, p2, p3, p4, p5, p6) do{try{e(p1, p2, p3, p4, p5, p6);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_7(w, e, p1, p2, p3, p4, p5, p6, p7) do{try{e(p1, p2, p3, p4, p5, p6, p7);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)   _T(", ")  _T(#p7)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_8(w, e, p1, p2, p3, p4, p5, p6, p7, p8) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_9(w, e, p1, p2, p3, p4, p5, p6, p7, p8, p9) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8) _T(", ")  _T(#p9)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_10(w, e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8) _T(", ")  _T(#p9)  _T(", ")  _T(#p10)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_11(w, e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8) _T(", ")  _T(#p9)  _T(", ")  _T(#p10)  _T(", ")  _T(#p11)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define ERROR_TIMEOUT_STRINGIZE(L) #L
#define ERROR_TIMEOUT_MAKESTRING(M,L) M(L)
#define ERROR_TIMEOUT_MACROASSTRING(x) ERROR_TIMEOUT_MAKESTRING( ERROR_TIMEOUT_STRINGIZE, x)

#ifndef REASONABLE_TIME
#define REASONABLE_TIME (::IsDebuggerPresent() ? INFINITE : 60000)
#endif

#define THROW_IF_NOT_SIGNALLED_IN_TIME(w, e, t) do{if (false == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(w, _T("Timeout waiting for ") _T(#e)  _T(" after ")  _T(ERROR_TIMEOUT_MACROASSTRING(t))  _T("ms")); } }while(0)

#define THROW_IF_NOT_SIGNALLED_IN_REASONABLE_TIME(w, e) THROW_IF_NOT_SIGNALLED_IN_TIME(w, e, REASONABLE_TIME)

#define THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME(w, e, t) do{if (true == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(w, _T(#e)  _T(" was signalled after ")  _T(ERROR_TIMEOUT_MACROASSTRING(t))  _T("ms and it should never be signalled")); } }while(0)

#ifndef SHORT_TIME_NON_ZERO
#define SHORT_TIME_NON_ZERO 100
#endif

#define THROW_IF_SIGNALLED(w, e) THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME(w, e, SHORT_TIME_NON_ZERO)

#define THROW_ON_FAILURE_EX(t) do{if(!(t)){throw JetByteTools::Test::CTestException(_T("Test Failed:") _T(#t));}}while(0)

#define THROW_IF_STRINGS_DONT_MATCH_EX(s1, s2) do{if(!(s1 == s2)){throw JetByteTools::Test::CTestException(_T("Test Failed:\n \"") + JetByteTools::Win32::_tstring(s1) + _T("\" !=\n \"") + JetByteTools::Win32::_tstring(s2) + _T("\")"));}}while(0)

#define THROW_IF_STRINGS_DONT_MATCH_A_EX(s1, s2) do{if(!(s1 == s2)){throw JetByteTools::Test::CTestException(_T("Test Failed:\n \"") + JetByteTools::Win32::_tstring(JetByteTools::Win32::CStringConverter::AtoT(s1)) + _T("\" !=\n \"") + JetByteTools::Win32::_tstring(JetByteTools::Win32::CStringConverter::AtoT(s2)) + _T("\")"));}}while(0)

#define THROW_IF_EQUAL_EX(v1, v2) do{if(v1 == v2){throw JetByteTools::Test::CTestException(_T("Test Failed: ") _T(#v1)  _T(" (") + JetByteTools::Win32::ToString(v1) + _T(") == ") _T(#v2)  _T(" (") + JetByteTools::Win32::ToString(v2) + _T(")"));}}while(0)

#define THROW_IF_NOT_EQUAL_EX(v1, v2) do{if(v1 != v2){throw JetByteTools::Test::CTestException(_T("Test Failed: ") _T(#v1)  _T(" (") + JetByteTools::Win32::ToString(v1) + _T(") != ") _T(#v2)  _T(" (") + JetByteTools::Win32::ToString(v2) + _T(")"));}}while(0)

#define THROW_ON_NO_EXCEPTION_EX(e) do{try{e();throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("()"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_1(e, p) do{try{e(p);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_2(e, p1, p2) do{try{e(p1, p2);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_TEST_EXCEPTION_EX_2(e, p1, p2) do{try{e(p1, p2);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(")"));}catch(JetByteTools::Test::CTestException &){}catch(...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_3(e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_SEH_EXCEPTION_EX_3(e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){}catch(...){throw;}}while(0)

#define THROW_ON_NO_TEST_EXCEPTION_EX_3(e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(")"));}catch(JetByteTools::Test::CTestException &){}catch(...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_4(e, p1, p2, p3, p4) do{try{e(p1, p2, p3, p4);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_TEST_EXCEPTION_EX_4(e, p1, p2, p3, p4) do{try{e(p1, p2, p3, p4);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(")"));}catch(JetByteTools::Test::CTestException &){}catch(...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_5(e, p1, p2, p3, p4, p5) do{try{e(p1, p2, p3, p4, p5);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_6(e, p1, p2, p3, p4, p5, p6) do{try{e(p1, p2, p3, p4, p5, p6);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_7(e, p1, p2, p3, p4, p5, p6, p7) do{try{e(p1, p2, p3, p4, p5, p6, p7);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)   _T(", ")  _T(#p7)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_8(e, p1, p2, p3, p4, p5, p6, p7, p8) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_9(e, p1, p2, p3, p4, p5, p6, p7, p8, p9) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8)  _T(", ")  _T(#p9)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_10(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8)  _T(", ")  _T(#p9)  _T(", ")  _T(#p10)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_11(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8)  _T(", ")  _T(#p9)  _T(", ")  _T(#p10)  _T(", ")  _T(#p11)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_12(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e)  _T("(")  _T(#p1)  _T(", ")  _T(#p2)  _T(", ")  _T(#p3)  _T(", ")  _T(#p4)  _T(", ")  _T(#p5)  _T(", ")  _T(#p6)  _T(", ")  _T(#p7)  _T(", ")  _T(#p8)  _T(", ")  _T(#p9)  _T(", ")  _T(#p10)  _T(", ")  _T(#p11)  _T(", ")  _T(#p12)  _T(")"));}catch(JetByteTools::Test::CTestException &){throw;}catch(JetByteTools::Win32::CSEHException &){throw;}catch(...){}}while(0)

#define THROW_IF_NOT_SIGNALLED_EX(e) do{if (false == e.Wait(static_cast<JetByteTools::Milliseconds>(0))) { throw JetByteTools::Test::CTestException(_T("Expected ") _T(#e)  _T(" to be signalled")); } }while(0)

#define THROW_IF_NOT_SIGNALLED_IN_TIME_EX(e, t) do{if (false == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(_T("Timeout waiting for ") _T(#e)  _T(" after ")  _T(ERROR_TIMEOUT_MACROASSTRING(t))  _T("ms")); } }while(0)

#define THROW_IF_NOT_SIGNALLED_IN_REASONABLE_TIME_EX(e) THROW_IF_NOT_SIGNALLED_IN_TIME_EX(e, REASONABLE_TIME)

#define THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME_EX(e, t) do{if (true == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(_T(#e)  _T(" was signalled after ")  _T(ERROR_TIMEOUT_MACROASSTRING(t))  _T("ms and it should never be signalled")); } }while(0)

#define THROW_IF_SIGNALLED_EX(e) THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME_EX(e, SHORT_TIME_NON_ZERO)


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_TEST_TEST_EXCEPTION_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TestException.h
///////////////////////////////////////////////////////////////////////////////
