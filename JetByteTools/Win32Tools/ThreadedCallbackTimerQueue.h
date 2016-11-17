#pragma once
#ifndef JETBYTE_TOOLS_WIN32_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
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
#include "IProvideTickCount64.h"
#include "Thread.h"
#include "IRunnable.h"
#include "AutoResetEvent.h"
#include "ReentrantLockableObject.h"
#include "ConditionalSmartPointer.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IMonitorThreadedCallbackTimerQueue;

///////////////////////////////////////////////////////////////////////////////
// CThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// A class that manages a group of timers that implement IQueueTimers::Timer
/// and which have their IQueueTimers::Timer::OnTimer() method called when the
/// timer expires. The class uses an implementation of IManageTimerQueue to manage
/// the timers and then manages its own timeouts using a thread to call
/// IManageTimerQueue::BeginTimeoutHandling() every GetNextTimeout() milliseconds.
/// You can configure it to use CCallbackTimerQueueEx or
/// supply your own implementation of IManageTimerQueue.
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

      /// Create a timer queue and monitor it with the supplied monitor.

      explicit CThreadedCallbackTimerQueue(
         IMonitorThreadedCallbackTimerQueue &monitor);

      /// Create a timer queue that uses the provdided instance of IProvideTickCount64 to
      /// obtain its tick counts rather than getting them directly from the system.

      explicit CThreadedCallbackTimerQueue(
         const IProvideTickCount64 &tickProvider);

      /// Create a timer queue that uses the provdided instance of IProvideTickCount64 to
      /// obtain its tick counts rather than getting them directly from the system.
      /// Monitor it with the supplied monitor.

      CThreadedCallbackTimerQueue(
         IMonitorThreadedCallbackTimerQueue &monitor,
         const IProvideTickCount64 &tickProvider);

      /// Create a timer queue that uses the supplied instance of IManageTimerQueue as its
      /// implementation. Note that we don't take ownership of the implementation, it's up
      /// to you to manage its lifetime.

      explicit CThreadedCallbackTimerQueue(
         IManageTimerQueue &impl);

      /// Create a timer queue that uses the supplied instance of IManageTimerQueue as its
      /// implementation. Note that we don't take ownership of the implementation, it's up
      /// to you to manage its lifetime. Monitor it with the supplied monitor.

      CThreadedCallbackTimerQueue(
         IManageTimerQueue &impl,
         IMonitorThreadedCallbackTimerQueue &monitor);

      ~CThreadedCallbackTimerQueue();

      /// Sets the name of the timer queue thread as displayed in the Visual Studio debugger
      /// to the supplied name. By default the constructors set the name of the thread
      /// to "TimerQueue".

      void SetThreadName(
         const JetByteTools::Win32::_tstring &threadName) const;

      /// Starts the shutdown process and returns immediately.

      void BeginShutdown();

      /// Initiates a shutdown (if one isn't already in progresss) and then waits
      /// for it to complete. Does not return until the shutdown has completed or
      /// the timeout has expired. Returns true if the shutdown is complete.

      //lint -esym(534, JetByteTools::Win32::CThreadedCallbackTimerQueue::WaitForShutdownToComplete) (Ignoring return value of function)
      bool WaitForShutdownToComplete(
         const Milliseconds timeout = INFINITE);

      // Implement IQueueTimers
      // We need to fully specify the IQueueTimers types to get around a bug in
      // doxygen 1.5.2

      IQueueTimers::Handle CreateTimer() override;

      bool SetTimer(
         const IQueueTimers::Handle &handle,
         IQueueTimers::Timer &timer,
         const Milliseconds timeout,
         const IQueueTimers::UserData userData) override;

      bool CancelTimer(
         const IQueueTimers::Handle &handle) override;

      bool DestroyTimer(
         IQueueTimers::Handle &handle) override;

      bool DestroyTimer(
         const IQueueTimers::Handle &handle) override;

      void SetTimer(
         IQueueTimers::Timer &timer,
         const Milliseconds timeout,
         const IQueueTimers::UserData userData) override;

      Milliseconds GetMaximumTimeout() const override;

      /// Called when the timer queue thread is terminated due to an exception
      /// this is a BAD situation! Override this to deal with it, log the error,
      /// etc...

      virtual void OnThreadTerminationException(
         const _tstring &message);

      /// Called on the timer thread when the timer thread is started

      virtual bool OnThreadInitialised();

      /// Called on the timer thread when the thread is shutting down

      virtual void OnThreadShutdown();

   private :

      Milliseconds GetNextTimeout();

      bool BeginTimeoutHandling();

      void SignalStateChange();

      // Implement IRunnable

      unsigned int Run() override;

      IMonitorThreadedCallbackTimerQueue &m_monitor;

      mutable CReentrantLockableObject m_lock;

      CAutoResetEvent m_stateChangeEvent;

      CThread m_thread;

      TConditionalSmartPointer<IManageTimerQueue> m_spTimerQueue;

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

#endif // JETBYTE_TOOLS_WIN32_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
