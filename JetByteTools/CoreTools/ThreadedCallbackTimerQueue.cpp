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
#include "CallbackTimerQueue.h"
#include "StringConverter.h"
#include "Exception.h"
#include "ExceptionLeakPrevention.h"
#include "NullThreadedCallbackTimerQueueMonitor.h"
#include "DebugTrace.h"

#if (JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
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
      m_spTimerQueue(new CCallbackTimerQueue(m_monitor))
{
   m_thread.Start(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueue(m_monitor))
{
   m_thread.Start(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueue(m_monitor, tickProvider))
{
   m_thread.Start(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IMonitorThreadedCallbackTimerQueue &monitor,
   const IProvideTickCount64 &tickProvider)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueue(m_monitor, tickProvider))
{
   m_thread.Start(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   ISupportTimerQueueFacade &impl)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(&impl, false)
{
   m_thread.Start(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   ISupportTimerQueueFacade &impl,
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(&impl, false)
{
   m_thread.Start(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::~CThreadedCallbackTimerQueue()
{
   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_START

   WaitForShutdownToComplete();

   JETBYTE_CATCH_AND_LOG_ALL_IN_DESTRUCTORS_IF_ENABLED_END
}

void CThreadedCallbackTimerQueue::BeginShutdown()
{
   m_shutdownEvent.Set();

   m_spTimerQueue->BeginShutdown(*this);
}

bool CThreadedCallbackTimerQueue::WaitForShutdownToComplete(
   const Milliseconds timeout)
{
   BeginShutdown();

   const bool threadComplete = m_thread.Wait(timeout);

   const bool queueComplete = m_spTimerQueue->WaitForShutdownToComplete(*this, timeout);

   return queueComplete && threadComplete;
}

void CThreadedCallbackTimerQueue::DumpStats(
   const _tstring &message) const
{
   OutputEx(_T("Timer queue stats: ") + message);

   if (m_thread.IsRunning())
   {
      OutputEx(_T("Stats are not available when the timer queue is running"));
   }
   else
   {
      #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
      if (m_stats.stateChanges + m_stats.timeoutsProcessed)
      {
         OutputEx(_T("             Wake ups: ") + ToString(m_stats.stateChanges + m_stats.timeoutsProcessed));
         OutputEx(_T("        State changes: ") + ToString(m_stats.stateChanges));

         if (m_stats.timeoutsProcessed)
         {
            OutputEx(_T("             Timeouts: ") + ToString(m_stats.timeoutsProcessed));
            OutputEx(_T("     Timers processed: ") + ToString(m_stats.totalTimersProcessed));
            OutputEx(_T(" Max timers processed: ") + ToString(m_stats.maxTimersProcessed));
            OutputEx(_T(" Ave timers processed: ") + ToString(m_stats.totalTimersProcessed / m_stats.timeoutsProcessed));
            OutputEx(_T("     Processing loops: ") + ToString(m_stats.totalProcessingLoops));
            OutputEx(_T(" Max processing loops: ") + ToString(m_stats.maxLoopIterations));
            OutputEx(_T("Ave loops per timeout: ") + ToString(m_stats.timeoutsProcessed / m_stats.totalProcessingLoops));

            if (m_stats.totalTimersProcessed > m_stats.totalProcessingLoops)
            {
               OutputEx(_T("  Ave timers per loop: ") + ToString(m_stats.totalTimersProcessed / m_stats.totalProcessingLoops));
            }
            else
            {
               OutputEx(_T("   Ave loop per timer: ") + ToString(m_stats.totalProcessingLoops / m_stats.totalTimersProcessed));
            }
         }
 
         OutputEx(_T("               Create: ") + ToString(m_stats.createTimer));
         OutputEx(_T("                IsSet: ") + ToString(m_stats.timerIsSet));
         OutputEx(_T("                  Set: ") + ToString(m_stats.setTimer1 + m_stats.setTimer2 + m_stats.setTimer3 + m_stats.setTimer4));

         if (m_stats.setTimer1)
         {
            OutputEx(_T("              Set (1): ") + ToString(m_stats.setTimer1));
         }

         if (m_stats.setTimer2)
         {
            OutputEx(_T("              Set (2): ") + ToString(m_stats.setTimer2));
         }

         if (m_stats.setTimer3)
         {
            OutputEx(_T("              Set (3): ") + ToString(m_stats.setTimer3));
         }

         if (m_stats.setTimer4)
         {
            OutputEx(_T("              Set (4): ") + ToString(m_stats.setTimer4));
         }

         OutputEx(_T("               Update: ") + ToString(m_stats.updateTimer1 + m_stats.updateTimer2));

         if (m_stats.updateTimer1)
         {
            OutputEx(_T("           Update (1): ") + ToString(m_stats.updateTimer1));
         }

         if (m_stats.updateTimer2)
         {
            OutputEx(_T("           Update (2): ") + ToString(m_stats.updateTimer2));
         }

         OutputEx(_T("               Cancel: ") + ToString(m_stats.cancelTimer1 + m_stats.cancelTimer2));

         if (m_stats.cancelTimer1)
         {
            OutputEx(_T("           Cancel (1): ") + ToString(m_stats.cancelTimer1));
         }

         if (m_stats.cancelTimer2)
         {
            OutputEx(_T("           Cancel (2): ") + ToString(m_stats.cancelTimer2));
         }

         OutputEx(_T("              Destroy: ") + ToString(m_stats.destroyTimer1 + m_stats.destroyTimer2));

         if (m_stats.destroyTimer1)
         {
            OutputEx(_T("          Destroy (1): ") + ToString(m_stats.destroyTimer1));
         }

         if (m_stats.destroyTimer2)
         {
            OutputEx(_T("          Destroy (2): ") + ToString(m_stats.destroyTimer2));
         }
      }
      #else
      OutputEx(_T("Stats are not available"));
      #endif
   }
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   m_stats.timerIsSet++;
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   m_stats.createTimer++;
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
   const SetTimerIf setTimerIf,
   bool *pOptionalFirstToExpireHasChanged)
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   if (pOptionalFirstToExpireHasChanged)
   {
      m_stats.setTimer1++;
   }
   else
   {
      m_stats.setTimer2++;
   }
   #endif

   bool firstToExpireHasChanged = false;

   const bool wasPending = m_spTimerQueue->SetTimer(handle, timer, timeout, userData, setTimerIf, &firstToExpireHasChanged);

   #if (JETBYTE_PERF_TIMER_OPTIMISE_STATE_CHANGE_SET_TIMER == 1)
   if (firstToExpireHasChanged)
   {
      SignalStateChange();
   }
   #else
   SignalStateChange();
   #endif

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

bool CThreadedCallbackTimerQueue::UpdateTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated,
   bool *pOptionalFirstToExpireHasChanged)
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   if (pOptionalFirstToExpireHasChanged)
   {
      m_stats.updateTimer1++;
   }
   else
   {
      m_stats.updateTimer2++;
   }
   #endif

   bool wasUpdated = false;

   bool firstToExpireHasChanged = false;

   const bool wasPending = m_spTimerQueue->UpdateTimer(handle, timer, timeout, userData, updateIf, &wasUpdated, &firstToExpireHasChanged);

   if (pWasUpdated)
   {
      *pWasUpdated = wasUpdated;
   }

   #if (JETBYTE_PERF_TIMER_OPTIMISE_STATE_CHANGE_UPDATE_TIMER == 1)
   if (firstToExpireHasChanged)
   {
      SignalStateChange();
   }
   #else
   if (wasUpdated)
   {
      SignalStateChange();
   }
   #endif

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

bool CThreadedCallbackTimerQueue::CancelTimer(
   const Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   if (pOptionalFirstToExpireHasChanged)
   {
      m_stats.cancelTimer1++;
   }
   else
   {
      m_stats.cancelTimer2++;
   }
   #endif

   bool firstToExpireHasChanged = false;

   const bool wasPending = m_spTimerQueue->CancelTimer(handle, &firstToExpireHasChanged);

   #if (JETBYTE_PERF_TIMER_OPTIMISE_STATE_CHANGE_CANCEL_TIMER == 1)
   if (firstToExpireHasChanged)
   {
      SignalStateChange();
   }
   #else
   if (wasPending)
   {
      SignalStateChange();
   }
   #endif

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   if (pOptionalFirstToExpireHasChanged)
   {
      m_stats.destroyTimer1++;
   }
   else
   {
      m_stats.destroyTimer2++;
   }
   #endif

   bool firstToExpireHasChanged = false;

   const bool wasPending = m_spTimerQueue->DestroyTimer(handle, &firstToExpireHasChanged);

   #if (JETBYTE_PERF_TIMER_OPTIMISE_STATE_CHANGE_DESTROY_TIMER == 1)
   if (firstToExpireHasChanged)
   {
      SignalStateChange();
   }
   #else
   if (wasPending)
   {
      SignalStateChange();
   }
   #endif

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   return wasPending;
}

void CThreadedCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   bool *pOptionalFirstToExpireHasChanged)
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

   #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
   if (pOptionalFirstToExpireHasChanged)
   {
      m_stats.setTimer3++;
   }
   else
   {
      m_stats.setTimer4++;
   }
   #endif

   bool firstToExpireHasChanged = false;

   m_spTimerQueue->SetTimer(timer, timeout, userData, &firstToExpireHasChanged);

   if (pOptionalFirstToExpireHasChanged)
   {
      *pOptionalFirstToExpireHasChanged = firstToExpireHasChanged;
   }

   #if (JETBYTE_PERF_TIMER_OPTIMISE_STATE_CHANGE_SET_TIMER == 1)
   if (firstToExpireHasChanged)
   {
      SignalStateChange();
   }
   #else
   SignalStateChange();
   #endif
}

Milliseconds CThreadedCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_spTimerQueue->GetMaximumTimeout();
}

unsigned int CThreadedCallbackTimerQueue::Run()
{
   #if (JETBYTE_ADMIN_INSTALL_PER_THREAD_ERROR_HANDLER_IN_CTHREAD == 0)
   CPerThreadErrorHandler errorHandler;
   #endif

   JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_START

   try
   {
      if (OnThreadInitialised())
      {
         HANDLE handles[2] =
         {
            m_shutdownEvent.GetWaitHandle(),
            m_stateChangeEvent.GetWaitHandle()
         };

         bool done = m_shutdownEvent.Wait(0);

         while (!done)
         {
            const Milliseconds timeout = GetNextTimeout();

            if (timeout == 0)
            {
               #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
               m_monitor.OnTimerProcessingStarted();
               #endif

               #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
               m_stats.timeoutsProcessed++;

               size_t processingLoops = 0;
               #endif

               if (BeginTimeoutHandling())
               {
                  #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
                  processingLoops++;
                  #endif

                  do
                  {
                     const size_t timersProcessed = m_spTimerQueue->HandleTimeouts();

                     #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
                     m_stats.totalTimersProcessed += timersProcessed;

                     if (timersProcessed > m_stats.maxTimersProcessed)
                     {
                        m_stats.maxTimersProcessed = timersProcessed;
                     }
                     #else
                     (void)timersProcessed;
                     #endif

                     EndTimeoutHandling();
                  }
                  while (!m_shutdownEvent.Wait(0) && BeginTimeoutHandling());
               }

               #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
               m_stats.totalProcessingLoops += processingLoops;

               if (processingLoops > m_stats.maxLoopIterations)
               {
                  m_stats.maxLoopIterations = processingLoops;
               }
               #endif

               #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
               m_monitor.OnTimerProcessingStopped();
               #endif
            }
            else
            {
               const DWORD result = IWaitable::WaitForMultipleHandles(2, handles, FALSE, timeout);

               if (result == WAIT_OBJECT_0)
               {
                  done = true;
               }
               else if (result == (WAIT_OBJECT_0 + 1))
               {
                  #if (JETBYTE_PERF_TIMER_COLLECT_STATS == 1)
                  m_stats.stateChanges++;
                  #endif
               }
               else if (result != WAIT_TIMEOUT)
               {
                  throw CException(
                     _T("CThreadedCallbackTimerQueue::Run()"),
                     _T("Unexpected return value from WaitForMultipleObjects - ") + ToString(result));
               }
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

   JETBYTE_CATCH_AND_LOG_ALL_AT_THREAD_BOUNDARY_IF_ENABLED_END

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

   return m_spTimerQueue->BeginTimeoutHandling();
}

void CThreadedCallbackTimerQueue::EndTimeoutHandling()
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
   
   m_spTimerQueue->EndTimeoutHandling();
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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
