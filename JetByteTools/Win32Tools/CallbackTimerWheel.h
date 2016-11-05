#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

#include "JetByteTools\Admin\Types.h"

#include "IManageTimerQueue.h"

#include <WTypes.h>

#include <set>

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
         const Milliseconds maximumTimeout);

      explicit CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         const Milliseconds maximumTimeout);

      CCallbackTimerWheel(
         const Milliseconds maximumTimeout,
         const Milliseconds timerGranularity);

      CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         const Milliseconds maximumTimeout,
         const Milliseconds timerGranularity);

      CCallbackTimerWheel(
         const Milliseconds maximumTimeout,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         const Milliseconds maximumTimeout,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         const Milliseconds maximumTimeout,
         const Milliseconds timerGranularity,
         const IProvideTickCount &tickCountProvider);

      CCallbackTimerWheel(
         IMonitorCallbackTimerQueue &monitor,
         const Milliseconds maximumTimeout,
         const Milliseconds timerGranularity,
         const IProvideTickCount &tickCountProvider);

      ~CCallbackTimerWheel();

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

   private :

      class TimerData;

      Milliseconds CalculateTimeout(
         const Milliseconds timeout);

      Handle OnTimerCreated(
         TimerData *pData);

      void InsertTimer(
         const Milliseconds timeout,
         TimerData &data,
         const bool wasPending = false);

      static TimerData **CreateTimerWheel(
         const size_t numTimers);

      TimerData &ValidateHandle(
         const Handle &handle) const;

      TimerData **GetFirstTimerSet() const;

      void OnTimerCancelled();

      TimerData *GetTimersToProcess(
         const Milliseconds now);

      TimerData *GetAllTimersToProcess(
         const Milliseconds now);

      TimerData **GetTimerAtOffset(
         const size_t offset) const;

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

      typedef std::set<TimerData *> Handles;

      Handles m_handles;

      TimeoutHandle m_handlingTimeouts;
      
      /// No copies do not implement
      CCallbackTimerWheel(const CCallbackTimerWheel &rhs);
      /// No copies do not implement
      CCallbackTimerWheel &operator=(const CCallbackTimerWheel &rhs);
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
