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

static const DWORD s_tickCountMax = 0xFFFFFFFF;

static const DWORD s_timeoutMax = s_tickCountMax / 4 * 3;

///////////////////////////////////////////////////////////////////////////////
// Static members
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::Handle CCallbackTimerQueue::InvalidHandleValue = 0;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  m_tickProvider(tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2)
{   
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const DWORD maxTimeout,
   const IProvideTickCount &tickProvider)
   :  m_tickProvider(tickProvider),
      m_maxTimeout(maxTimeout),
      m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2)
{   
}

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_tickProvider(s_tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2)
{
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const DWORD maxTimeout)
   :  m_tickProvider(s_tickProvider),
      m_maxTimeout(maxTimeout),
      m_pCurrentQueue(&m_queue1),
      m_pWrappedQueue(&m_queue2)
{
}

CCallbackTimerQueue::~CCallbackTimerQueue()
{
   for (HandleMap::iterator it = m_handleMap.begin(); it != m_handleMap.end(); ++it)
   {
      TimerData *pData = reinterpret_cast<TimerData*>(it->first);

      delete pData;
   }
}

CCallbackTimerQueue::Handle CCallbackTimerQueue::CreateTimer()
{
   TimerData *pData = new TimerData();

   Handle handle = reinterpret_cast<Handle>(pData);

   MarkHandleUnset(handle);

   return handle;
}

bool CCallbackTimerQueue::SetTimer(
   const Handle &handle, 
   Timer &timer,
   const DWORD timeoutMillis,
   const UserData userData)
{
   bool wrapped = false;

   const DWORD timeout = GetAbsoluteTimeout(timeoutMillis, wrapped);

   HandleMap::iterator it = ValidateHandle(handle);

   const bool wasPending = CancelTimer(handle, it);

   TimerData *pData = reinterpret_cast<TimerData*>(it->first);

   pData->UpdateData(timer, userData);

   InsertTimer(handle, pData, timeout, wrapped);

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

void CCallbackTimerQueue::SetTimer(
   Timer &timer,
   const DWORD timeoutMillis,
   const UserData userData)
{
   bool wrapped = false;

   const DWORD timeout = GetAbsoluteTimeout(timeoutMillis, wrapped);

   TimerData *pData = new TimerData(timer, userData);

   Handle handle = reinterpret_cast<Handle>(pData);

   InsertTimer(handle, pData, timeout, wrapped);
}


DWORD CCallbackTimerQueue::GetAbsoluteTimeout(
   const DWORD timeoutMillis,
   bool &wrapped) const
{
   if (timeoutMillis > m_maxTimeout)
   {
      throw CException(_T("CCallbackTimerQueue::GetAbsoluteTimeout()"), _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   const DWORD now = m_tickProvider.GetTickCount();

   const DWORD timeout = now + timeoutMillis;

   wrapped = (timeout < now);

   return timeout;
}

void CCallbackTimerQueue::InsertTimer(
   const Handle &handle,
   TimerData * const pData,
   const DWORD timeoutMillis,
   const bool wrapped)
{
   TimerQueue *pQueue = wrapped ? m_pWrappedQueue : m_pCurrentQueue;         

   TimerQueue::iterator it = pQueue->insert(TimerQueue::value_type(timeoutMillis, pData));

   m_handleMap[handle] = HandleMapValue(pQueue, it);
}

CCallbackTimerQueue::HandleMap::iterator CCallbackTimerQueue::ValidateHandle(
   const Handle &handle)
{
   HandleMap::iterator it = m_handleMap.find(handle);

   if (it == m_handleMap.end())
   {
      throw CException(_T("CCallbackTimerQueue::ValidateHandle()"), _T("Invalid timer handle: ") + ToString(handle));
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

DWORD CCallbackTimerQueue::GetNextTimeout() const
{
   bool haveTimer = true;

   DWORD timeUntilTimeout = INFINITE;

   TimerQueue::const_iterator it = m_pCurrentQueue->begin();

   if (it == m_pCurrentQueue->end())
   {
      it = m_pWrappedQueue->begin();

      if (it == m_pWrappedQueue->end())
      {
         haveTimer = false;
      }
   }

   if (haveTimer)
   {
      const DWORD now = m_tickProvider.GetTickCount();

      const DWORD timeout = it->first;

      timeUntilTimeout = timeout - now;

      if (timeUntilTimeout > s_timeoutMax)
      {
         timeUntilTimeout = 0;
      }
   }  

   return timeUntilTimeout;
}

void CCallbackTimerQueue::HandleTimeouts()
{
   while (0 == GetNextTimeout())
   {
      TimerQueue::iterator it = m_pCurrentQueue->begin();
      
      if (it == m_pCurrentQueue->end())
      {
         std::swap(m_pCurrentQueue, m_pWrappedQueue);

         it = m_pCurrentQueue->begin();
      }

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
