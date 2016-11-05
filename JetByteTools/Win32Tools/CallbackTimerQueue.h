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

class CCallbackTimerQueue : public IQueueTimers
{
   public :

      CCallbackTimerQueue();

      explicit CCallbackTimerQueue(
         const DWORD maxTimeout);

      explicit CCallbackTimerQueue(
         const IProvideTickCount &tickProvider);

      CCallbackTimerQueue(
         const DWORD maxTimeout,
         const IProvideTickCount &tickProvider);

      ~CCallbackTimerQueue();

      DWORD GetNextTimeout() const;

      void HandleTimeouts();

      // Implement IQueueTimers

      virtual Handle CreateTimer();

      virtual bool SetTimer(
         const Handle &handle, 
         Timer &timer,
         const DWORD timeoutMillis,
         const UserData userData);

      virtual bool CancelTimer(
         const Handle &handle);

      virtual bool DestroyTimer(
         Handle &handle);

      virtual void SetTimer(
         Timer &timer,
         const DWORD timeoutMillis,
         const UserData userData);

   private :

      class TimerData;

      typedef std::multimap<DWORD, TimerData *> TimerQueue;

      typedef std::pair<TimerQueue *, TimerQueue::iterator> HandleMapValue;

      typedef std::map<Handle, HandleMapValue> HandleMap;

      HandleMap::iterator ValidateHandle(
         const Handle &handle);

      bool CancelTimer(
         const Handle &handle,
         const HandleMap::iterator &it);

      DWORD GetAbsoluteTimeout(
         const DWORD timeoutMillis,
         bool &wrapped) const;

      void InsertTimer(
         const Handle &handle,
         TimerData * const pData,
         const DWORD timeoutMillis,
         const bool wrapped);

      void MarkHandleUnset(
         Handle handle);

      TimerQueue m_queue1;
      TimerQueue m_queue2;

      TimerQueue *m_pCurrentQueue;
      TimerQueue *m_pWrappedQueue;

      HandleMap m_handleMap;

      const IProvideTickCount &m_tickProvider;

      const DWORD m_maxTimeout;

      // No copies do not implement
      CCallbackTimerQueue(const CCallbackTimerQueue &rhs);
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
