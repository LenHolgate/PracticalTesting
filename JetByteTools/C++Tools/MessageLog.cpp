///////////////////////////////////////////////////////////////////////////////
// File: MessageLog.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "JetByteTools\Admin\Admin.h"

#include "MessageLog.h"
#include "NullMessageLog.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

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
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: MessageLog.cpp
///////////////////////////////////////////////////////////////////////////////
