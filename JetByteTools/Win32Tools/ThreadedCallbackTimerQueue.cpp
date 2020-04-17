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

#include "JetByteTools/Admin/Admin.h"

#include "ThreadedCallbackTimerQueue.h"
#include "CallbackTimerQueueEx.h"
#include "Utils.h"
#include "Exception.h"
#include "SEHException.h"
#include "NullThreadedCallbackTimerQueueMonitor.h"
#include "DebugTrace.h"

#if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
#include "PerThreadErrorHandler.h"
#endif

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CNullThreadedCallbackTimerQueueMonitor s_monitor;

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue()
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueueEx(m_monitor)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueueEx(m_monitor)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueueEx(m_monitor, tickProvider)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IMonitorThreadedCallbackTimerQueue &monitor,
   const IProvideTickCount64 &tickProvider)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueueEx(m_monitor, tickProvider)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IManageTimerQueue &impl)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(&impl, false),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IManageTimerQueue &impl,
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(&impl, false),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::~CThreadedCallbackTimerQueue()
{
   try
   {
      WaitForShutdownToComplete();
   }
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED
}

void CThreadedCallbackTimerQueue::BeginShutdown()
{
   m_shutdown = true;

   SignalStateChange();
}

bool CThreadedCallbackTimerQueue::WaitForShutdownToComplete(
   const Milliseconds timeout)
{
   BeginShutdown();

   return m_thread.Wait(timeout);
}

bool CThreadedCallbackTimerQueue::TimerIsSet(
   const Handle &handle) const
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::IsSetTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->TimerIsSet(handle);
}

CThreadedCallbackTimerQueue::Handle CThreadedCallbackTimerQueue::CreateTimer()
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::CreateTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->CreateTimer();
}

void CThreadedCallbackTimerQueue::SetThreadName(
   const _tstring &threadName) const
{
   m_thread.SetThreadName(threadName);
}

bool CThreadedCallbackTimerQueue::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const SetTimerIf setTimerIf)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::SetTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   const bool wasPending = m_spTimerQueue->SetTimer(handle, timer, timeout, userData, setTimerIf);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::UpdateTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::SetTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   bool wasUpdated = false;

   const bool wasPending = m_spTimerQueue->UpdateTimer(handle, timer, timeout, userData, updateIf, &wasUpdated);

   if (pWasUpdated)
   {
      *pWasUpdated = wasUpdated;
   }

   if (wasUpdated && updateIf != UpdateAlwaysNoTimeoutChange)
   {
      SignalStateChange();
   }

   return wasPending;
}

bool CThreadedCallbackTimerQueue::CancelTimer(
   const Handle &handle)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::CancelTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   const bool wasPending = m_spTimerQueue->CancelTimer(handle);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   Handle &handle)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::DestroyTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->DestroyTimer(handle);
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   const Handle &handle)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::DestroyTimerContention);

      lock.Lock();
   }

   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->DestroyTimer(handle);
}

void CThreadedCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::SetOneOffTimerContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   m_spTimerQueue->SetTimer(timer, timeout, userData);

   SignalStateChange();
}

Milliseconds CThreadedCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_spTimerQueue->GetMaximumTimeout();
}

unsigned int CThreadedCallbackTimerQueue::Run()
{
   #if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
   CPerThreadErrorHandler errorHandler;
   #endif

   try
   {
      try
      {
         if (OnThreadInitialised())
         {
            while (!m_shutdown)
            {
               const Milliseconds timeout = GetNextTimeout();

               if (timeout == 0)
               {
                     if (BeginTimeoutHandling())
                  {
                        m_spTimerQueue->HandleTimeout();

                     #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
                     CLockableObject::PotentialOwner lock(m_lock);

                     if (!lock.TryLock())
                     {
                        m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::TimerProcessingContention);

                        lock.Lock();
                     }
                     #else
                     CLockableObject::Owner lock(m_lock);
                     #endif

                     m_spTimerQueue->EndTimeoutHandling();
                  }

                  #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
                  m_monitor.OnTimerProcessingStopped();
                  #endif

               }
               else
               {
                  m_stateChangeEvent.Wait(timeout);
               }
            }

            OnThreadShutdown();
         }
      }
      catch (const CException &e)
      {
         OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - Exception: ") + e.GetWhere() + _T(" - ") + e.GetMessage());
      }
      catch (const CSEHException &e)
      {
         OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - SEH Exception: ") + e.GetWhere() + _T(" - ") + e.GetMessage());
      }
      catch (const std::exception &e)
      {
         OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - STD Exception: ") + CStringConverter::AtoT(e.what()));
      }
      JETBYTE_CATCH_ALL_AT_THREAD_BOUNDARY_IF_ENABLED
      {
         OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - Unexpected exception"));
      }
   }
   JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED

   return 0;
}

bool CThreadedCallbackTimerQueue::BeginTimeoutHandling()
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::TimerProcessingContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerProcessingStarted();
   #endif

   return m_spTimerQueue->BeginTimeoutHandling();
}

Milliseconds CThreadedCallbackTimerQueue::GetNextTimeout()
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::GetNextTimeoutContention);

      lock.Lock();
   }
   #else
   CLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->GetNextTimeout();
}

void CThreadedCallbackTimerQueue::SignalStateChange()
{
   m_stateChangeEvent.Set();
}

void CThreadedCallbackTimerQueue::OnThreadTerminationException(
   const _tstring &message)
{
   OutputEx(_T("CThreadedCallbackTimerQueue::OnThreadTerminationException() - ") + message);

   // derived class can disable this.
}

bool CThreadedCallbackTimerQueue::OnThreadInitialised()
{
   return true;
}

void CThreadedCallbackTimerQueue::OnThreadShutdown()
{

}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
