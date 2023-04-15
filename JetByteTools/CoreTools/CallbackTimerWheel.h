#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
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

#include "IntrusiveSet.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IProvideTickCount;
class IMonitorCallbackTimerQueue;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerWheel
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerWheel : public IManageTimerQueue
{
   public :

      explicit CCallbackTimerWheel(
         Milliseconds maximumTimeout);

      explicit CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         Milliseconds maximumTimeout);

      CCallbackTimerWheel(
         Milliseconds maximumTimeout,
         Milliseconds timerGranularity);

      CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         Milliseconds maximumTimeout,
         Milliseconds timerGranularity);

      CCallbackTimerWheel(
         Milliseconds maximumTimeout,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         Milliseconds maximumTimeout,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         Milliseconds maximumTimeout,
         Milliseconds timerGranularity,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         Milliseconds maximumTimeout,
         Milliseconds timerGranularity,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         const CCallbackTimerWheel &rhs) = delete;

      ~CCallbackTimerWheel() override;

      CCallbackTimerWheel &operator=(
         const CCallbackTimerWheel &rhs) = delete;

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

      class TimerData;

      Milliseconds CalculateTimeout(
         Milliseconds timeout);

      Handle OnTimerCreated(
         const TimerData *pData);

      void InsertTimer(
         Milliseconds timeout,
         TimerData &data,
         bool wasPending = false);

      static TimerData **CreateTimerWheel(
         size_t numTimers);

      TimerData &ValidateHandle(
         const Handle &handle) const;

      TimerData **GetFirstTimerSet() const;

      void OnTimerCancelled();

      TimerData *GetTimersToProcess(
         Milliseconds now);

      TimerData *GetAllTimersToProcess(
         Milliseconds now);

      TimerData **GetTimerAtOffset(
         size_t offset) const;

      TimerData *PrepareTimersForHandleTimeout(
         TimerData *pTimers);

      IMonitorCallbackTimerQueue &m_monitor;

      const Milliseconds m_maximumTimeout;

      const Milliseconds m_timerGranularity;

      const size_t m_numTimers;

      const IProvideTickCount &m_tickCountProvider;

      Milliseconds m_currentTime;

      TimerData **m_pTimersStart;

      TimerData **m_pTimersEnd;

      TimerData **m_pNow;

      TimerData **m_pFirstTimerSetHint;

      size_t m_numTimersSet;

      typedef TIntrusiveSet<TimerData> ActiveHandles;

      ActiveHandles m_activeHandles;

      bool m_handlingTimeouts;

      TimerData *m_pTimeoutsToBeHandled;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
