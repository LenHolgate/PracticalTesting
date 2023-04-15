///////////////////////////////////////////////////////////////////////////////
// File: TestException.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "JetByteTools/CoreTools/DebugHelpers.h"

#include "TestException.h"
#include "TestMonitor.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::CException;
using JetByteTools::Core::_tstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CTestException
///////////////////////////////////////////////////////////////////////////////

CTestException::CTestException(
   const _tstring &message)
   :  CException(_T(""), message)
{
   if (CTestMonitor::DebugOnFailure())
   {
      DebugBreak();
   }
}

CTestException::CTestException(
   const _tstring &where,
   const _tstring &message)
   :  CException(where, message)
{
   if (CTestMonitor::DebugOnFailure())
   {
      DebugBreak();
   }
}

CTestException::CTestException(
   const _tstring &where,
   const _tstring &message,
   const bool nonFatal)
   :  CException(where, message)
{
   if (!nonFatal && CTestMonitor::DebugOnFailure())
   {
      DebugBreak();
   }
}

///////////////////////////////////////////////////////////////////////////////
// CNonFatalTestException
///////////////////////////////////////////////////////////////////////////////

CNonFatalTestException::CNonFatalTestException(
   const _tstring &message)
   :  CTestException(_T(""), message, true)
{
}

CNonFatalTestException::CNonFatalTestException(
   const _tstring &where,
   const _tstring &message)
   :  CTestException(where, message, true)
{
}

///////////////////////////////////////////////////////////////////////////////
// CTestSkippedException
///////////////////////////////////////////////////////////////////////////////

CTestSkippedException::CTestSkippedException(
   const _tstring &message)
   :  CException(_T(""), message)
{
}

CTestSkippedException::CTestSkippedException(
   const _tstring &where,
   const _tstring &message)
   :  CException(where, message)
{
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestException.cpp
///////////////////////////////////////////////////////////////////////////////
