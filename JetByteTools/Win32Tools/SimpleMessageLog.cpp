///////////////////////////////////////////////////////////////////////////////
// File: SimpleMessageLog.cpp
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

#include "SimpleMessageLog.h"
#include "StringConverter.h"
#include "Utils.h"
#include "SystemTime.h"

#pragma hdrstop

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;
using std::wstring;
using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSimpleMessageLog
///////////////////////////////////////////////////////////////////////////////

CSimpleMessageLog::CSimpleMessageLog()
   :  m_fileName("JetByteTools.log"),
      m_logToCOUT(true),
      m_logToOutputDebugString(true),
      m_logToFile(true),
      m_logToMemory(false),
      m_logEntryFormat(IncludeThreadId)
{
}

CSimpleMessageLog::CSimpleMessageLog(
   const _tstring &fileName,
   const DWORD targets,
   const DWORD logEntryFormat)
   :  m_fileName(CStringConverter::TtoA(fileName)),
      m_logToCOUT((targets & LogToCOUT) == LogToCOUT),
      m_logToOutputDebugString((targets & LogToOutputDebugString) == LogToOutputDebugString),
      m_logToFile((targets & LogToFile) == LogToFile),
      m_logToMemory((targets & LogToMemoryBuffer) == LogToMemoryBuffer),
      m_logEntryFormat(static_cast<LogEntryFormat>(logEntryFormat))

{
}

void CSimpleMessageLog::SetLogName(
   const string &fileName)
{
   CLockableObject::Owner lock(m_lock);

   if (fileName != "")
   {
      m_fileName = fileName;
   }

   if (m_output.is_open())
   {
      m_output.close();
   }
}

void CSimpleMessageLog::SetLogName(
   const wstring &fileName)
{
   SetLogName(CStringConverter::WtoA(fileName));
}

void CSimpleMessageLog::SetThreadIdentifier(
   const string &identifier)
{
   CLockableObject::Owner lock(m_lock);

   const string *pThreadID = InternalSetThreadIdentifier(identifier + ": ");

   (void)pThreadID;
}

void CSimpleMessageLog::SetThreadIdentifier(
   const wstring &identifier)
{
   CLockableObject::Owner lock(m_lock);

   const string *pThreadID = InternalSetThreadIdentifier(CStringConverter::WtoA(identifier) + ": ");

   (void)pThreadID;
}

_tstring CSimpleMessageLog::GetLogMessages() const
{
   _tstring result;

   if (m_logToMemory)
   {
      CLockableObject::Owner lock(m_lock);

      for (Messages::const_iterator it = m_messages.begin(), end = m_messages.end();
         it != end;
         ++it)
      {
         result += CStringConverter::AtoT(*it);
         result += _T("\n");
      }
   }
   else
   {
      result = _T("Memory backed message log not enabled");
   }

   return result;
}

string CSimpleMessageLog::GetTimestamp() const
{
   CSystemTime localtime;

   localtime.GetLocalTime();

   char timestamp[16];

   sprintf_s(
      timestamp,
      sizeof(timestamp),
      "%02d:%02d:%02d.%03d - ",
      localtime.wHour,
      localtime.wMinute,
      localtime.wSecond,
      localtime.wMilliseconds);

   return timestamp;
}

static const string s_emptyString;

void CSimpleMessageLog::LogMessage(
   const string &message)
{
   if (m_logToCOUT || m_logToOutputDebugString || m_logToFile)
   {
      CLockableObject::Owner lock(m_lock);

      const string &threadId = GetThreadID();

      if (m_logToCOUT)
      {
         // cout displays \n as a line break, we don't log it as such...

         cout << (m_logEntryFormat & IncludeThreadId ? threadId : s_emptyString) << 
                 (m_logEntryFormat & IncludeTimestamp ? GetTimestamp() : s_emptyString) <<
                  message << endl;
      }

      if (m_logToOutputDebugString)
      {
         const string msg = threadId + message + "\n";

         OutputDebugStringA(msg.c_str());
      }

      if (m_logToFile)
      {
         if (!m_output.is_open())
         {
            m_output.open(m_fileName.c_str(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);

            m_output << "****************New Log*****************\r\n";
         }

         m_output << (m_logEntryFormat & IncludeThreadId ? threadId : s_emptyString) << 
                     (m_logEntryFormat & IncludeTimestamp ? GetTimestamp() : s_emptyString) <<
                      message << "\r\n";
      }

      if (m_logToMemory)
      {
         m_messages.push_back((m_logEntryFormat & IncludeThreadId ? threadId : s_emptyString) +
                              (m_logEntryFormat & IncludeTimestamp ? GetTimestamp() : s_emptyString) + 
                               message);
      }
   }
}

void CSimpleMessageLog::LogMessage(
   const wstring &message)
{
   LogMessage(CStringConverter::WtoA(message));
}

void CSimpleMessageLog::LogMessage(
   const char * const pString)
{
   const string message(pString);

   LogMessage(message);
}

void CSimpleMessageLog::LogMessage(
   const wchar_t * const pString)
{
   LogMessage(CStringConverter::WtoA(pString));
}

void CSimpleMessageLog::LogMessage(
   const char * const pString,
   const DataLength stringLength)
{
   const string message(pString, stringLength);

   LogMessage(message);
}

void CSimpleMessageLog::LogMessage(
   const wchar_t * const pString,
   const DataLength stringLength)
{
   LogMessage(CStringConverter::WtoA(pString, stringLength));
}

const string &CSimpleMessageLog::GetThreadID()
{
   const string *pThreadID = reinterpret_cast<string *>(m_tls.GetValue());

   if (!pThreadID)
   {
      const string threadId = ToStringA(GetCurrentThreadId()) + ": ";

      pThreadID = InternalSetThreadIdentifier(threadId);
   }

   return *pThreadID;
}

const string *CSimpleMessageLog::InternalSetThreadIdentifier(
   const string &threadIdentifier)
{
   m_threadIDs.push_back(threadIdentifier);

   string *pThreadID = &m_threadIDs.back();

   m_tls.SetValue(pThreadID);

   return pThreadID;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SimpleMessageLog.cpp
///////////////////////////////////////////////////////////////////////////////
