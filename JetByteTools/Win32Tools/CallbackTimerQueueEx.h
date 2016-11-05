#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_EX_INCLUDED__
#define JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_EX_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueEx.h
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

#include "IQueueTimers.h"

#include <map>

///////////////////////////////////////////////////////////////////////////////
// Requires Windows Vista or later due to use of GetTickCount64()
///////////////////////////////////////////////////////////////////////////////

#if (_WIN32_WINNT >= 0x0600) 

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IProvideTickCount64;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx
///////////////////////////////////////////////////////////////////////////////

/// A class that manages a group of timers that implement IQueueTimers::Timer 
/// and which have their IQueueTimers::Timer::OnTimer() method called when the 
/// timer expires. You must manually manage the handling and processing of 
/// timeouts by calling HandleTimeouts() every GetNextTimeout() milliseconds.
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

class CCallbackTimerQueueEx : public IQueueTimers
{
   public :

      /// Create a timer queue.

      CCallbackTimerQueueEx();

      /// Create a timer queue that uses the provdided instance of 
      /// IProvideTickCount64 to obtain its tick counts rather than getting
      /// them directly from the system.
      
      explicit CCallbackTimerQueueEx(
         const IProvideTickCount64 &tickProvider);

      ~CCallbackTimerQueueEx();

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

      typedef std::multimap<ULONGLONG, TimerData *> TimerQueue;

      typedef std::map<Handle, TimerQueue::iterator> HandleMap;

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

      TimerQueue m_queue;

      HandleMap m_handleMap;

      const IProvideTickCount64 &m_tickProvider;

      const Milliseconds m_maxTimeout;

		/// No copies do not implement
      CCallbackTimerQueueEx(const CCallbackTimerQueueEx &rhs);
		/// No copies do not implement
      CCallbackTimerQueueEx &operator=(const CCallbackTimerQueueEx &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Requires Windows Vista or later due to use of GetTickCount64()
///////////////////////////////////////////////////////////////////////////////

#endif // (_WIN32_WINNT >= 0x0600)

#endif // JETBYTE_TOOLS_CALLBACK_TIMER_QUEUE_EX_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueueEx.h
///////////////////////////////////////////////////////////////////////////////
