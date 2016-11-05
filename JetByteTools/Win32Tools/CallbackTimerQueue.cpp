///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueue.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "CallbackTimerQueue.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCountProvider.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueue::TimerData
{
   public :

      TimerData();

      TimerData(
         Timer &timer,
         UserData userData);

      void UpdateData(
         Timer &timer,
         UserData userData);

      void OnTimer();

      bool IsOneShotTimer() const;

   private :
   
      Timer *m_pTimer;
      
      UserData m_userData;

      const bool m_oneShotTimer;
};

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const CTickCountProvider s_tickProvider;

static const Milliseconds s_tickCountMax = 0xFFFFFFFF;

static const Milliseconds s_timeoutMax = (s_tickCountMax / 4) * 3;

///////////////////////////////////////////////////////////////////////////////
// Static members
///////////////////////////////////////////////////////////////////////////////

#pragma TODO("Move to its own file?")

IQueueTimers::Handle IQueueTimers::InvalidHandleValue = 0;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2),
      m_tickProvider(tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_lastWrappedTimerSetTicks(0)
{   
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const Milliseconds maxTimeout,
   const IProvideTickCount &tickProvider)
   :  m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2),
      m_tickProvider(tickProvider),
      m_maxTimeout(maxTimeout),
      m_lastWrappedTimerSetTicks(0)
{   
}

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2),
      m_tickProvider(s_tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_lastWrappedTimerSetTicks(0)
{
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const Milliseconds maxTimeout)
   :  m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2),
      m_tickProvider(s_tickProvider),
      m_maxTimeout(maxTimeout),
      m_lastWrappedTimerSetTicks(0)
{
}

CCallbackTimerQueue::~CCallbackTimerQueue()
{
   try
   {
      for (HandleMap::iterator it = m_handleMap.begin(); it != m_handleMap.end(); ++it)
      {
         TimerData *pData = reinterpret_cast<TimerData*>(it->first);

         delete pData;
      }
   }
   catch(...)
   {
#pragma TODO("what exception should we be catching here???")
   }

   m_pCurrentQueue = 0;
   m_pWrappedQueue = 0;
}

CCallbackTimerQueue::Handle CCallbackTimerQueue::CreateTimer()
{
   TimerData *pData = new TimerData();

   Handle handle = reinterpret_cast<Handle>(pData);

   MarkHandleUnset(handle);

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerQueue::SetTimer(
   const Handle &handle, 
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   HandleTimeouts();

   const Milliseconds now = m_tickProvider.GetTickCount();

   const Milliseconds absoluteTimeout = GetAbsoluteTimeout(timeout, now);

   HandleMap::iterator it = ValidateHandle(handle);

   const bool wasPending = CancelTimer(handle, it);

   TimerData *pData = reinterpret_cast<TimerData*>(it->first);

   pData->UpdateData(timer, userData);

   InsertTimer(handle, pData, absoluteTimeout, now);

   return wasPending;
}

bool CCallbackTimerQueue::CancelTimer(
   const Handle &handle)
{
   return CancelTimer(handle, ValidateHandle(handle));
}

bool CCallbackTimerQueue::DestroyTimer(
   Handle &handle)
{
   HandleMap::iterator it = ValidateHandle(handle);

   TimerData *pData = reinterpret_cast<TimerData*>(it->first);

   const bool wasPending = CancelTimer(handle, it);

   m_handleMap.erase(it);

   delete pData;

   handle = InvalidHandleValue;

   return wasPending;
}

bool CCallbackTimerQueue::DestroyTimer(
   const Handle &handle)
{
   Handle handle_ = handle;

   return DestroyTimer(handle_);
}

void CCallbackTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   HandleTimeouts();

   const Milliseconds now = m_tickProvider.GetTickCount();

   const Milliseconds absoluteTimeout = GetAbsoluteTimeout(timeout, now);

   TimerData *pData = new TimerData(timer, userData);

   Handle handle = reinterpret_cast<Handle>(pData);

   InsertTimer(handle, pData, absoluteTimeout, now);
}

Milliseconds CCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

Milliseconds CCallbackTimerQueue::GetAbsoluteTimeout(
   const Milliseconds timeout,
   const Milliseconds now) const
{
   if (timeout > m_maxTimeout)
   {
      throw CException(_T("CCallbackTimerQueue::GetAbsoluteTimeout()"), _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   const Milliseconds absoluteTimeout = now + timeout;

   return absoluteTimeout;
}

void CCallbackTimerQueue::InsertTimer(
   const Handle &handle,
   TimerData * const pData,
   const Milliseconds timeout,
   const Milliseconds now)
{
   TimerQueue *pQueue = m_pCurrentQueue;         

   const bool wrapped = (timeout < now);

   if (wrapped)
   {
      pQueue = m_pWrappedQueue;

      m_lastWrappedTimerSetTicks = now;
   }

   TimerQueue::iterator it = pQueue->insert(TimerQueue::value_type(timeout, pData));

   m_handleMap[handle] = HandleMapValue(pQueue, it);
}

CCallbackTimerQueue::HandleMap::iterator CCallbackTimerQueue::ValidateHandle(
   const Handle &handle)
{
   HandleMap::iterator it = m_handleMap.find(handle);

   if (it == m_handleMap.end())
   {
// The following warning is generated when /Wp64 is set in a 32bit build. At present I think
// it's due to some confusion, and even if it isn't then it's not that crucial...
#pragma warning(push, 4)
#pragma warning(disable: 4244)
      throw CException(_T("CCallbackTimerQueue::ValidateHandle()"), _T("Invalid timer handle: ") + ToString(handle));
#pragma warning(pop)
   }
   
   return it;
}

bool CCallbackTimerQueue::CancelTimer(
   const Handle &handle,
   const HandleMap::iterator &it)
{
   bool wasPending = false;
   
   const HandleMapValue &value = it->second;

   TimerQueue *pQueue = value.first;

   if (pQueue)
   {
      pQueue->erase(value.second);

      MarkHandleUnset(handle);

      wasPending = true;
   }

   return wasPending;
}

Milliseconds CCallbackTimerQueue::GetNextTimeout() 
{
   bool haveTimer = true;

   bool wrapped = false;

   Milliseconds timeUntilTimeout = INFINITE;

   TimerQueue::const_iterator it = m_pCurrentQueue->begin();

   if (it == m_pCurrentQueue->end())
   {
      wrapped = true;

      it = m_pWrappedQueue->begin();

      if (it == m_pWrappedQueue->end())
      {
         haveTimer = false;
      }
   }

   if (haveTimer)
   {
      const Milliseconds now = m_tickProvider.GetTickCount();

      const Milliseconds timeout = it->first;

      timeUntilTimeout = timeout - now;

      if (timeUntilTimeout > s_timeoutMax)
      {
         timeUntilTimeout = 0;
      }

      if (wrapped && now < m_lastWrappedTimerSetTicks)
      {
         m_lastWrappedTimerSetTicks = 0;

         std::swap(m_pCurrentQueue, m_pWrappedQueue);
      }
   }  

   return timeUntilTimeout;
}

void CCallbackTimerQueue::HandleTimeouts()
{
   while (0 == GetNextTimeout())
   {
      TimerQueue::iterator it = m_pCurrentQueue->begin();
      
      TimerData *pData = it->second;

      m_pCurrentQueue->erase(it);

      Handle handle = reinterpret_cast<Handle>(pData);

      MarkHandleUnset(handle);

      pData->OnTimer();

      if (pData->IsOneShotTimer())
      {
         DestroyTimer(handle);
      }
   }
}

void CCallbackTimerQueue::MarkHandleUnset(
   Handle handle)
{
   m_handleMap[handle] = HandleMapValue(0, m_queue1.end());
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::TimerData()
   :  m_pTimer(0), 
      m_userData(0),
      m_oneShotTimer(false)
{
}

CCallbackTimerQueue::TimerData::TimerData(
   Timer &timer,
   UserData userData)
   :  m_pTimer(&timer), 
      m_userData(userData),
      m_oneShotTimer(true)
{
}

void CCallbackTimerQueue::TimerData::UpdateData(
   Timer &timer,
   UserData userData)
{
   if (m_oneShotTimer)
   {
      throw CException(_T("CCallbackTimerQueue::UpdateData()"), _T("Internal Error: Can't update one shot timers"));
   }

   m_pTimer = &timer; 
   m_userData = userData;
}

void CCallbackTimerQueue::TimerData::OnTimer()
{
   if (!m_pTimer)
   {
      throw CException(_T("CCallbackTimerQueue::OnTimer()"), _T("Internal Error: Timer not set"));
   }

   m_pTimer->OnTimer(m_userData);
}

bool CCallbackTimerQueue::TimerData::IsOneShotTimer() const
{
   return m_oneShotTimer;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
