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

#include "JetByteTools/Admin/Admin.h"

#include "Thread.h"
#include "IRunnable.h"
#include "StringConverter.h"
#include "Win32Exception.h"
#include "PerThreadErrorHandler.h"
#include "IListenToThreadNaming.h"
#include "ToString.h"

#pragma hdrstop

#include <process.h>

#include <deque>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CLockableObject s_lock;

#if (JETBYTE_TRACK_THREAD_NAMES == 1)

CThread::ThreadNames CThread::s_threadNames;

#endif

typedef std::deque<IListenToThreadNaming *> ThreadNameListeners;

static ThreadNameListeners s_threadNameListeners;

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
   :  m_threadID(0),
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

void CThread::StartSuspended()
{
   CLockableObject::Owner lock(m_lock);

   InternalStart(true);
}

void CThread::Start(
   const bool startSuspended)
{
   CLockableObject::Owner lock(m_lock);

   InternalStart(startSuspended);
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
   CLockableObject::Owner lock(m_lock);

   InternalStart(true);

   SetThreadPriority(priority);

   InternalResume();
}

void CThread::Resume()
{
   CLockableObject::Owner lock(m_lock);

   InternalResume();
}

void CThread::InternalResume()
{
   if (IsRunning())
   {
      if (static_cast<DWORD>(-1) == ResumeThread(m_hThread))
      {
         throw CWin32Exception(_T("CThread::Resume()"), GetLastError());
      }
   }
   else
   {
      throw CException(_T("CThread::Resume()"), _T("Thread has not been started!"));
   }
}

void CThread::EnableThreadPriorityBoost()
{
   if (!SetThreadPriorityBoost(m_hThread, FALSE))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CThread::EnableThreadPriorityBoost()"), lastError);
   }
}

void CThread::DisableThreadPriorityBoost()
{
   if (!SetThreadPriorityBoost(m_hThread, TRUE))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CThread::DisableThreadPriorityBoost()"), lastError);
   }
}

bool CThread::ThreadPriorityBoostEnabled() const
{
   BOOL disabled;

   if (!GetThreadPriorityBoost(m_hThread, &disabled))
   {
      const DWORD lastError = GetLastError();

      throw CWin32Exception(_T("CThread::ThreadPriorityBoostEnabled()"), lastError);
   }

   return !disabled;
}

void CThread::SetThreadPriority(
   const int priority)
{
   if (!::SetThreadPriority(m_hThread, priority))
   {
      const DWORD lastError = GetLastError();

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
   unsigned int result = 0;

   auto* pThis = reinterpret_cast<CThread*>(pV);

   if (pThis)
   {
      #if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 1)
      CPerThreadErrorHandler errorHandler;
      #endif

      try
      {
         result = pThis->m_runnable.Run();
      }
      JETBYTE_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED
      {
         result = 666;
      }
   }

   #if (JETBYTE_DELAY_THREAD_TERMINATION_HACK == 1)
   #ifdef JETBYTE_DELAY_THREAD_TERMINATION_DELAY
   ::SleepEx(JETBYTE_DELAY_THREAD_TERMINATION_DELAY, TRUE);
   #else
   ::SleepEx(10, TRUE);
   #endif
   #endif

   return result;
}

void CThread::Terminate(
   const DWORD exitCode)
{
#pragma warning(suppress: 6258)  // Using terminate thread
   if (!TerminateThread(m_hThread, exitCode))
   {
      throw CWin32Exception(_T("CThread::Terminate() - TerminateThread"), GetLastError());
   }

   m_hThread = INVALID_HANDLE_VALUE;
}

void CThread::SetThreadName(
   const _tstring &threadName) const
{
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
   const DWORD threadID,
   const _tstring &threadName)
{
   const std::string narrowString = CStringConverter::TtoA(threadName);

   CThreadNameInfo::SetThreadName(threadID, narrowString);

   CLockableObject::Owner lock(s_lock);

   for (auto *pListener : s_threadNameListeners)
   {
      if (pListener)
      {
         pListener->OnThreadNaming(ToString(threadID), threadName);
      }
   }

#if (JETBYTE_TRACK_THREAD_NAMES == 1)

   if (threadID != CThreadNameInfo::CurrentThreadID)
   {
      s_threadNames[threadID] = threadName;
   }
   else
   {
      s_threadNames[::GetCurrentThreadId()] = threadName;
   }

#endif
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

#if (JETBYTE_TRACK_THREAD_NAMES == 1)

CThread::ThreadNames CThread::GetThreadNames()
{
   CLockableObject::Owner lock(s_lock);

   return s_threadNames;
}

#endif

void CThread::AddThreadNameListener(
   IListenToThreadNaming &listener)
{
   CLockableObject::Owner lock(s_lock);

   s_threadNameListeners.push_back(&listener);
}

void CThread::RemoveThreadNameListener(
   IListenToThreadNaming &listener)
{
   CLockableObject::Owner lock(s_lock);

   const auto it = std::find(s_threadNameListeners.begin(), s_threadNameListeners.end(), &listener);

   if (it != s_threadNameListeners.end())
   {
      *it = nullptr;
   }
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
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Thread.cpp
///////////////////////////////////////////////////////////////////////////////
