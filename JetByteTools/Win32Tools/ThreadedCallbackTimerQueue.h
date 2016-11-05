#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
#define JETBYTE_TOOLS_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueue.h
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

#include "JetByteTools\Admin\Types.h"

#include "IManageTimerQueue.h"
#include "IProvideTickCount.h"
#include "IProvideTickCount64.h"
#include "Thread.h"
#include "IRunnable.h"
#include "AutoResetEvent.h"
#include "CriticalSection.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// A class that manages a group of timers that implement IQueueTimers::Timer 
/// and which have their IQueueTimers::Timer::OnTimer() method called when the 
/// timer expires. The class uses an implementation of IManageTimerQueue to manage 
/// the timers and then manages its own timeouts using a thread to call 
/// IManageTimerQueue::HandleTimeouts() or IManageTimerQueue::BeginTimeoutHandling()
/// every GetNextTimeout() milliseconds. 
/// You can configure it to use CCallbackTimerQueue, CCallbackTimerQueueEx or 
/// supply your own implementation of IManageTimerQueue and you can configure 
/// either lock held timer dispatch or no locks timer dispatch.
/// See <a href="http://www.lenholgate.com/archives/000381.html">here</a> for
/// more details.
/// \ingroup Timers
/// \ingroup Threading

class CThreadedCallbackTimerQueue : 
   public IQueueTimers,
   private IRunnable
{
   public :

      enum TimerQueueImplementation
      {
         /// Select CCallbackTimerQueueEx  as the implementation if it's available
         BestForPlatform         = 0x01,  

         /// Select CCallbackTimerQueueEx as the implementation
         TickCount64             = 0x02,  

         /// Select CCallbackTimerQueue as the implementation
         HybridTickCount64       = 0x04,  
         //TickCount32        

         /// Holds our internal lock when dispatching timers (potentially slightly faster)
         DispatchTimersWithLock  = 0x00,  

         /// Does not hold our internal lock when dispatching timers
         DispatchTimersNoLock    = 0x10,  

         BestForPlatformNoLock   =  BestForPlatform | DispatchTimersNoLock,
         TickCount64NoLock       =  TickCount64 | DispatchTimersNoLock,
         HybridTickCount64NoLock =  HybridTickCount64 | DispatchTimersNoLock
      };

      /// Create a timer queue using the specified implementation.

      explicit CThreadedCallbackTimerQueue(
         const TimerQueueImplementation timerQueueImplementation = BestForPlatform);

      /// Create a timer queue that uses CCallbackTimerQueue as its implementation and
      /// that uses the provdided instance of IProvideTickCount to obtain its tick 
      /// counts rather than getting them directly from the system.

      explicit CThreadedCallbackTimerQueue(
         const IProvideTickCount &tickProvider,
         const TimerQueueImplementation timerQueueImplementation = DispatchTimersWithLock);

      /// Create a timer queue that uses CCallbackTimerQueueEx as its implementation and
      /// that uses the provdided instance of IProvideTickCount64 to obtain its tick 
      /// counts rather than getting them directly from the system.

      explicit CThreadedCallbackTimerQueue(
         const IProvideTickCount64 &tickProvider,
         const TimerQueueImplementation timerQueueImplementation = DispatchTimersWithLock);

      /// Create a timer queue that uses the supplied instance of IManageTimerQueue as its 
      /// implementation. Note that we don't take ownership of the implementation, it's up
      /// to you to manage its lifetime.

      explicit CThreadedCallbackTimerQueue(
         IManageTimerQueue &impl,
         const TimerQueueImplementation timerQueueImplementation = DispatchTimersWithLock);

      ~CThreadedCallbackTimerQueue();

      /// Sets the name of the timer queue thread as displayed in the Visual Studio debugger
      /// to the supplied name. By default the constructors set the name of the thread
      /// to "TimerQueue".

      void SetThreadName(
         const JetByteTools::Win32::_tstring &threadName) const;

      // Implement IQueueTimers
      // We need to fully specify the IQueueTimers types to get around a bug in 
      // doxygen 1.5.2

      virtual IQueueTimers::Handle CreateTimer();

      virtual bool SetTimer(
         const IQueueTimers::Handle &handle, 
         IQueueTimers::Timer &timer,
         const Milliseconds timeout,
         const IQueueTimers::UserData userData);

      virtual bool CancelTimer(
         const IQueueTimers::Handle &handle);

      virtual bool DestroyTimer(
         IQueueTimers::Handle &handle);

      virtual bool DestroyTimer(
         const IQueueTimers::Handle &handle);

      virtual void SetTimer(
         IQueueTimers::Timer &timer,
         const Milliseconds timeout,
         const IQueueTimers::UserData userData);

      virtual Milliseconds GetMaximumTimeout() const;

      /// Called when the timer queue thread is terminated due to an exception
      /// this is a BAD situation! Override this to deal with it, log the error,
      /// etc...

      virtual void OnThreadTerminationException(
         const _tstring &message);

   private :

      Milliseconds GetNextTimeout();

      void InitiateShutdown();

      void SignalStateChange();

      // Implement IRunnable

      virtual int Run() throw();

      mutable CCriticalSection m_criticalSection;

      CAutoResetEvent m_stateChangeEvent;

      CThread m_thread;

      IManageTimerQueue *m_pTimerQueue;

      const bool m_weOwnImpl;

      const bool m_dispatchWithLock;

      volatile bool m_shutdown;

      /// No copies do not implement
      CThreadedCallbackTimerQueue(const CThreadedCallbackTimerQueue &rhs);
      /// No copies do not implement
      CThreadedCallbackTimerQueue &operator=(const CThreadedCallbackTimerQueue &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
