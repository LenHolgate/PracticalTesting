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
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation);

static bool DispatchWithLock(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation,
   const bool onlyAllowDispatchBits);

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
   :  m_thread(*this),
      m_pTimerQueue(CreateTimerQueue(timerQueueImplementation)),
      m_weOwnImpl(true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, false)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount &tickProvider,
   const TimerQueueImplementation timerQueueImplementation)
   :  m_thread(*this),
      m_pTimerQueue(new CCallbackTimerQueue(tickProvider)),
      m_weOwnImpl(true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   const IProvideTickCount64 &tickProvider,
   const TimerQueueImplementation timerQueueImplementation)
   :  m_thread(*this),
      m_pTimerQueue(new CCallbackTimerQueueEx(tickProvider)),
      m_weOwnImpl(true),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue(
   IManageTimerQueue &impl,
   const TimerQueueImplementation timerQueueImplementation)
   :  m_thread(*this),
      m_pTimerQueue(&impl),
      m_weOwnImpl(false),
      m_dispatchWithLock(DispatchWithLock(timerQueueImplementation, true)),
      m_shutdown(false)
{
   m_thread.Start();

   m_thread.SetThreadName(_T("TimerQueue"));
}

CThreadedCallbackTimerQueue::~CThreadedCallbackTimerQueue()
{
   InitiateShutdown();

   m_thread.Wait();

   if (m_weOwnImpl)
   {
      delete m_pTimerQueue;
   }
}

CThreadedCallbackTimerQueue::Handle CThreadedCallbackTimerQueue::CreateTimer()
{
   CCriticalSection::Owner lock(m_criticalSection);
   
   return m_pTimerQueue->CreateTimer();
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
   CCriticalSection::Owner lock(m_criticalSection);

   const bool wasPending = m_pTimerQueue->SetTimer(handle, timer, timeout, userData);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::CancelTimer(
   const Handle &handle)
{
   CCriticalSection::Owner lock(m_criticalSection);
   
   const bool wasPending = m_pTimerQueue->CancelTimer(handle);

   SignalStateChange();

   return wasPending;
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   Handle &handle)
{
   CCriticalSection::Owner lock(m_criticalSection);

   return m_pTimerQueue->DestroyTimer(handle);
}

bool CThreadedCallbackTimerQueue::DestroyTimer(
   const Handle &handle)
{
   CCriticalSection::Owner lock(m_criticalSection);

   return m_pTimerQueue->DestroyTimer(handle);
}

void CThreadedCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   CCriticalSection::Owner lock(m_criticalSection);

   m_pTimerQueue->SetTimer(timer, timeout, userData);

   SignalStateChange();
}

Milliseconds CThreadedCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_pTimerQueue->GetMaximumTimeout();
}

int CThreadedCallbackTimerQueue::Run() throw()
{
   CSEHException::Translator sehTranslator;
   
   try
   {
      if (m_dispatchWithLock)
      {
         while (!m_shutdown)
         {
            const Milliseconds timeout = GetNextTimeout();
       
            if (timeout == 0)
            {
               CCriticalSection::Owner lock(m_criticalSection);
      
               m_pTimerQueue->HandleTimeouts();
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
                  CCriticalSection::Owner lock(m_criticalSection);
      
                  handle = m_pTimerQueue->BeginTimeoutHandling();
               }

               if (handle != IManageTimerQueue::InvalidTimeoutHandleValue)
               {
                  m_pTimerQueue->HandleTimeout(handle);

                  CCriticalSection::Owner lock(m_criticalSection);

                  m_pTimerQueue->EndTimeoutHandling(handle);
               }
            }
            else 
            {
               m_stateChangeEvent.Wait(timeout);
            }
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

   return m_pTimerQueue->GetNextTimeout();
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
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static IManageTimerQueue *CreateTimerQueue(
   const CThreadedCallbackTimerQueue::TimerQueueImplementation timerQueueImplementation)
{
   switch (timerQueueImplementation & CLASS_MASK)
   {
      case CThreadedCallbackTimerQueue::BestForPlatform :

#if (_WIN32_WINNT >= 0x0600) 

         return new CCallbackTimerQueueEx();

#else

         return new CCallbackTimerQueue();

#endif 

      break;

      case CThreadedCallbackTimerQueue::TickCount64 :

#if (_WIN32_WINNT >= 0x0600)  

      return new CCallbackTimerQueueEx();

#else

   throw CException(
      _T("CThreadedCallbackTimerQueue::CThreadedCallbackTimerQueue()"), 
      _T("Currently unsupported on this platform."));

#endif

      break;

      case CThreadedCallbackTimerQueue::HybridTickCount64 : 

         return new CCallbackTimerQueue();

      break;

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
