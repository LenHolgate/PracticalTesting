///////////////////////////////////////////////////////////////////////////////
// File: MessageLog.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "MessageLog.h"
#include "NullMessageLog.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Static variables
///////////////////////////////////////////////////////////////////////////////

static CNullMessageLog s_nullMessageLog;

///////////////////////////////////////////////////////////////////////////////
// CMessageLog
///////////////////////////////////////////////////////////////////////////////

CMessageLog::CMessageLog()
   :  m_pLog(&s_nullMessageLog)
{
}

CMessageLog::CMessageLog(
   ILogMessages &log)
   :  m_pLog(&log)
{
}

CMessageLog::~CMessageLog()
{
   m_pLog = nullptr;
}

ILogMessages *CMessageLog::SetLog(
   ILogMessages &log)
{
   ILogMessages *pOldLog = m_pLog;

   m_pLog = &log;

   return pOldLog;
}

void CMessageLog::ClearLog()
{
   m_pLog = &s_nullMessageLog;
}

void CMessageLog::SetThreadIdentifier(
   const string &identifier)
{
   m_pLog->SetThreadIdentifier(identifier);
}

void CMessageLog::SetThreadIdentifier(
   const wstring &identifier)
{
   m_pLog->SetThreadIdentifier(identifier);
}

void CMessageLog::SetLogName(
   const string &name)
{
   m_pLog->SetLogName(name);
}

void CMessageLog::SetLogName(
   const wstring &name)
{
   m_pLog->SetLogName(name);
}

void CMessageLog::LogMessage(
   const VectorOfLines &messages)
{
   m_pLog->LogMessage(messages);
}

void CMessageLog::LogMessage(
   const DequeOfLines &messages)
{
   m_pLog->LogMessage(messages);
}

void CMessageLog::LogMessage(
   const string &message)
{
   m_pLog->LogMessage(message);
}

void CMessageLog::LogMessage(
   const wstring &message)
{
   m_pLog->LogMessage(message);
}

void CMessageLog::LogMessage(
   const char * const pString)
{
   m_pLog->LogMessage(pString);
}

void CMessageLog::LogMessage(
   const wchar_t * const pString)
{
   m_pLog->LogMessage(pString);
}

void CMessageLog::LogMessage(
   const char * const pString,
   const DataLength stringLength)
{
   m_pLog->LogMessage(pString, stringLength);
}

void CMessageLog::LogMessage(
   const wchar_t * const pString,
   const DataLength stringLength)
{
   m_pLog->LogMessage(pString, stringLength);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MessageLog.cpp
///////////////////////////////////////////////////////////////////////////////
