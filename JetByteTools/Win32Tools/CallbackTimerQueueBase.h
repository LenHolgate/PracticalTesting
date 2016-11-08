#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_BASE_INCLUDED__
#define JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_BASE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueBase.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
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

#include "IManageTimerQueue.h"

#include "IntrusiveMultiMap.h"
#include "IntrusiveSet.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IMonitorCallbackTimerQueue;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueBase
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueueBase : public IManageTimerQueue
{
   public :

      // Implement IManageTimerQueue

      virtual Milliseconds GetNextTimeout();

      virtual void HandleTimeouts();

      virtual IManageTimerQueue::TimeoutHandle BeginTimeoutHandling();

      virtual void HandleTimeout(
         IManageTimerQueue::TimeoutHandle &handle);

      virtual void EndTimeoutHandling(
         IManageTimerQueue::TimeoutHandle &handle);

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

      virtual bool DispatchesWithoutLock() const;

   protected :

      /// Create a timer queue.

      CCallbackTimerQueueBase();

      explicit CCallbackTimerQueueBase(
         IMonitorCallbackTimerQueue &monitor);

      virtual ~CCallbackTimerQueueBase();

      bool SetInternalTimer(
         const IQueueTimers::Handle &handle,
         IQueueTimers::Timer &timer,
         const ULONGLONG absoluteTimeout);

   private :

      class TimerDataSetNodeAccessor;

      class TimerData;

      class TimerDataIntrusiveMultiMapNodeKeyAccessor
      {
         public :

            static ULONGLONG GetKeyFromT(
               const TimerData *pNode);
      };

      class TimerDataIntrusiveMultiMapNodeAccessor
      {
         public :

            static CIntrusiveMultiMapNode * GetNodeFromT(
               const TimerData *pData);

            static TimerData *GetTFromNode(
               const CIntrusiveMultiMapNode *pNode);

            static TimerData *GetTFromNode(
               const CIntrusiveRedBlackTreeNode *pNode);
      };

      typedef TIntrusiveMultiMap<
         TimerData,
         ULONGLONG,
         TimerDataIntrusiveMultiMapNodeKeyAccessor,
         std::less<ULONGLONG>,
         TimerDataIntrusiveMultiMapNodeAccessor> TimerQueue;

      typedef TIntrusiveSet<TimerData> ActiveHandles;

      TimerData *ValidateHandle(
         const Handle &handle);

      TimerData *CreateTimerInternal();

      TimeoutHandle GetNextTimeoutHandle();

      //lint -e{1411} (Member with different signature hides virtual member --- Eff. C++ 3rd Ed. item 33)
      bool CancelTimer(
         TimerData *pData);

      void InsertTimer(
         TimerData * const pData,
         const Milliseconds timeout);

      void InsertTimer(
         TimerData * const pData,
         const ULONGLONG absoluteTimeout);

      virtual ULONGLONG GetTickCount64() = 0;

      TimerQueue m_queue;

      ActiveHandles m_activeHandles;

      IMonitorCallbackTimerQueue &m_monitor;

      const Milliseconds m_maxTimeout;

      TimeoutHandle m_handlingTimeouts;

      TimeoutHandle m_nextTimeoutHandle;

      TimerData *m_pTimeoutsToBeHandled;

      /// No copies do not implement
      CCallbackTimerQueueBase(const CCallbackTimerQueueBase &rhs);
      /// No copies do not implement
      CCallbackTimerQueueBase &operator=(const CCallbackTimerQueueBase &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_BASE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueBase.h
///////////////////////////////////////////////////////////////////////////////
