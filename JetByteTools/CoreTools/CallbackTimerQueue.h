#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
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

#include "IManageTimerQueue.h"

#include "IntrusiveMultiMap.h"
#include "IntrusiveSet.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IProvideTickCount64;
class IMonitorCallbackTimerQueue;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// A class that manages a group of timers that implement IQueueTimers::Timer
/// and which have their IQueueTimers::Timer::OnTimer() method called when the
/// timer expires. You must manually manage the handling and processing of
/// timeouts by calling IManageTimerQueue::BeginTimeoutHandling() every
/// IManageTimerQueue::GetNextTimeout() milliseconds.
/// See <a href="http://www.lenholgate.com/archives/000342.html">here</a> for
/// more details.
/// Note: the maximum timeout that you can set is 4294967294ms as 0xFFFFFFF is
/// reserved as 'INFINITE' i.e. a timer that never expires. Internally the code
/// uses an unsigned 64 bit counter which will wrap in around 584942417.4 years
/// from the creation of the timer queue. You cannot set a timer that crosses
/// this wrap point and an exception will be thrown. GetMaximumTimeout() does
/// not report the reducing maximum timeout as the wrap point approaches, it
/// will always return 4294967294ms.
/// \ingroup Timers

class CCallbackTimerQueue : public IManageTimerQueue
{
   public :

      /// Create a timer queue.

      CCallbackTimerQueue();

      /// Create a timer queue and monitor it with the supplied monitor.

      explicit CCallbackTimerQueue(
         IMonitorCallbackTimerQueue &monitor);

      /// Create a timer queue that uses the provdided instance of
      /// IProvideTickCount64 to obtain its tick counts rather than getting
      /// them directly from the system.

      explicit CCallbackTimerQueue(
         const IProvideTickCount64 &tickProvider);

      /// Create a timer queue that uses the provdided instance of
      /// IProvideTickCount64 to obtain its tick counts rather than getting
      /// them directly from the system. Monitor it with the supplied monitor.

      CCallbackTimerQueue(
         IMonitorCallbackTimerQueue &monitor,
         const IProvideTickCount64 &tickProvider);

      CCallbackTimerQueue(
         const CCallbackTimerQueue &rhs) = delete;

      ~CCallbackTimerQueue() override;

      CCallbackTimerQueue &operator=(
         const CCallbackTimerQueue &rhs) = delete;

      // Implement IManageTimerQueue

      Milliseconds GetNextTimeout() override;

      bool BeginTimeoutHandling() override;

      void HandleTimeout() override;

      void EndTimeoutHandling() override;

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
         std::less<>,
         TimerDataIntrusiveMultiMapNodeAccessor> TimerQueue;

      typedef TIntrusiveSet<TimerData> ActiveHandles;

      TimerData *ValidateHandle(
         const Handle &handle) const;

      bool CancelTimer(
         TimerData *pData);

      void InsertTimer(
         TimerData *pData,
         Milliseconds timeout);

      void InsertTimer(
         TimerData *pData,
         ULONGLONG absoluteTimeout);

      TimerQueue m_queue;

      ActiveHandles m_activeHandles;

      const IProvideTickCount64 &m_tickProvider;

      IMonitorCallbackTimerQueue &m_monitor;

      const Milliseconds m_maxTimeout;

      bool m_handlingTimeouts;

      TimerData *m_pTimeoutsToBeHandled;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
