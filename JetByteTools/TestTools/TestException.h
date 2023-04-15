#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TestException.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2003 JetByte Limited.
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

#include "JetByteTools/CoreTools/DebugHelpers.h"
#include "JetByteTools/CoreTools/Exception.h"
#include "JetByteTools/CoreTools/ToString.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestException
///////////////////////////////////////////////////////////////////////////////

class CTestException : public JetByteTools::Core::CException
{
   public :

      explicit CTestException(
         const JetByteTools::Core::_tstring &message);

      CTestException(
         const JetByteTools::Core::_tstring &where,
         const JetByteTools::Core::_tstring &message);

   protected :

      CTestException(
         const JetByteTools::Core::_tstring &where,
         const JetByteTools::Core::_tstring &message,
         bool nonFatal);
};

class CNonFatalTestException : public CTestException
{
   public :

      explicit CNonFatalTestException(
         const JetByteTools::Core::_tstring &message);

      CNonFatalTestException(
         const JetByteTools::Core::_tstring &where,
         const JetByteTools::Core::_tstring &message);

};

class CTestSkippedException : public JetByteTools::Core::CException
{
   public :

      explicit CTestSkippedException(
         const JetByteTools::Core::_tstring &message);

      CTestSkippedException(
         const JetByteTools::Core::_tstring &where,
         const JetByteTools::Core::_tstring &message);
};

#define SKIP_TEST_EX(m) do{throw JetByteTools::Test::CTestSkippedException(m);}while(0)

#define SKIP_TEST(w, m) do{throw JetByteTools::Test::CTestSkippedException(w, m);}while(0)

#define THROW_IF_DONT_MATCH(w, s1, s2) do{if((s1) != (s2)){JetByteTools::Core::Output(_T("Expected: ") + JetByteTools::Core::_tstring(s1));JetByteTools::Core::Output(_T("     got: ") + JetByteTools::Core::_tstring(s2));throw CTestException(w, _T("Test Failed: \"") + JetByteTools::Core::_tstring(s1) + _T("\" != \"") + JetByteTools::Core::_tstring(s2));}}while(0)

#define THROW_ON_FAILURE(w, t) do{if(!(t)){throw JetByteTools::Test::CTestException(w, _T("Test Failed:") _T(#t));}}while(0)

#define THROW_IF_STRINGS_DONT_MATCH(w, s1, s2) do{if(!(s1 == s2)){throw JetByteTools::Test::CTestException(w, _T("Test Failed:\n \"") + s1 + _T("\" !=\n \"") + s2 + _T("\")"));}}while(0)

#define THROW_ON_NO_EXCEPTION(w, e) do{try{e();throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("()"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_1(w, e, p) do{try{e(p);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_SEH_EXCEPTION_1(w, e, p1) do{try{e(p1);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (JetByteTools::Win32::CSEHException &){}catch (...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_2(w, e, p1, p2) do{try{e(p1, p2);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_3(w, e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_SEH_EXCEPTION_3(w, e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (JetByteTools::Win32::CSEHException &){}catch (...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_4(w, e, p1, p2, p3, p4) do{try{e(p1, p2, p3, p4);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_5(w, e, p1, p2, p3, p4, p5) do{try{e(p1, p2, p3, p4, p5);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_6(w, e, p1, p2, p3, p4, p5, p6) do{try{e(p1, p2, p3, p4, p5, p6);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_7(w, e, p1, p2, p3, p4, p5, p6, p7) do{try{e(p1, p2, p3, p4, p5, p6, p7);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6)   _T(", ") _T(#p7) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_8(w, e, p1, p2, p3, p4, p5, p6, p7, p8) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_9(w, e, p1, p2, p3, p4, p5, p6, p7, p8, p9) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_10(w, e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_11(w, e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);throw JetByteTools::Test::CTestException(w, _T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(", ") _T(#p11) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define ERROR_TIMEOUT_STRINGIZE(L) #L
#define ERROR_TIMEOUT_MAKESTRING(M,L) M(L)
#define ERROR_TIMEOUT_MACROASSTRING(x) ERROR_TIMEOUT_MAKESTRING( ERROR_TIMEOUT_STRINGIZE, x)

#ifndef REASONABLE_TIME
#define REASONABLE_TIME (IsDebuggerPresent() ? INFINITE : 60000)
#endif

#define THROW_IF_NOT_SIGNALLED_IN_TIME(w, e, t) do{if (false == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(w, _T("Timeout waiting for ") _T(#e) _T(" after ") _T(ERROR_TIMEOUT_MACROASSTRING(t)) _T("ms")); } }while(0)

#define THROW_IF_NOT_SIGNALLED_IN_REASONABLE_TIME(w, e) THROW_IF_NOT_SIGNALLED_IN_TIME(w, e, REASONABLE_TIME)

#define THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME(w, e, t) do{if (true == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(w, _T(#e) _T(" was signalled after ") _T(ERROR_TIMEOUT_MACROASSTRING(t)) _T("ms and it should never be signalled")); } }while(0)

#ifndef SHORT_TIME_NON_ZERO
#define SHORT_TIME_NON_ZERO 100
#endif

#define THROW_IF_SIGNALLED(w, e) THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME(w, e, SHORT_TIME_NON_ZERO)

#define THROW_ON_FAILURE_EX(t) do{if(!(t)){throw JetByteTools::Test::CTestException(_T("Test Failed:") _T(#t));}}while(0)

#define THROW_IF_STRINGS_DONT_MATCH_EX(s1, s2) do{if(!(s1 == s2)){throw JetByteTools::Test::CTestException(_T("Test Failed:\n \"") + JetByteTools::Core::_tstring(s1) + _T("\" !=\n \"") + JetByteTools::Core::_tstring(s2) + _T("\""));}}while(0)

#define THROW_IF_STRINGS_DONT_MATCH_A_EX(s1, s2) do{if(!(s1 == s2)){throw JetByteTools::Test::CTestException(_T("Test Failed:\n \"") + JetByteTools::Core::_tstring(JetByteTools::Core::CStringConverter::AtoT(s1)) + _T("\" !=\n \"") + JetByteTools::Core::_tstring(JetByteTools::Core::CStringConverter::AtoT(s2)) + _T("\""));}}while(0)

#define THROW_IF_EQUAL_EX(v1, v2) do{if(v1 == v2){throw JetByteTools::Test::CTestException(_T("Test Failed: ") _T(#v1) _T(" (") + JetByteTools::Core::ToString(v1) + _T(") == ") _T(#v2) _T(" (") + JetByteTools::Core::ToString(v2) + _T(")"));}}while(0)

#define THROW_IF_NOT_EQUAL_EX(v1, v2) do{if(v1 != v2){throw JetByteTools::Test::CTestException(_T("Test Failed: ") _T(#v1) _T(" (") + JetByteTools::Core::ToString(v1) + _T(") != ") _T(#v2) _T(" (") + JetByteTools::Core::ToString(v2) + _T(")"));}}while(0)

#define THROW_ON_NO_EXCEPTION_FROM_CTOR_EX(e) do{try{e;throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("()"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX(e) do{try{e();throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("()"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_1(e, p) do{try{e(p);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_2(e, p1, p2) do{try{e(p1, p2);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_TEST_EXCEPTION_EX_2(e, p1, p2) do{try{e(p1, p2);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(")"));}catch (JetByteTools::Test::CTestException &){}catch (...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_3(e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_SEH_EXCEPTION_EX_3(e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (JetByteTools::Win32::CSEHException &){}catch (...){throw;}}while(0)

#define THROW_ON_NO_TEST_EXCEPTION_EX_3(e, p1, p2, p3) do{try{e(p1, p2, p3);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(")"));}catch (JetByteTools::Test::CTestException &){}catch (...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_4(e, p1, p2, p3, p4) do{try{e(p1, p2, p3, p4);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_TEST_EXCEPTION_EX_4(e, p1, p2, p3, p4) do{try{e(p1, p2, p3, p4);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(")"));}catch (JetByteTools::Test::CTestException &){}catch (...){throw;}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_5(e, p1, p2, p3, p4, p5) do{try{e(p1, p2, p3, p4, p5);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_6(e, p1, p2, p3, p4, p5, p6) do{try{e(p1, p2, p3, p4, p5, p6);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_7(e, p1, p2, p3, p4, p5, p6, p7) do{try{e(p1, p2, p3, p4, p5, p6, p7);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6)   _T(", ") _T(#p7) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_8(e, p1, p2, p3, p4, p5, p6, p7, p8) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_9(e, p1, p2, p3, p4, p5, p6, p7, p8, p9) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_10(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_11(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(", ") _T(#p11) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_12(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(", ") _T(#p11) _T(", ") _T(#p12) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_13(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(", ") _T(#p11) _T(", ") _T(#p12) _T(", ") _T(#p13) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_ON_NO_EXCEPTION_EX_14(e, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14) do{try{e(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);throw JetByteTools::Test::CTestException(_T("No Exception from: ") _T(#e) _T("(") _T(#p1) _T(", ") _T(#p2) _T(", ") _T(#p3) _T(", ") _T(#p4) _T(", ") _T(#p5) _T(", ") _T(#p6) _T(", ") _T(#p7) _T(", ") _T(#p8) _T(", ") _T(#p9) _T(", ") _T(#p10) _T(", ") _T(#p11) _T(", ") _T(#p12) _T(", ") _T(#p13) _T(", ") _T(#p14) _T(")"));}catch (JetByteTools::Test::CTestException &){throw;}catch (...){}}while(0)

#define THROW_IF_NOT_SIGNALLED_EX(e) do{if (false == e.Wait(static_cast<JetByteTools::Milliseconds>(0))) { throw JetByteTools::Test::CTestException(_T("Expected ") _T(#e) _T(" to be signalled")); } }while(0)

#define THROW_IF_NOT_SIGNALLED_IN_TIME_EX(e, t) do{if (false == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(_T("Timeout waiting for ") _T(#e) _T(" after ") _T(ERROR_TIMEOUT_MACROASSTRING(t)) _T("ms")); } }while(0)

#define THROW_IF_NOT_SIGNALLED_IN_REASONABLE_TIME_EX(e) THROW_IF_NOT_SIGNALLED_IN_TIME_EX(e, REASONABLE_TIME)

#define THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME_EX(e, t) do{if (true == e.Wait(static_cast<JetByteTools::Milliseconds>(t))) { throw JetByteTools::Test::CTestException(_T(#e) _T(" was signalled after ") _T(ERROR_TIMEOUT_MACROASSTRING(t)) _T("ms and it should never be signalled")); } }while(0)

#define THROW_IF_SIGNALLED_EX(e) THROW_IF_SIGNALLED_WITHIN_GIVEN_TIME_EX(e, SHORT_TIME_NON_ZERO)


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestException.h
///////////////////////////////////////////////////////////////////////////////
