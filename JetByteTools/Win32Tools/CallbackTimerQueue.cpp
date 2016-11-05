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

      void PrepareForHandleTimeout();

      void HandleTimeout();

      bool DeleteAfterTimeout() const;

      bool IsMaintenanceTimer(
         const CCallbackTimerQueue *pQueue) const;

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

static const CTickCountProvider s_tickProvider;

static const Milliseconds s_tickCountMax = 0xFFFFFFFF;

static const Milliseconds s_timeoutMax = s_tickCountMax - 1;

#pragma warning(push, 4)
#pragma warning(disable: 4268)   // 'const' static/global data initialized with 
                                 // compiler generated default constructor fills 
                                 // the object with zeros

static const LARGE_INTEGER s_zeroLargeInteger;  // Rely on static init to provide 
                                                // us with a LARGE_INTEGER zero.
#pragma warning(pop)

///////////////////////////////////////////////////////////////////////////////
// Static members
///////////////////////////////////////////////////////////////////////////////

#pragma TODO("Move to its own file?")

IQueueTimers::Handle IQueueTimers::InvalidHandleValue = 0;

IManageTimerQueue::TimeoutHandle IManageTimerQueue::InvalidTimeoutHandleValue = 0;

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

#pragma TODO("Remove the duplication between this and CCallbackTimerQueueEx")

CCallbackTimerQueue::CCallbackTimerQueue(
   const IProvideTickCount &tickProvider)
   :  m_tickProvider(tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer()),
      m_handlingTimeouts(false)
{   
   SetMaintenanceTimer();
}

CCallbackTimerQueue::CCallbackTimerQueue()
   :  m_tickProvider(s_tickProvider),
      m_maxTimeout(s_timeoutMax),
      m_lastCount(s_zeroLargeInteger),
      m_maintenanceTimer(CreateTimer()),
      m_handlingTimeouts(false)
{
   SetMaintenanceTimer();
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
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"), 
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   HandleMap::iterator it = ValidateHandle(handle);

   const bool wasPending = CancelTimer(handle, it);

   TimerData *pData = reinterpret_cast<TimerData*>(it->first);

   pData->UpdateData(timer, userData);

   InsertTimer(handle, pData, timeout);

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
   if (timeout > m_maxTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::SetTimer()"), 
         _T("Timeout value is too large, max = ") + ToString(m_maxTimeout));
   }

   TimerData *pData = new TimerData(timer, userData);

   Handle handle = reinterpret_cast<Handle>(pData);

   InsertTimer(handle, pData, timeout);
}

Milliseconds CCallbackTimerQueue::GetMaximumTimeout() const
{
   return m_maxTimeout;
}

ULONGLONG CCallbackTimerQueue::GetTickCount64()
{
   const DWORD lastCount = m_lastCount.LowPart;

   m_lastCount.LowPart = m_tickProvider.GetTickCount();

   if (m_lastCount.LowPart < lastCount)
   {
      if (m_lastCount.LowPart < lastCount)
      {
         m_lastCount.HighPart++;

         SetMaintenanceTimer();
      }
   }

   return m_lastCount.QuadPart;
}

void CCallbackTimerQueue::InsertTimer(
   const Handle &handle,
   TimerData * const pData,
   const Milliseconds timeout)
{
   const ULONGLONG now = GetTickCount64();

   const ULONGLONG absoluteTimeout = now + timeout;

   if (absoluteTimeout < now)
   {
      throw CException(
         _T("CCallbackTimerQueue::InsertTimer()"), 
         _T("Timeout will extend beyond the wrap point of GetTickCount64(). ")
         _T("Well done at having your machine running for this long, ")
         _T("but this is outside of our specificiation..."));
   }

   TimerQueue::iterator it = m_queue.insert(TimerQueue::value_type(absoluteTimeout, pData));

   m_handleMap[handle] = it;
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
      throw CException(
         _T("CCallbackTimerQueue::ValidateHandle()"), 
         _T("Invalid timer handle: ") + ToString(handle));
#pragma warning(pop)
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

      MarkHandleUnset(handle);

      wasPending = true;
   }

   return wasPending;
}

Milliseconds CCallbackTimerQueue::GetNextTimeout() 
{
   Milliseconds timeUntilTimeout = INFINITE;

   TimerQueue::const_iterator it = m_queue.begin();

   if (it != m_queue.end())
   {
      if (it->second->IsMaintenanceTimer(this))
      {
         it++;
      }

      if (it != m_queue.end())
      {
         const ULONGLONG now = GetTickCount64();

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
   }

   return timeUntilTimeout;
}

void CCallbackTimerQueue::HandleTimeouts()
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

IManageTimerQueue::TimeoutHandle CCallbackTimerQueue::GetTimeoutHandle(
   TimerData *pData)
{
   TimeoutHandle handle = reinterpret_cast<TimeoutHandle>(pData);

   m_timeoutHandles.insert(handle);

   return handle;
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::ValidateTimeoutHandle(
   IManageTimerQueue::TimeoutHandle &handle)
{
   if (m_timeoutHandles.end() == m_timeoutHandles.find(handle))
   {
// The following warning is generated when /Wp64 is set in a 32bit build. At present I think
// it's due to some confusion, and even if it isn't then it's not that crucial...
#pragma warning(push, 4)
#pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueue::ValidateTimeoutHandle()"), 
         _T("Invalid timeout handle: ") + ToString(handle));
#pragma warning(pop)
   }
   
   return reinterpret_cast<TimerData *>(handle);
}

CCallbackTimerQueue::TimerData *CCallbackTimerQueue::EraseTimeoutHandle(
   IManageTimerQueue::TimeoutHandle &handle)
{
   if (0 == m_timeoutHandles.erase(handle))
   {
// The following warning is generated when /Wp64 is set in a 32bit build. At present I think
// it's due to some confusion, and even if it isn't then it's not that crucial...
#pragma warning(push, 4)
#pragma warning(disable: 4244)
      throw CException(
         _T("CCallbackTimerQueue::EraseTimeoutHandle()"), 
         _T("Invalid timeout handle: ") + ToString(handle));
#pragma warning(pop)
   }

   return reinterpret_cast<TimerData *>(handle);
}

IManageTimerQueue::TimeoutHandle CCallbackTimerQueue::BeginTimeoutHandling()
{
   if (m_handlingTimeouts)
   {
      throw CException(
         _T("CCallbackTimerQueue::BeginTimeoutHandling()"), 
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

void CCallbackTimerQueue::HandleTimeout(
   IManageTimerQueue::TimeoutHandle &handle)
{
   TimerData *pData = ValidateTimeoutHandle(handle);

   pData->HandleTimeout();
}

void CCallbackTimerQueue::EndTimeoutHandling(
   IManageTimerQueue::TimeoutHandle &handle)
{
   TimerData *pData = EraseTimeoutHandle(handle);

   if (pData->DeleteAfterTimeout())
   {
      delete pData;
   }

   m_handlingTimeouts = false;
}

void CCallbackTimerQueue::MarkHandleUnset(
   Handle handle)
{
   m_handleMap[handle] = m_queue.end();
}

void CCallbackTimerQueue::SetMaintenanceTimer()
{
   // Sets a timer for the next GetTickCount() roll over...

   HandleMap::iterator it = ValidateHandle(m_maintenanceTimer);

   CancelTimer(m_maintenanceTimer, it);

   TimerData *pData = reinterpret_cast<TimerData*>(it->first);

   pData->UpdateData(m_maintenanceTimerHandler, reinterpret_cast<UserData>(this));

  {
      Handle handle = reinterpret_cast<Handle>(pData);

      const ULONGLONG now = GetTickCount64();

      const ULONGLONG absoluteTimeout = (now & 0xFFFFFFFF00000000) + 0x0000000100000000;
  
      TimerQueue::iterator it = m_queue.insert(TimerQueue::value_type(absoluteTimeout, pData));

      m_handleMap[handle] = it;
   }
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::MaintenanceTimerHandler
///////////////////////////////////////////////////////////////////////////////

void CCallbackTimerQueue::MaintenanceTimerHandler::OnTimer(
   UserData userData)
{
   reinterpret_cast<CCallbackTimerQueue *>(userData)->SetMaintenanceTimer();
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::TimerData()
   :  m_deleteAfterTimeout(false)
{
}

CCallbackTimerQueue::TimerData::TimerData(
   Timer &timer,
   UserData userData)
   :  m_active(timer, userData),
      m_deleteAfterTimeout(true)
{
}

void CCallbackTimerQueue::TimerData::UpdateData(
   Timer &timer,
   UserData userData)
{
   if (m_deleteAfterTimeout)
   {
      throw CException(
         _T("CCallbackTimerQueue::UpdateData()"), 
         _T("Internal Error: Can't update one shot timers or timers pending deletion"));
   }

   m_active.pTimer = &timer; 
   m_active.userData = userData;
}

void CCallbackTimerQueue::TimerData::OnTimer()
{
   OnTimer(m_active);
}

void CCallbackTimerQueue::TimerData::OnTimer(
   const Data &data)
{
   if (!data.pTimer)
   {
      throw CException(
         _T("CCallbackTimerQueue::TimerData::OnTimer()"), 
         _T("Internal Error: Timer not set"));
   }

   data.pTimer->OnTimer(data.userData);
}

void CCallbackTimerQueue::TimerData::PrepareForHandleTimeout()
{
   m_timedout = m_active;

   m_active.Clear();
}

void CCallbackTimerQueue::TimerData::HandleTimeout()
{
   OnTimer(m_timedout);

   m_timedout.Clear();
}

bool CCallbackTimerQueue::TimerData::DeleteAfterTimeout() const
{
   return m_deleteAfterTimeout;
}

bool CCallbackTimerQueue::TimerData::IsMaintenanceTimer(
   const CCallbackTimerQueue *pQueue) const
{
   return m_active.userData == reinterpret_cast<UserData>(pQueue);
}

bool CCallbackTimerQueue::TimerData::HasTimedOut() const
{ 
   return m_timedout.pTimer != 0;
}

void CCallbackTimerQueue::TimerData::SetDeleteAfterTimeout()
{
   m_deleteAfterTimeout = true;
}

///////////////////////////////////////////////////////////////////////////////
// CCallbackTimerQueue::TimerData::Data
///////////////////////////////////////////////////////////////////////////////

CCallbackTimerQueue::TimerData::Data::Data()
   :  pTimer(0), 
      userData(0)
{

}

CCallbackTimerQueue::TimerData::Data::Data(
   Timer &timer, 
   UserData userData_)
   :  pTimer(&timer),
      userData(userData_)
{

}

void CCallbackTimerQueue::TimerData::Data::Clear()
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
