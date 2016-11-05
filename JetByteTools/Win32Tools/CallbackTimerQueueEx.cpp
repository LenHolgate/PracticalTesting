///////////////////////////////////////////////////////////////////////////////
// File: CallbackTimerQueueEx.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "CallbackTimerQueueEx.h"
#include "Utils.h"
#include "Exception.h"
#include "TickCount64Provider.h"

#pragma hdrstop

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
// CCallbackTimerQueueExEx::TimerData
///////////////////////////////////////////////////////////////////////////////

class CCallbackTimerQueueEx::TimerData
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

static const CTickCount64Provider s_tickProvider;

static const Milliseconds s_tickCountMax = 0xFFFFFFFF;

static const Milliseconds s_timeoutMax = s_tickCountMax - 1;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::CCallbackTimerQueueEx(
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_maxTimeout(s_timeoutMax)
{

}

CCallbackTimerQueueEx::CCallbackTimerQueueEx()
   :  m_tickProvider(s_tickProvider),
      m_maxTimeout(s_timeoutMax)
{

}

CCallbackTimerQueueEx::~CCallbackTimerQueueEx()
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
}

CCallbackTimerQueueEx::Handle CCallbackTimerQueueEx::CreateTimer()
{
   TimerData *pData = new TimerData();

   Handle handle = reinterpret_cast<Handle>(pData);

   MarkHandleUnset(handle);

   return reinterpret_cast<Handle>(pData);
}

bool CCallbackTimerQueueEx::SetTimer(
   const Handle &handle, 
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::GetAbsoluteTimeout()"), 
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   HandleMap::iterator it = ValidateHandle(handle);

   const bool wasPending = CancelTimer(handle, it);

   TimerData *pData = reinterpret_cast<TimerData*>(it->first);

   pData->UpdateData(timer, userData);

   InsertTimer(handle, pData, timeout);

   return wasPending;
}

bool CCallbackTimerQueueEx::CancelTimer(
   const Handle &handle)
{
   return CancelTimer(handle, ValidateHandle(handle));
}

bool CCallbackTimerQueueEx::DestroyTimer(
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

bool CCallbackTimerQueueEx::DestroyTimer(
   const Handle &handle)
{
   Handle handle_ = handle;

   return DestroyTimer(handle_);
}

void CCallbackTimerQueueEx::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::GetAbsoluteTimeout()"), 
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   TimerData *pData = new TimerData(timer, userData);

   Handle handle = reinterpret_cast<Handle>(pData);

   InsertTimer(handle, pData, timeout);
}

Milliseconds CCallbackTimerQueueEx::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

void CCallbackTimerQueueEx::InsertTimer(
   const Handle &handle,
   TimerData * const pData,
   const Milliseconds timeout)
{
   const ULONGLONG now = m_tickProvider.GetTickCount64();

   const ULONGLONG absoluteTimeout = now + timeout;

   if (absoluteTimeout < now)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::InsertTimer()"), 
         _T("Timeout will extend beyond the wrap point of GetTickCount64(). ")
         _T("Well done at having your machine running for this long, ")
         _T("but this is outside of our specificiation..."));
   }

   TimerQueue::iterator it = m_queue.insert(TimerQueue::value_type(absoluteTimeout, pData));

   m_handleMap[handle] = it;
}

CCallbackTimerQueueEx::HandleMap::iterator CCallbackTimerQueueEx::ValidateHandle(
   const Handle &handle)
{
   HandleMap::iterator it = m_handleMap.find(handle);

   if (it == m_handleMap.end())
   {
// The following warning is generated when /Wp64 is set in a 32bit build. At present I think
// it's due to some confusion, and even if it isn't then it's not that crucial...
#pragma warning(push, 4)
#pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueueEx::ValidateHandle()"), 
         _T("Invalid timer handle: ") + ToString(handle));
#pragma warning(pop)
   }
   
   return it;
}

bool CCallbackTimerQueueEx::CancelTimer(
   const Handle &handle,
   const HandleMap::iterator &it)
{
   bool wasPending = false;
   
   if (it->second != m_queue.end())
   {
      m_queue.erase(it->second);

      MarkHandleUnset(handle);

      wasPending = true;
   }

   return wasPending;
}

Milliseconds CCallbackTimerQueueEx::GetNextTimeout() 
{
   Milliseconds timeUntilTimeout = INFINITE;

   TimerQueue::const_iterator it = m_queue.begin();

   if (it != m_queue.end())
   {
      const ULONGLONG now = m_tickProvider.GetTickCount64();

      const ULONGLONG timeout = it->first;
 
      const ULONGLONG ticksUntilTimeout = timeout - now;

      if (ticksUntilTimeout > s_timeoutMax)
      {
         timeUntilTimeout = 0;
      }
      else
      {
         timeUntilTimeout = static_cast<Milliseconds>(ticksUntilTimeout);
      }
   }

   return timeUntilTimeout;
}

void CCallbackTimerQueueEx::HandleTimeouts()
{
   while (0 == GetNextTimeout())
   {
      TimerQueue::iterator it = m_queue.begin();
      
      TimerData *pData = it->second;

      m_queue.erase(it);

      Handle handle = reinterpret_cast<Handle>(pData);

      MarkHandleUnset(handle);

      pData->OnTimer();

      if (pData->IsOneShotTimer())
      {
         DestroyTimer(handle);
      }
   }
}

void CCallbackTimerQueueEx::MarkHandleUnset(
   Handle handle)
{
   m_handleMap[handle] = m_queue.end();
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::TimerData::TimerData()
   :  m_pTimer(0), 
      m_userData(0),
      m_oneShotTimer(false)
{
}

CCallbackTimerQueueEx::TimerData::TimerData(
   Timer &timer,
   UserData userData)
   :  m_pTimer(&timer), 
      m_userData(userData),
      m_oneShotTimer(true)
{
}

void CCallbackTimerQueueEx::TimerData::UpdateData(
   Timer &timer,
   UserData userData)
{
   if (m_oneShotTimer)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::UpdateData()"), 
         _T("Internal Error: Can't update one shot timers"));
   }

   m_pTimer = &timer; 
   m_userData = userData;
}

void CCallbackTimerQueueEx::TimerData::OnTimer()
{
   if (!m_pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::OnTimer()"), 
         _T("Internal Error: Timer not set"));
   }

   m_pTimer->OnTimer(m_userData);
}

bool CCallbackTimerQueueEx::TimerData::IsOneShotTimer() const
{
   return m_oneShotTimer;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// Requires Windows Vista or later due to use of GetTickCount64()
///////////////////////////////////////////////////////////////////////////////

#endif // (_WIN32_WINNT >= 0x0600)

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
