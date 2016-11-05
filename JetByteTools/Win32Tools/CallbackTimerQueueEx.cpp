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

      void PrepareForHandleTimeout();

      void HandleTimeout();

      bool DeleteAfterTimeout() const;
      bool HasTimedOut() const;

      void SetDeleteAfterTimeout();

   private :
   
      struct Data
      {
         Data();

         Data(
            Timer &timer, 
            UserData userData);

         void Clear();

         Timer *pTimer;
      
         UserData userData;
      };

      void OnTimer(
         const Data &data);

      Data m_active;

      Data m_timedout;

      bool m_deleteAfterTimeout;
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

#pragma TODO("Remove the duplication between this and CCallbackTimerQueue")

CCallbackTimerQueueEx::CCallbackTimerQueueEx(
   const IProvideTickCount64 &tickProvider)
   :  m_tickProvider(tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false)
{

}

CCallbackTimerQueueEx::CCallbackTimerQueueEx()
   :  m_tickProvider(s_tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_handlingTimeouts(false)
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
         _T("CCallbackTimerQueueEx::SetTimer()"), 
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

   if (pData->HasTimedOut())
   {
      pData->DeleteAfterTimeout();
   }
   else
   {
      delete pData;
   }

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
         _T("CCallbackTimerQueueEx::SetTimer()"), 
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

      if (pData->DeleteAfterTimeout())
      {
         m_handleMap.erase(handle);

         delete pData;
      }
   }
}

IManageTimerQueue::TimeoutHandle CCallbackTimerQueueEx::GetTimeoutHandle(
   TimerData *pData)
{
   TimeoutHandle handle = reinterpret_cast<TimeoutHandle>(pData);

   m_timeoutHandles.insert(handle);

   return handle;
}

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::ValidateTimeoutHandle(
   IManageTimerQueue::TimeoutHandle &handle)
{
   if (m_timeoutHandles.end() == m_timeoutHandles.find(handle))
   {
// The following warning is generated when /Wp64 is set in a 32bit build. At present I think
// it's due to some confusion, and even if it isn't then it's not that crucial...
#pragma warning(push, 4)
#pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueueEx::ValidateTimeoutHandle()"), 
         _T("Invalid timeout handle: ") + ToString(handle));
#pragma warning(pop)
   }
   
   return reinterpret_cast<TimerData *>(handle);
}

CCallbackTimerQueueEx::TimerData *CCallbackTimerQueueEx::EraseTimeoutHandle(
   IManageTimerQueue::TimeoutHandle &handle)
{
   if (0 == m_timeoutHandles.erase(handle))
   {
// The following warning is generated when /Wp64 is set in a 32bit build. At present I think
// it's due to some confusion, and even if it isn't then it's not that crucial...
#pragma warning(push, 4)
#pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueueEx::EraseTimeoutHandle()"), 
         _T("Invalid timeout handle: ") + ToString(handle));
#pragma warning(pop)
   }

   return reinterpret_cast<TimerData *>(handle);
}
IManageTimerQueue::TimeoutHandle CCallbackTimerQueueEx::BeginTimeoutHandling()
{
   if (m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::BeginTimeoutHandling()"), 
         _T("Already handling timeouts, you need to call EndTimeoutHandling()?"));
   }

   TimeoutHandle timeoutHandle = InvalidTimeoutHandleValue;

   if (0 == GetNextTimeout())
   {
      m_handlingTimeouts = true;

      TimerQueue::iterator it = m_queue.begin();
      
      TimerData *pData = it->second;

      m_queue.erase(it);

      // Need to duplicate the timer data so that a call to SetTimer that occurs after 
      // this call returns but before a call to HandleTimeout with this timer doesn't 
      // cause the timer that is about to happen to be changed before it actually 
      // "goes off"...

      pData->PrepareForHandleTimeout();

      timeoutHandle = GetTimeoutHandle(pData);

      Handle handle = reinterpret_cast<Handle>(pData);

      if (pData->DeleteAfterTimeout())
      {
         m_handleMap.erase(handle);
         
         // pData will be cleaned up after we've processed the timeout
      }
      else
      {
         MarkHandleUnset(handle);
      }
   }

   return timeoutHandle;
}

void CCallbackTimerQueueEx::HandleTimeout(
   IManageTimerQueue::TimeoutHandle &handle)
{
   TimerData *pData = ValidateTimeoutHandle(handle);

   pData->HandleTimeout();
}

void CCallbackTimerQueueEx::EndTimeoutHandling(
   IManageTimerQueue::TimeoutHandle &handle)
{
   TimerData *pData = EraseTimeoutHandle(handle);

   if (pData->DeleteAfterTimeout())
   {
      delete pData;
   }

   m_handlingTimeouts = false;
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
   :  m_deleteAfterTimeout(false)
{
}

CCallbackTimerQueueEx::TimerData::TimerData(
   Timer &timer,
   UserData userData)
   :  m_active(timer, userData),
      m_deleteAfterTimeout(true)
{
}

void CCallbackTimerQueueEx::TimerData::UpdateData(
   Timer &timer,
   UserData userData)
{
   if (m_deleteAfterTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::UpdateData()"), 
         _T("Internal Error: Can't update one shot timers or timers pending deletion"));
   }

   m_active.pTimer = &timer; 
   m_active.userData = userData;
}

void CCallbackTimerQueueEx::TimerData::OnTimer()
{
   OnTimer(m_active);
}

void CCallbackTimerQueueEx::TimerData::OnTimer(
   const Data &data)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueueEx::TimerData::OnTimer()"), 
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimer(data.userData);
}

void CCallbackTimerQueueEx::TimerData::PrepareForHandleTimeout()
{
   m_timedout = m_active;

   m_active.Clear();
}

void CCallbackTimerQueueEx::TimerData::HandleTimeout()
{
   OnTimer(m_timedout);

   m_timedout.Clear();
}

bool CCallbackTimerQueueEx::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
}

bool CCallbackTimerQueueEx::TimerData::HasTimedOut() const
{ 
   return m_timedout.pTimer != 0;
}

void CCallbackTimerQueueEx::TimerData::SetDeleteAfterTimeout()
{
   m_deleteAfterTimeout = true;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueueEx::TimerData::Data
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueueEx::TimerData::Data::Data()
   :  pTimer(0), 
      userData(0)
{

}

CCallbackTimerQueueEx::TimerData::Data::Data(
   Timer &timer, 
   UserData userData_)
   :  pTimer(&timer),
      userData(userData_)
{

}

void CCallbackTimerQueueEx::TimerData::Data::Clear()
{
   pTimer = 0;
   userData = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: CallbackTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
