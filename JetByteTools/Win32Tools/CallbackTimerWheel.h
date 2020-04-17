#pragma once
#ifndef JETBYTE_TOOLS_CALLBACK_TIMER_WHEEL_INCLUDED__
#define JETBYTE_TOOLS_CALLBACK_TIMER_WHEEL_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
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

#include "JetByteTools/Admin/Types.h"

#include "IManageTimerQueue.h"

#include "IntrusiveSet.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

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

      ~CCallbackTimerWheel();

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
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_CALLBACK_TIMER_WHEEL_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerWheel.h
///////////////////////////////////////////////////////////////////////////////
