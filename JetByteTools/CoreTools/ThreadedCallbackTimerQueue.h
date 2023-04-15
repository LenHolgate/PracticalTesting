#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ThreadedCallbackTimerQueue.h
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

#include "Types.h"
#include "IManageTimerQueue.h"
#include "IProvideTickCount64.h"
#include "Thread.h"
#include "IRunnable.h"
#include "AutoResetEvent.h"
#include "ReentrantLockableObject.h"
#include "ConditionalSmartPointer.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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
/// You can configure it to use CCallbackTimerQueue or
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

      CThreadedCallbackTimerQueue(
         const CThreadedCallbackTimerQueue &rhs) = delete;

      ~CThreadedCallbackTimerQueue() override;

      CThreadedCallbackTimerQueue &operator=(
         const CThreadedCallbackTimerQueue &rhs) = delete;

      /// Sets the name of the timer queue thread as displayed in the Visual Studio debugger
      /// to the supplied name. By default the constructors set the name of the thread
      /// to "TimerQueue".

      void SetThreadName(
         const _tstring &threadName) const;

      /// Starts the shutdown process and returns immediately.

      void BeginShutdown();

      /// Initiates a shutdown (if one isn't already in progresss) and then waits
      /// for it to complete. Does not return until the shutdown has completed or
      /// the timeout has expired. Returns true if the shutdown is complete.

      bool WaitForShutdownToComplete(
         Milliseconds timeout = INFINITE);

      // Implement IQueueTimers
      // We need to fully specify the IQueueTimers types to get around a bug in
      // doxygen 1.5.2

      Handle CreateTimer() override;

      bool SetTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData) override;

      bool CancelTimer(
         const Handle &handle) override;

      bool DestroyTimer(
         Handle &handle) override;

      bool DestroyTimer(
         const Handle &handle) override;

      void SetTimer(
         Timer &timer,
         Milliseconds timeout,
         UserData userData) override;

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
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadedCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
