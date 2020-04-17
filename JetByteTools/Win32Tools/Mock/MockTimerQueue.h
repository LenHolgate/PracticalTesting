#pragma once
#ifndef JETBYTE_TOOLS_WIN32_MOCK_TIMER_QUEUE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MOCK_TIMER_QUEUE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "JetByteTools/TestTools/TestLog.h"

#include "JetByteTools/Win32Tools/IManageTimerQueue.h"
#include "JetByteTools/Win32Tools/AutoResetEvent.h"
#include "JetByteTools/Win32Tools/IRunnable.h"
#include "JetByteTools/Win32Tools/Thread.h"
#include "JetByteTools/Win32Tools/ReentrantLockableObject.h"

#include <list>
#include <map>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
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

            ~TimerExpiryThread() = default;

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

      bool logSetTimerEvenIfNotSet;

      void NameTimer(
         const Handle &handle,
         const _tstring &name);

      _tstring GetHandleAsString(
         Handle handle) const;

      void PopTimer();

      void OnTimer();

      bool IsTimerSet() const;

      void SetNextTimeout(
         Milliseconds nextTimeout);

      bool WaitForNextTimeout(
         Milliseconds timeout) const;

      bool WaitForOnTimer(
         Milliseconds timeout);

      bool WaitForSetTimer(
         Milliseconds timeout);

      // Implement IManageTimerQueue

      Milliseconds GetNextTimeout() override;

      bool BeginTimeoutHandling() override;

      void HandleTimeout() override;

      void EndTimeoutHandling() override;

      // Implement IQueueTimers

      Handle CreateTimer() override;

      bool TimerIsSet(
         const Handle &handle) const override;

      bool SetTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         SetTimerIf setTimerIf = SetTimerAlways) override;

      bool UpdateTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr) override;

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

      CAutoResetEvent m_onSetTimerEvent;

      long m_nextTimer;

      struct TimerDetails
      {
         TimerDetails(
            const Handle &handle_,
            Timer &timer_,
            const Milliseconds timeout_,
            const UserData userData_)
            :  handle(handle_),
               timer(timer_),
               timeout(timeout_),
               userData(userData_)
         {
         }

         const Handle handle;

         Timer &timer;

         Milliseconds timeout;

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
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MOCK_TIMER_QUEUE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
