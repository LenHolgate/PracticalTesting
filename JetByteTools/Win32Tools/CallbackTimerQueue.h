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

#include "IQueueTimers.h"

#include <map>

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
/// timeouts by calling HandleTimeouts() every GetNextTimeout() milliseconds.
/// See <a href="http://www.lenholgate.com/archives/000342.html">here</a> for 
/// more details.
/// \ingroup Timers

class CCallbackTimerQueue : public IQueueTimers
{
   public :

      /// Create a timer queue.

      CCallbackTimerQueue();

      /// Create a timer queue with the specified maximum timeout value.
      
      explicit CCallbackTimerQueue(
         const Milliseconds maxTimeout);

      /// Create a timer queue that uses the provdided instance of 
      /// IProvideTickCount to obtain its tick counts rather than getting
      /// them directly from the system.
      
      explicit CCallbackTimerQueue(
         const IProvideTickCount &tickProvider);

      /// Create a timer queue with the specified maximum timeout value
      /// and that uses the provdided instance of IProvideTickCount to 
      /// obtain its tick counts rather than getting them directly from 
      /// the system.
      
      CCallbackTimerQueue(
         const Milliseconds maxTimeout,
         const IProvideTickCount &tickProvider);

      ~CCallbackTimerQueue();

      /// Get the number of milliseconds until the next timer is due to fire.
      /// Or INFINITE if no timer is set.
      
      Milliseconds GetNextTimeout();

      /// Process any timers that have timed out.
      
      void HandleTimeouts();

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

      typedef std::multimap<Milliseconds, TimerData *> TimerQueue;

      typedef std::pair<TimerQueue *, TimerQueue::iterator> HandleMapValue;

      typedef std::map<Handle, HandleMapValue> HandleMap;

      HandleMap::iterator ValidateHandle(
         const Handle &handle);

      bool CancelTimer(
         const Handle &handle,
         const HandleMap::iterator &it);

      Milliseconds GetAbsoluteTimeout(
         const Milliseconds timeout,
         const Milliseconds now) const;

      void InsertTimer(
         const Handle &handle,
         TimerData * const pData,
         const Milliseconds timeout,
         const Milliseconds now);

      void MarkHandleUnset(
         Handle handle);

      TimerQueue m_queue1;
      TimerQueue m_queue2;

      TimerQueue *m_pCurrentQueue;
      TimerQueue *m_pWrappedQueue;

      HandleMap m_handleMap;

      const IProvideTickCount &m_tickProvider;

      const Milliseconds m_maxTimeout;

      Milliseconds m_lastWrappedTimerSetTicks;

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
