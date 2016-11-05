#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include "JetByteTools\TestTools\TestLog.h"

#include "..\IManageTimerQueue.h"
#include "..\AutoResetEvent.h"

#include <list>

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

      explicit CMockTimerQueue(
         const bool dispatchWithoutLock = true);

      bool waitForOnTimerWaitComplete;

      bool includeHandleValuesInLogs;

      void OnTimer();

      bool IsTimerSet() const;

      void SetNextTimeout(
         const Milliseconds nextTimeout);

      bool WaitForNextTimeout(
         const Milliseconds timeout);

      bool WaitForOnTimer(
         const Milliseconds timeout);

      // Implement IManageTimerQueue

      virtual Milliseconds GetNextTimeout();

      virtual void HandleTimeouts();

      virtual IManageTimerQueue::TimeoutHandle BeginTimeoutHandling();

      virtual void HandleTimeout(
         IManageTimerQueue::TimeoutHandle &handle);

      virtual void EndTimeoutHandling(
         IManageTimerQueue::TimeoutHandle &handle);

      // Implement IQueueTimers

      virtual Handle CreateTimer();

      virtual bool SetTimer(
         const Handle &handle,
         Timer &timer,
         const Milliseconds timeout,
         const UserData userData);

      virtual bool CancelTimer(
         const Handle &handle);

      virtual bool DestroyTimer(
         Handle &handle);

      virtual bool DestroyTimer(
         const Handle &handle);

      virtual void SetTimer(
         Timer &timer,
         const Milliseconds timeout,
         const UserData userData);

      virtual Milliseconds GetMaximumTimeout() const;

      virtual bool DispatchesWithoutLock() const;

   private :

      const bool m_dispatchWithoutLock;

      CAutoResetEvent m_nextTimeoutEvent;

      CAutoResetEvent m_onTimerEvent;

      CAutoResetEvent m_onTimerWaitCompleteEvent;

      long m_nextTimer;

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

      /// No copies do not implement
      CMockTimerQueue(const CMockTimerQueue &rhs);
      /// No copies do not implement
      CMockTimerQueue &operator=(const CMockTimerQueue &rhs);
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
