#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_INCLUDED__
#define JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueue.h
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

#include "IManageTimerQueue.h"

#include <map>
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

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// A class that manages a group of timers that implement IQueueTimers::Timer 
/// and which have their IQueueTimers::Timer::OnTimer() method called when the 
/// timer expires. You must manually manage the handling and processing of 
/// timeouts by calling either IManageTimerQueue::HandleTimeouts() or 
/// IManageTimerQueue::BeginTimeoutHandling() every 
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

      /// Create a timer queue that uses the provdided instance of 
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system.
      
      explicit CCallbackTimerQueue(
         const IProvideTickCount &tickProvider);

      virtual ~CCallbackTimerQueue();

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

   private :

      class TimerData;

      typedef std::multimap<ULONGLONG, TimerData *> TimerQueue;

      typedef std::map<Handle, TimerQueue::iterator> HandleMap;

      typedef std::set<TimeoutHandle> TimeoutHandles;

      HandleMap::iterator ValidateHandle(
         const Handle &handle);

      bool CancelTimer(
         const Handle &handle,
         const HandleMap::iterator &it);

      void InsertTimer(
         const Handle &handle,
         TimerData * const pData,
         const Milliseconds timeout);

      void MarkHandleUnset(
         Handle handle);

      void SetMaintenanceTimer();

      ULONGLONG GetTickCount64();

      TimeoutHandle GetTimeoutHandle(
         TimerData *pData);

      TimerData *ValidateTimeoutHandle(
         IManageTimerQueue::TimeoutHandle &handle);

      TimerData *EraseTimeoutHandle(
         IManageTimerQueue::TimeoutHandle &handle);

      TimerQueue m_queue;

      HandleMap m_handleMap;

      TimeoutHandles m_timeoutHandles;

      const IProvideTickCount &m_tickProvider;

      const Milliseconds m_maxTimeout;

      LARGE_INTEGER m_lastCount;

      Handle m_maintenanceTimer;

      class MaintenanceTimerHandler : public IQueueTimers::Timer
      {
         // Implement IQueueTimers::Timer 

         virtual void OnTimer(
            IQueueTimers::Timer::UserData userData);
      };

      friend class MaintenanceTimerHandler;

      MaintenanceTimerHandler m_maintenanceTimerHandler;

      bool m_handlingTimeouts;

		/// No copies do not implement
      CCallbackTimerQueue(const CCallbackTimerQueue &rhs);
		/// No copies do not implement
      CCallbackTimerQueue &operator=(const CCallbackTimerQueue &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
