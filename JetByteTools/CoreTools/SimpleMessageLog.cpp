///////////////////////////////////////////////////////////////////////////////
// File: SimpleMessageLog.cpp
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

#include "SimpleMessageLog.h"
#include "StringConverter.h"
#include "ToString.h"
#include "SystemTime.h"
#include "ThreadId.h"
#include "Thread.h"
#include "AtomicLong.h"

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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CAtomicULong s_nextSequentialThreadId(0);

static const string s_emptyString;

static const DWORD s_includeThreadIdMask = CSimpleMessageLog::IncludeThreadName | CSimpleMessageLog::IncludeThreadId | CSimpleMessageLog::IncludeSequentialThreadId;

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
   if (m_logEntryFormat & IncludeThreadName)
   {
      CThread::AddThreadNameListener(*this);
   }
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
   if (m_logEntryFormat & IncludeThreadName)
   {
      CThread::AddThreadNameListener(*this);
   }
}

_tstring CSimpleMessageLog::MapThreadIdToLogThreadId(
   const _tstring &threadId)
{
   return CStringConverter::AtoT(MapThreadIdToLogThreadIdA(CStringConverter::TtoA(threadId)));
}

string CSimpleMessageLog::MapThreadIdToLogThreadIdA(
   const string &threadId)
{
   CLockableObject::Owner lock(m_lock);

   const auto it = m_threadIdMap.find(threadId);

   if (it != m_threadIdMap.end())
   {
      return it->second;
   }

   return threadId;
}


void CSimpleMessageLog::OnThreadNaming(
   const _tstring &threadId,
   const _tstring &threadName)
{
   // we currently can't update the name once it has been set
   // we'd need a structure for the TLS stuff, and an index
   // to that structure so that we could set its members
   // from any thread...

   if (m_logEntryFormat & IncludeThreadName)
   {
      const string id = CStringConverter::TtoA(threadId);

      const string name = CStringConverter::TtoA(threadName);

      CLockableObject::Owner lock(m_lock);

      m_threadNameMap.insert(ThreadIdMap::value_type(id, name));

      if (m_logEntryFormat & IncludeSequentialThreadId)
      {
         if (m_threadIdMap.end() == m_threadIdMap.find(id))
         {
            const string seqId = ToStringA(s_nextSequentialThreadId.Increment());

            m_threadIdMap.insert(ThreadIdMap::value_type(id, seqId));
         }
      }
   }
}

void CSimpleMessageLog::SetLogName(
   const string &fileName)
{
   CLockableObject::Owner lock(m_lock);

   if (!fileName.empty())
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

   const string *pThreadID = InternalSetThreadIdentifier(
      ToStringA(GetCurrentThreadId()),
      identifier + ": ");

   (void)pThreadID;
}

void CSimpleMessageLog::SetThreadIdentifier(
   const wstring &identifier)
{
   CLockableObject::Owner lock(m_lock);

   const string *pThreadID = InternalSetThreadIdentifier(
      ToStringA(GetCurrentThreadId()),
      CStringConverter::WtoA(identifier) + ": ");

   (void)pThreadID;
}

_tstring CSimpleMessageLog::GetLogMessages() const
{
   _tstring result;

   if (m_logToMemory)
   {
      CLockableObject::Owner lock(m_lock);

      for (const auto &m_message : m_messages)
      {
         result += CStringConverter::AtoT(m_message);
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
      static_cast<int>(localtime.wHour),
      static_cast<int>(localtime.wMinute),
      static_cast<int>(localtime.wSecond),
      static_cast<int>(localtime.wMilliseconds));

   return timestamp;
}

void CSimpleMessageLog::LogMessage(
   const VectorOfLines &messages)
{
   if (m_logToCOUT || m_logToOutputDebugString || m_logToFile)
   {
      CLockableObject::Owner lock(m_lock);

      for (const auto &message : messages)
      {
         LogMessageInternal(message);
      }
   }
}

void CSimpleMessageLog::LogMessage(
   const DequeOfLines &messages)
{
   if (m_logToCOUT || m_logToOutputDebugString || m_logToFile)
   {
      CLockableObject::Owner lock(m_lock);

      for (const auto &message : messages)
      {
         LogMessageInternal(message);
      }
   }
}

void CSimpleMessageLog::LogMessage(
   const string &message)
{
   if (m_logToCOUT || m_logToOutputDebugString || m_logToFile)
   {
      CLockableObject::Owner lock(m_lock);

      LogMessageInternal(message);
   }
}

void CSimpleMessageLog::LogMessageInternal(
   const string &message)
{
   const string &threadId = GetThreadID();

   if (m_logToCOUT)
   {
      // cout displays \n as a line break, we don't log it as such...

      cout << ((m_logEntryFormat & s_includeThreadIdMask) ? threadId : s_emptyString) <<
               ((m_logEntryFormat & IncludeTimestamp) ? GetTimestamp() : s_emptyString) <<
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
         m_output.open(m_fileName.c_str(), std::ios_base::out | std::ios_base::app);//| std::ios_base::binary);

         if (!(m_logEntryFormat & NoNewLogBanner))
         {
            m_output << "****************New Log*****************\r\n";
         }
      }

      m_output << ((m_logEntryFormat & s_includeThreadIdMask) ? threadId : s_emptyString) <<
                  ((m_logEntryFormat & IncludeTimestamp) ? GetTimestamp() : s_emptyString) <<
                     message << endl;
   }

   if (m_logToMemory)
   {
      m_messages.push_back(((m_logEntryFormat & s_includeThreadIdMask) ? threadId : s_emptyString) +
                           ((m_logEntryFormat & IncludeTimestamp) ? GetTimestamp() : s_emptyString) +
                              message);
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
   const string *pThreadID = static_cast<string *>(m_tls.GetValue());

   if (!pThreadID)
   {
      const string threadId = ToStringA(Core::GetCurrentThreadId());

      string identifier;

      if (m_logEntryFormat & IncludeSequentialThreadId)
      {
         string seqId;

         const auto it = m_threadIdMap.find(threadId);

         if (it != m_threadIdMap.end())
         {
            seqId = it->second;
         }
         else
         {
            seqId = ToStringA(s_nextSequentialThreadId.Increment());

            m_threadIdMap.insert(ThreadIdMap::value_type(threadId, seqId));
         }

         identifier = seqId + ": ";
      }
      else if (m_logEntryFormat & IncludeThreadId)
      {
         identifier = threadId + ": ";
      }

      if (m_logEntryFormat & IncludeThreadName)
      {
         const auto it = m_threadNameMap.find(threadId);

         if (it != m_threadNameMap.end())
         {
            identifier += it->second + ": ";
         }
         else
         {
            identifier += "[UNKNOWN]: ";
         }
      }

      pThreadID = InternalSetThreadIdentifier(threadId, identifier);
   }

   return *pThreadID;
}

const string *CSimpleMessageLog::InternalSetThreadIdentifier(
   const string &id,
   const string &identifier)
{
   m_threadIdMap.insert(ThreadIdMap::value_type(id, identifier));

   m_threadIDs.push_back(identifier);

   string *pThreadID = &m_threadIDs.back();

   m_tls.SetValue(pThreadID);

   return pThreadID;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SimpleMessageLog.cpp
///////////////////////////////////////////////////////////////////////////////
