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

#include "CallbackTimerQueue.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCountProvider.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

struct CCallbackTimerQueue::TimerData
{
   TimerData(
      Timer &timer,
      DWORD timeout,
      UserData userData);

   Timer * const pTimer;
   const DWORD timeout;
   const UserData userData;
};

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

static const CTickCountProvider s_tickProvider;

static const DWORD s_tickCountMax = 0xFFFFFFFF;

static const DWORD s_timeoutMax = s_tickCountMax / 4 * 3;

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
   for (TimerQueue::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
   {
      if (it != m_wrapPoint)
      {
         delete *it;
      }
   }
}

CCallbackTimerQueue::Handle CCallbackTimerQueue::SetTimer(
   Timer &timer,
   const DWORD timeoutMillis,
   const UserData userData)
{
   if (timeoutMillis > m_maxTimeout)
   {
      throw CException(_T("CCallbackTimerQueue::SetTimer()"), _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   const DWORD now = m_tickProvider.GetTickCount();

   const DWORD timeout = now + timeoutMillis;

   bool wrapped = (timeout < now);

   TimerData *pData = new TimerData(timer, timeout, userData);

   if (wrapped && m_wrapPoint == m_queue.end())
   {
      m_wrapPoint = m_queue.insert(m_queue.end(), 0);      
   }
   
   TimerQueue::iterator wrapPoint = m_wrapPoint;

   TimerQueue::iterator it = (wrapped ? ++wrapPoint :  m_queue.begin());
   // insert into the list in the right place

   while (it != m_queue.end() && it != m_wrapPoint && (*it)->timeout < timeout)
   {
      ++it;
   }

   it = m_queue.insert(it, pData);

   Handle handle = reinterpret_cast<Handle>(pData);

   m_handleMap[handle] = it;

   return handle;
}

bool CCallbackTimerQueue::CancelTimer(
   Handle handle)
{
   bool wasPending = false;

   HandleMap::iterator mapIt = m_handleMap.find(handle);

   if (mapIt != m_handleMap.end())
   {
      wasPending = true;

      TimerQueue::iterator it = mapIt->second;

      delete *it;
      m_queue.erase(it);
      m_handleMap.erase(mapIt);
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

      timeout = pData->timeout - now;

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
         m_handleMap.erase(reinterpret_cast<Handle>(pData));

         pData->pTimer->OnTimer(pData->userData);

         delete pData;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::TimerData(
   Timer &timer_,
   DWORD timeout_,
   UserData userData_)
   :  pTimer(&timer_), 
      timeout(timeout_),
      userData(userData_)
{
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::Timer
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::Timer::~Timer()
{
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
