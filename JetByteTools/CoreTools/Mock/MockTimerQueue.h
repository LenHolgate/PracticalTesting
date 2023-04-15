#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "JetByteTools/TestTools/TestLog.h"

#include "JetByteTools/CoreTools/IManageTimerQueue.h"
#include "JetByteTools/CoreTools/AutoResetEvent.h"
#include "JetByteTools/CoreTools/AtomicLong.h"
#include "JetByteTools/CoreTools/IRunnable.h"
#include "JetByteTools/CoreTools/Thread.h"
#include "JetByteTools/CoreTools/ReentrantLockableObject.h"

#include <list>
#include <map>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// A mock object that implements IManageTimerQueue.
/// \ingroup Win32ToolsMocks

class CMockTimerQueue :
   public IManageTimerQueue,
   public JetByteTools::Test::CTestLog
{
   public :

      class TimerExpiryThread : private IRunnable
      {
         public :

            explicit TimerExpiryThread(
               CMockTimerQueue &timerQueue);

            TimerExpiryThread(
               const TimerExpiryThread &rhs) = delete;

            ~TimerExpiryThread() override = default;

            TimerExpiryThread &operator=(
               const TimerExpiryThread &rhs) = delete;

            void WaitForCompletion() const;

         private :

            // Implement IRunnable

            unsigned int Run() override;

            CThread m_thread;

            CMockTimerQueue &m_timerQueue;
      };

      CMockTimerQueue();

      explicit CMockTimerQueue(
         CTestLog *pLinkedLog);

      CMockTimerQueue(
         const CMockTimerQueue &rhs) = delete;

      CMockTimerQueue &operator=(
         const CMockTimerQueue &rhs) = delete;

      bool waitForOnTimerWaitComplete;

      bool includeHandleValuesInLogs;

      void OnTimer();

      bool IsTimerSet() const;

      void SetNextTimeout(
         Milliseconds nextTimeout);

      bool WaitForNextTimeout(
         Milliseconds timeout) const;

      bool WaitForOnTimer(
         Milliseconds timeout);

      // Implement IManageTimerQueue

      Milliseconds GetNextTimeout() override;

      bool BeginTimeoutHandling() override;

      void HandleTimeout() override;

      void EndTimeoutHandling() override;

      // Implement IQueueTimers

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

   private :

      mutable CReentrantLockableObject m_lock;

      CAutoResetEvent m_nextTimeoutEvent;

      CAutoResetEvent m_onTimerEvent;

      CAutoResetEvent m_onTimerWaitCompleteEvent;

      CAtomicLong m_nextTimer;

      struct TimerDetails
      {
         TimerDetails(
            const Handle &handle_,
            Timer &timer_,
            UserData userData_)
            :  handle(handle_),
               timer(timer_),
               userData(userData_)
         {
         }

         const Handle handle;

         Timer &timer;

         UserData userData;
      };

      typedef std::list<TimerDetails> SetTimers;

      SetTimers m_setTimers;

      const Milliseconds m_maxTimeout;

      Milliseconds m_nextTimeout;

      typedef std::map<Handle, _tstring> TimerNames;

      TimerNames m_timerNames;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
