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
#include "CallbackTimerQueue.h"
#include "CallbackTimerQueueEx.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCountProvider.h"
#include "SEHException.h"
#include "NullThreadedCallbackTimerQueueMonitor.h"
#include "DebugTrace.h"
#include "PerThreadErrorHandler.h"

#pragma hdrstop

#pragma warning(disable: 4355)  // 'this' used as base member initialiser

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static IManageTimerQueue *CreateTimerQueue(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation,
   IMonitorCallbackTimerQueue &monitor);

static bool DispatchWithLock(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation,
   const bool onlyAllowDispatchBits);

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CNullThreadedCallbackTimerQueueMonitor s_monitor;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const short CLASS_MASK    = 0x0F;  // select the class from a TimerQueueImplementation
static const short DISPATCH_MASK = 0xF0;  // select the dispatch type from a TimerQueueImplementation

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const TimerQueueImplementation timerQueueImplementation)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(CreateTimerQueue(timerQueueImplementation, m_monitor), true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, false)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const TimerQueueImplementation timerQueueImplementation,
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(CreateTimerQueue(timerQueueImplementation, m_monitor), true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, false)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount &tickProvider,
   const TimerQueueImplementation timerQueueImplementation)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueue(m_monitor, tickProvider), true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount &tickProvider,
   const TimerQueueImplementation timerQueueImplementation,
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueue(m_monitor, tickProvider), true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider,
   const TimerQueueImplementation timerQueueImplementation)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueueEx(m_monitor, tickProvider), true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider,
   const TimerQueueImplementation timerQueueImplementation,
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(new CCallbackTimerQueueEx(m_monitor, tickProvider), true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IManageTimerQueue &impl,
   const TimerQueueImplementation timerQueueImplementation)
   :  m_monitor(s_monitor),
      m_thread(*this),
      m_spTimerQueue(&impl, false),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IManageTimerQueue &impl,
   const TimerQueueImplementation timerQueueImplementation,
   IMonitorThreadedCallbackTimerQueue &monitor)
   :  m_monitor(monitor),
      m_thread(*this),
      m_spTimerQueue(&impl, false),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
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

bool CThreadedCallbackTimerQueue::DispatchesWithoutLock() const
{
   return !m_dispatchWithLock;
}

int CThreadedCallbackTimerQueue::Run()
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
            if (m_dispatchWithLock)
            {
               while (!m_shutdown)
               {
                  const Milliseconds timeout = GetNextTimeout();

                  if (timeout == 0)
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

                     m_spTimerQueue->HandleTimeouts();

                     #if (JETBYTE_PERF_TIMER_QUEUE_MONITORING == 1)
                     m_monitor.OnTimerProcessingStopped();
                     #endif
                  }
                  else
                  {
                     m_stateChangeEvent.Wait(timeout);
                  }
               }
            }
            else
            {
               while (!m_shutdown)
               {
                  const Milliseconds timeout = GetNextTimeout();

                  if (timeout == 0)
                  {
                     IManageTimerQueue::TimeoutHandle handle = IManageTimerQueue::InvalidTimeoutHandleValue;

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

                        handle = m_spTimerQueue->BeginTimeoutHandling();
                     }

                     if (handle != IManageTimerQueue::InvalidTimeoutHandleValue)
                     {
                        m_spTimerQueue->HandleTimeout(handle);

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

                        m_spTimerQueue->EndTimeoutHandling(handle);
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
            }

            OnThreadShutdown();
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
      catch(const std::exception &e)
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
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static IManageTimerQueue *CreateTimerQueue(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation,
   IMonitorCallbackTimerQueue &monitor)
{
   switch (timerQueueImplementation & CLASS_MASK)
   {
      case CThreadedCallbackTimerQueue::BestForPlatform :

         #if (_WIN32_WINNT >= 0x0600)
         return new CCallbackTimerQueueEx(monitor);
         #else
         return new CCallbackTimerQueue(monitor);
         #endif

      case CThreadedCallbackTimerQueue::TickCount64 :

         #if (_WIN32_WINNT >= 0x0600)
         return new CCallbackTimerQueueEx(monitor);
         #else
         throw CException(
            _T("CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue()"),
            _T("Currently unsupported on this platform."));
         #endif

      case CThreadedCallbackTimerQueue::HybridTickCount64 :

         return new CCallbackTimerQueue(monitor);

      //case CThreadedCallbackTimerQueue::TickCount32 :

      //   // In case we need to support the old implementation

      //break;
   }

   throw CException(
      _T("CThreadedCallbackTimerQueue::CreateTimerQueue()"),
      _T("Unexpected value for timerQueueImplementation: ") + ToString(timerQueueImplementation));
}

static bool DispatchWithLock(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation,
   const bool onlyAllowDispatchBits)
{
   if (onlyAllowDispatchBits && (timerQueueImplementation & CLASS_MASK) != 0)
   {
      throw CException(
         _T("CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue()"),
         _T("TimerQueueImplementation should only specify dispatch option. TickProvider implies implementation"));
   }

   return (static_cast<DWORD>(timerQueueImplementation) & DISPATCH_MASK) == CThreadedCallbackTimerQueue::DispatchTimersWithLock;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
