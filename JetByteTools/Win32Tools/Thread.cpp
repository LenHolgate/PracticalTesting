///////////////////////////////////////////////////////////////////////////////
// File: Thread.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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

#include "Thread.h"
#include "StringConverter.h"
#include "Win32Exception.h"

#pragma hdrstop

#include <process.h>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CThreadNameInfo
///////////////////////////////////////////////////////////////////////////////

class CThreadNameInfo
{
   public :

      static const DWORD CurrentThreadID;

      static void SetThreadName(
         DWORD threadID, 
         const std::string &threadName);

      DWORD GetSize() const;

      ULONG_PTR *GetData() const;

   private :

      CThreadNameInfo(
         const DWORD threadId,
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
   :  m_threadID(0),
      m_runnable(runnable)
{

}

CThread::~CThread()
{

}
      
bool CThread::WasStarted() const
{
   return INVALID_HANDLE_VALUE != m_hThread;
}

void CThread::Start()
{
   Start(false);
}

void CThread::StartSuspended()
{
   Start(true);
}

void CThread::Start(
   bool startSuspended)
{
   if (!WasStarted())
   {
      const uintptr_t result = ::_beginthreadex(0, 0, ThreadFunction, (void*)this, startSuspended, reinterpret_cast<unsigned int*>(&m_threadID));

      if (result == 0)
      {
         throw CWin32Exception(_T("CThread::Start() - _beginthreadex"), errno);
      }

      m_hThread = reinterpret_cast<HANDLE>(result);
   }
   else
   {
      throw CException(_T("CThread::Start()"), _T("Thread already running - you can only call Start() once!"));
   }
}

void CThread::StartWithPriority(
   const int priority)
{
   StartSuspended();

   SetThreadPriority(priority);

   Resume();
}


void CThread::Resume() 
{
   if (WasStarted())
   {
      if (-1 == ::ResumeThread(m_hThread))
      {
         throw CWin32Exception(_T("CThread::Resume()"), ::GetLastError());
      }
   }
   else
   {
      throw CException(_T("CThread::Resume()"), _T("Thread has not been started!"));
   }
}

void CThread::EnableThreadPriorityBoost()
{
   if (!::SetThreadPriorityBoost(m_hThread, FALSE))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CThread::EnableThreadPriorityBoost()"), lastError);
   }
}

void CThread::DisableThreadPriorityBoost()
{
   if (!::SetThreadPriorityBoost(m_hThread, TRUE))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CThread::DisableThreadPriorityBoost()"), lastError);
   }
}

bool CThread::ThreadPriorityBoostEnabled() const
{
   BOOL disabled;

   if (!::GetThreadPriorityBoost(m_hThread, &disabled))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CThread::ThreadPriorityBoostEnabled()"), lastError);
   }

   return !disabled;
}

void CThread::SetThreadPriority(
   const int priority)
{
   if (!::SetThreadPriority(m_hThread, priority))
   {
      const DWORD lastError = ::GetLastError();

      throw CWin32Exception(_T("CThread::SetThreadPriority()"), lastError);
   }
}

int CThread::GetThreadPriority() const
{
   return ::GetThreadPriority(m_hThread);
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
   int result = 0;

   CThread* pThis = (CThread*)pV;
   
   if (pThis)
   {
      try
      {
         result = pThis->m_runnable.Run();
      }
      catch(...)
      {
         result = 666;
      }
   }

   return result;
}

void CThread::Terminate(
   DWORD exitCode)
{
   if (!::TerminateThread(m_hThread, exitCode))
   {
      throw CWin32Exception(_T("CThread::Terminate() - TerminateThread"), ::GetLastError());
   }

   m_hThread = INVALID_HANDLE_VALUE;
}

void CThread::SetThreadName(
   const _tstring &threadName) const
{
   if (WasStarted())
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
   DWORD threadID, 
   const _tstring &threadName)
{
   const std::string narrowString = CStringConverter::TtoA(threadName);
   
   CThreadNameInfo::SetThreadName(threadID, narrowString);
}

bool CThread::IsThisThread() const
{
   bool isThisThread = false;

   if (WasStarted())
   {
      isThisThread = (::GetCurrentThreadId() == m_threadID);
   }

   return isThisThread;
}

///////////////////////////////////////////////////////////////////////////////
// CThreadNameInfo
///////////////////////////////////////////////////////////////////////////////

const DWORD CThreadNameInfo::CurrentThreadID = 0xFFFFFFFF;

CThreadNameInfo::CThreadNameInfo(
   DWORD threadID,
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
   DWORD threadID, 
   const std::string &threadName)
{
   CThreadNameInfo info(threadID, threadName.c_str());

   __try
   {
      ::RaiseException(0x406D1388, 0, info.GetSize(), info.GetData());
   }
   __except(EXCEPTION_CONTINUE_EXECUTION)
   {
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: Thread.cpp
///////////////////////////////////////////////////////////////////////////////

