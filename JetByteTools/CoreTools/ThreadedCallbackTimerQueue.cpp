///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2004 JetByte Limited.
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

#include "ThreadedCallbackTimerQueue.h"
#include "CallbackTimerQueueEx.h"
#include "StringConverter.h"
#include "Exception.h"
#include "NullThreadedCallbackTimerQueueMonitor.h"
#include "DebugTrace.h"

#if (JETBYTE_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
#include "PerThreadErrorHandler.h"
#endif

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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

CThreadedCallbackTimerQueue::Handle CThreadedCallbackTimerQueue::CreateTimer()
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::CreateTimerContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
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
   const UserData userData)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::SetTimerContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
   #endif

   const bool wasPending = m_spTimerQueue->SetTimer(handle, timer, timeout, userData);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::CancelTimer(
   const Handle &handle)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::CancelTimerContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
   #endif

   const bool wasPending = m_spTimerQueue->CancelTimer(handle);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   Handle &handle)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::DestroyTimerContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->DestroyTimer(handle);
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   const Handle &handle)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::DestroyTimerContention);

      lock.Lock();
   }

   #else
   CReentrantLockableObject::Owner lock(m_lock);
   #endif

   return m_spTimerQueue->DestroyTimer(handle);
}

void CThreadedCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::SetOneOffTimerContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
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
                     CReentrantLockableObject::PotentialOwner lock(m_lock);

                     if (!lock.TryLock())
                     {
                        m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::TimerProcessingContention);

                        lock.Lock();
                     }
                     #else
                     CReentrantLockableObject::Owner lock(m_lock);
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
         OnThreadTerminationException(_T("CThreadedCallbackTimerQueue::Run() - Exception: ") + e.GetDetails());
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
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::TimerProcessingContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
   #endif

   #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
   m_monitor.OnTimerProcessingStarted();
   #endif

   return m_spTimerQueue->BeginTimeoutHandling();
}

Milliseconds CThreadedCallbackTimerQueue::GetNextTimeout()
{
   #if (JETBYTE_PERF_TIMER_CONTENTION_MONITORING == 1)
   CReentrantLockableObject::PotentialOwner lock(m_lock);

   if (!lock.TryLock())
   {
      m_monitor.OnTimerProcessingContention(IMonitorThreadedCallbackTimerQueue::GetNextTimeoutContention);

      lock.Lock();
   }
   #else
   CReentrantLockableObject::Owner lock(m_lock);
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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
