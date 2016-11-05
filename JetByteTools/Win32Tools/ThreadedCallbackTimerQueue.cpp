///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "ThreadedCallbackTimerQueue.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCountProvider.h"
#include "SEHException.h"

#pragma hdrstop

#pragma warning(disable: 4355)  // 'this' used as base member initialiser

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  m_thread(*this),
      m_timerQueue(tickProvider),
      m_shutdown(false)
{
   m_thread.Start();
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const Milliseconds maxTimeout,
   const IProvideTickCount &tickProvider)
   :  m_thread(*this),
      m_timerQueue(maxTimeout, tickProvider),
      m_shutdown(false)
{   
   m_thread.Start();
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue()
   :  m_thread(*this),
      m_shutdown(false)
{
   m_thread.Start();
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const Milliseconds maxTimeout)
   :  m_thread(*this),
      m_timerQueue(maxTimeout),
      m_shutdown(false)
{
   m_thread.Start();
}

CThreadedCallbackTimerQueue::~CThreadedCallbackTimerQueue()
{
   InitiateShutdown();

   m_thread.Wait();
}

CThreadedCallbackTimerQueue::Handle CThreadedCallbackTimerQueue::CreateTimer()
{
   CCriticalSection::Owner lock(m_criticalSection);
   
   return m_timerQueue.CreateTimer();
}

bool CThreadedCallbackTimerQueue::SetTimer(
   const Handle &handle, 
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   CCriticalSection::Owner lock(m_criticalSection);

   const bool wasPending = m_timerQueue.SetTimer(handle, timer, timeout, userData);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::CancelTimer(
   const Handle &handle)
{
   CCriticalSection::Owner lock(m_criticalSection);
   
   const bool wasPending = m_timerQueue.CancelTimer(handle);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   Handle &handle)
{
   CCriticalSection::Owner lock(m_criticalSection);

   return m_timerQueue.DestroyTimer(handle);
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   const Handle &handle)
{
   CCriticalSection::Owner lock(m_criticalSection);

   return m_timerQueue.DestroyTimer(handle);
}

void CThreadedCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   CCriticalSection::Owner lock(m_criticalSection);

   m_timerQueue.SetTimer(timer, timeout, userData);

   SignalStateChange();
}

Milliseconds CThreadedCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_timerQueue.GetMaximumTimeout();
}

int CThreadedCallbackTimerQueue::Run() throw()
{
   CSEHException::Translator sehTranslator;
   
   try
   {
      while (!m_shutdown)
      {
         const Milliseconds timeout = GetNextTimeout();
    
         if (timeout == 0)
         {
            CCriticalSection::Owner lock(m_criticalSection);
   
            m_timerQueue.HandleTimeouts();
         }
         else 
         {
            m_stateChangeEvent.Wait(timeout);
         }
      }
   }
   catch(const CException &e)
   {
      OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - Exception: ") + e.GetWhere() + _T(" - ") + e.GetMessage());
   }
   catch(const CSEHException &e)
   {
      OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - SEH Exception: ") + e.GetWhere() + _T(" - ") + e.GetMessage());
   }
   catch(...)
   {
      OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - Unexpected exception"));
   }

   return 0;
}

Milliseconds CThreadedCallbackTimerQueue::GetNextTimeout()
{
   CCriticalSection::Owner lock(m_criticalSection);

   return m_timerQueue.GetNextTimeout();
}

void CThreadedCallbackTimerQueue::InitiateShutdown()
{
   m_shutdown = true;

   SignalStateChange();
}

void CThreadedCallbackTimerQueue::SignalStateChange()
{
   m_stateChangeEvent.Set();
}

void CThreadedCallbackTimerQueue::OnThreadTerminationException(
   const _tstring & /*message*/)
{
   // derived class could/should log?
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
