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
      m_logToFile(true)
{
}

CSimpleMessageLog::CSimpleMessageLog(
   const _tstring &fileName,
   const LogTargets targets)
   :  m_fileName(CStringConverter::TtoA(fileName)),
      m_logToCOUT((targets & LogToCOUT) == LogToCOUT),
      m_logToOutputDebugString((targets & LogToOutputDebugString) == LogToOutputDebugString),
      m_logToFile((targets & LogToFile) == LogToFile)

{
}

void CSimpleMessageLog::SetLogName(
   const string &fileName)
{
   CCriticalSection::Owner lock(m_criticalSection);

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
   SetThreadID(identifier + ": ");
}

void CSimpleMessageLog::SetThreadIdentifier(
   const wstring &identifier)
{
   SetThreadID(CStringConverter::WtoA(identifier) + ": ");
}

void CSimpleMessageLog::LogMessage(
   const string &message)
{
   if (m_logToCOUT || m_logToOutputDebugString || m_logToFile)
   {
      CCriticalSection::Owner lock(m_criticalSection);

      const string &threadId = GetThreadID();

      if (m_logToCOUT)
      {
         // cout displays \n as a line break, we don't log it as such...

         std::cout << threadId << message << endl;
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

         m_output << threadId << message << "\r\n";
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
      
      pThreadID = SetThreadID(threadId);
   }

   return *pThreadID;
}

const string *CSimpleMessageLog::SetThreadID(
   const string &threadIdentifier)
{
   CCriticalSection::Owner lock(m_criticalSection);

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
