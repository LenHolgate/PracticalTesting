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

#include "CallbackTimerQueue.h"
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
/// timer expires. The class uses a CCallbackTimerQueue object to manage the
/// timers and then manages its own timeouts using a thread to call 
/// CCallbackTimerQueue::HandleTimeouts() every GetNextTimeout() milliseconds. 
/// See <a href="http://www.lenholgate.com/archives/000381.html">here</a> for
/// more details.
/// \ingroup Timers
/// \ingroup Threading

class CThreadedCallbackTimerQueue : 
   public IQueueTimers,
   private IRunnable
{
   public :

      /// Create a timer queue.
      CThreadedCallbackTimerQueue();

      /// Create a timer queue with the specified maximum timeout value.
      explicit CThreadedCallbackTimerQueue(
         const Milliseconds maxTimeout);

      /// Create a timer queue that uses the provdided instance of 
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system.
      explicit CThreadedCallbackTimerQueue(
         const IProvideTickCount &tickProvider);

      /// Create a timer queue with the specified maximum timeout value
      /// and that uses the provdided instance of IProvideTickCount to 
      /// obtain its tick counts rather than getting them directly from 
      /// the system.
      CThreadedCallbackTimerQueue(
         const Milliseconds maxTimeout,
         const IProvideTickCount &tickProvider);

      ~CThreadedCallbackTimerQueue();

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

      CCallbackTimerQueue m_timerQueue;

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
