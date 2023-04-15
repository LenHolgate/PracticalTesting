///////////////////////////////////////////////////////////////////////////////
// File: Thread.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2002 JetByte Limited.
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

#include "Thread.h"

#include "JetByteTools/CoreTools/IRunnable.h"
#include "JetByteTools/CoreTools/StringConverter.h"
#include "JetByteTools/CoreTools/PerThreadErrorHandler.h"
#include "JetByteTools/CoreTools/ToString.h"
#include "JetByteTools/CoreTools/ErrorCodeException.h"

#pragma hdrstop

#include <process.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CThreadNameInfo
///////////////////////////////////////////////////////////////////////////////

class CThreadNameInfo
{
   public :

      static const ThreadId CurrentThreadID;

      static void SetThreadName(
         ThreadId threadID,
         const std::string &threadName);

      DWORD GetSize() const;

      ULONG_PTR *GetData() const;

   private :

      CThreadNameInfo(
         DWORD threadID,
         const char *pThreadName);

      const DWORD_PTR m_type;
      const LPCSTR m_pName;
      const DWORD_PTR m_threadID;
      const DWORD_PTR m_flags;
};

///////////////////////////////////////////////////////////////////////////////
// CThread
///////////////////////////////////////////////////////////////////////////////

CThread::CThread(
   IRunnable &runnable)
   :  m_threadID(Core::InvalidThreadId),
      m_runnable(runnable)
{
}

bool CThread::IsRunning() const
{
   return INVALID_HANDLE_VALUE != m_hThread && !m_hThread.Wait(0);
}

void CThread::Start()
{
   CLockableObject::Owner lock(m_lock);

   InternalStart(false);
}

void CThread::InternalStart(
   const bool startSuspended)
{
   if (!IsRunning())
   {
      const uintptr_t result = _beginthreadex(
         nullptr,
         0,
         ThreadFunction,
         reinterpret_cast<void*>(this),
         startSuspended,
         reinterpret_cast<unsigned int*>(&m_threadID));

      if (result == 0)
      {
         throw CErrorCodeException(_T("CThread::Start() - _beginthreadex"), errno);
      }

      m_hThread = reinterpret_cast<HANDLE>(result);
   }
   else
   {
      throw CException(_T("CThread::Start()"), _T("Thread already running - you can only call Start() once!"));
   }
}

HANDLE CThread::GetWaitHandle() const
{
   return m_hThread.GetWaitHandle();
}

void CThread::Wait() const
{
   m_hThread.Wait();
}

bool CThread::Wait(
   const Milliseconds timeout) const
{
   return m_hThread.Wait(timeout);
}

unsigned int __stdcall CThread::ThreadFunction(
   void *pV)
{
   unsigned int result = 0;

   auto* pThis = static_cast<CThread*>(pV);

   if (pThis)
   {
      #if (JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 1)
      CPerThreadErrorHandler errorHandler;
      #endif

      try
      {
         pThis->NotifyThreadStartListeners();

         result = pThis->m_runnable.Run();
      }
      JETBYTE_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED
      {
         result = 666;
      }

      pThis->NotifyThreadStopListeners();
   }

   return result;
}

void CThread::SetThreadName(
   const _tstring &threadName) const
{
   // TODO - https://docs.microsoft.com/en-gb/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreaddescription?redirectedfrom=MSDN

   CLockableObject::Owner lock(m_lock);

   if (IsRunning())
   {
      SetThreadName(m_threadID, threadName);
   }
   else
   {
      throw CException(_T("CThread::SetThreadName()"), _T("Thread has not been started!"));
   }
}

void CThread::SetCurrentThreadName(
   const _tstring &threadName)
{
   SetThreadName(CThreadNameInfo::CurrentThreadID, threadName);
}

void CThread::SetThreadName(
   ThreadId threadID,
   const _tstring &threadName)
{
   const std::string narrowString = CStringConverter::TtoA(threadName);

   CThreadNameInfo::SetThreadName(threadID, narrowString);

   if (threadID == CThreadNameInfo::CurrentThreadID)
   {
      threadID = GetCurrentThreadId();
   }

   CThreadBase::SetThreadName(ToString(threadID), threadName);
}

bool CThread::IsThisThread() const
{
   bool isThisThread = false;

   if (IsRunning())
   {
      isThisThread = (GetCurrentThreadId() == m_threadID);
   }

   return isThisThread;
}

///////////////////////////////////////////////////////////////////////////////
// CThreadNameInfo
///////////////////////////////////////////////////////////////////////////////

const DWORD CThreadNameInfo::CurrentThreadID = 0xFFFFFFFF;

CThreadNameInfo::CThreadNameInfo(
   const DWORD threadID,
   const char *pThreadName)
   :  m_type(0x1000),
      m_pName(pThreadName),
      m_threadID(threadID),
      m_flags(0)
{
}

DWORD CThreadNameInfo::GetSize() const
{
   // We pass 4 ULONG_PTR sized items to the debugger...

   return 4;
}

ULONG_PTR *CThreadNameInfo::GetData() const
{
   return reinterpret_cast<ULONG_PTR*>(const_cast<CThreadNameInfo *>(this));
}

void CThreadNameInfo::SetThreadName(
   const DWORD threadID,
   const std::string &threadName)
{
   const CThreadNameInfo info(threadID, threadName.c_str());

   __try
   {
      RaiseException(0x406D1388, 0, info.GetSize(), info.GetData());
   }
#pragma warning(suppress: 6312)  // Exception continue execution loop - no, it's OK.
   __except(EXCEPTION_CONTINUE_EXECUTION)
   {
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Thread.cpp
///////////////////////////////////////////////////////////////////////////////
