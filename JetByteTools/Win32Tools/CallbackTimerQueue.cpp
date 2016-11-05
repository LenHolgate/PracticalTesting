///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
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
         DWORD timeout,
         UserData userData);

      void UpdateData(
         Timer &timer,
         DWORD timeout,
         UserData userData);

      void OnTimer();

      bool IsBefore(
         const TimerData &rhs) const;

      DWORD TimeUntilTimeout(
         const DWORD now) const;

      bool IsOneShotTimer() const;

   private :
   
      Timer *m_pTimer;
   
      DWORD m_timeout;
   
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
      m_wrapPoint(m_queue.end()),
      m_maxTimeout(s_timeoutMax)
{   
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const DWORD maxTimeout,
   const IProvideTickCount &tickProvider)
   :  m_tickProvider(tickProvider),
      m_wrapPoint(m_queue.end()),
      m_maxTimeout(maxTimeout)
{   
}

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_tickProvider(s_tickProvider),
      m_wrapPoint(m_queue.end()),
      m_maxTimeout(s_timeoutMax)
{
}

CCallbackTimerQueue::CCallbackTimerQueue(
   const DWORD maxTimeout)
   :  m_tickProvider(s_tickProvider),
      m_wrapPoint(m_queue.end()),
      m_maxTimeout(maxTimeout)
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

   m_handleMap[handle] = m_queue.end();

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

   pData->UpdateData(timer, timeout, userData);

   InsertTimer(handle, pData, wrapped);

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

   TimerData *pData = new TimerData(timer, timeout, userData);

   Handle handle = reinterpret_cast<Handle>(pData);

   InsertTimer(handle, pData, wrapped);
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
   const bool wrapped)
{
   if (wrapped && m_wrapPoint == m_queue.end())
   {
      m_wrapPoint = m_queue.insert(m_queue.end(), 0);      
   }
   
   TimerQueue::iterator wrapPoint = m_wrapPoint;

   TimerQueue::iterator it = (wrapped ? ++wrapPoint :  m_queue.begin());
   // insert into the list in the right place

   while (it != m_queue.end() && it != m_wrapPoint && (*it)->IsBefore(*pData))
   {
      ++it;
   }

   it = m_queue.insert(it, pData);

   m_handleMap[handle] = it;
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
   
   if (it->second != m_queue.end())
   {
      m_queue.erase(it->second);

      m_handleMap[handle] = m_queue.end();

      wasPending = true;
   }

   return wasPending;
}

DWORD CCallbackTimerQueue::GetNextTimeout() const
{
   DWORD timeout = INFINITE;

   TimerQueue::const_iterator it = m_queue.begin();

   if (it != m_queue.end())
   {
      const DWORD now = m_tickProvider.GetTickCount();

      if (it == m_wrapPoint)
      {
         ++it;
      }

      TimerData *pData = *it;

      timeout = pData->TimeUntilTimeout(now);

      if (timeout > s_timeoutMax)
      {
         timeout = 0;
      }
   }  

   return timeout;
}

void CCallbackTimerQueue::HandleTimeouts()
{
   while (0 == GetNextTimeout())
   {
      TimerQueue::iterator it = m_queue.begin();
      
      if (it != m_queue.end())
      {
         if (it == m_wrapPoint)
         {  
            ++it;
          
            m_queue.erase(m_wrapPoint);

            m_wrapPoint = m_queue.end();
         }

         TimerData *pData = *it;

         m_queue.erase(it);

         Handle handle = reinterpret_cast<Handle>(pData);

         m_handleMap[handle] = m_queue.end();

         pData->OnTimer();

         if (pData->IsOneShotTimer())
         {
            DestroyTimer(handle);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::TimerData()
   :  m_pTimer(0), 
      m_timeout(0),
      m_userData(0),
      m_oneShotTimer(false)
{
}

CCallbackTimerQueue::TimerData::TimerData(
   Timer &timer,
   DWORD timeout,
   UserData userData)
   :  m_pTimer(&timer), 
      m_timeout(timeout),
      m_userData(userData),
      m_oneShotTimer(true)
{
}

void CCallbackTimerQueue::TimerData::UpdateData(
   Timer &timer,
   DWORD timeout,
   UserData userData)
{
   if (m_oneShotTimer)
   {
      throw CException(_T("CCallbackTimerQueue::UpdateData()"), _T("Internal Error: Can't update one shot timers"));
   }

   m_pTimer = &timer; 
   m_timeout = timeout;
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

bool CCallbackTimerQueue::TimerData::IsBefore(
   const TimerData &rhs) const
{
   return m_timeout < rhs.m_timeout;
}

DWORD CCallbackTimerQueue::TimerData::TimeUntilTimeout(
   const DWORD now) const
{
   return m_timeout - now;
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
