///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

#include "MockTimerQueue.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"

#pragma hdrstop

#pragma warning(disable:4355) // 'this' used in base member initializer list

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::ToString;
using JetByteTools::Win32::CReentrantLockableObject;

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestLog;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueue
///////////////////////////////////////////////////////////////////////////////

CMockTimerQueue::CMockTimerQueue()
   :  m_nextTimer(0),
      m_maxTimeout(0xFFFFFFFE),
      m_nextTimeout(INFINITE),
      waitForOnTimerWaitComplete(false),
      includeHandleValuesInLogs(true)
{

}

CMockTimerQueue::CMockTimerQueue(
   CTestLog *pLinkedLog)
   :  CTestLog(pLinkedLog),
      m_nextTimer(0),
      m_maxTimeout(0xFFFFFFFE),
      m_nextTimeout(INFINITE),
      waitForOnTimerWaitComplete(false),
      includeHandleValuesInLogs(true)
{

}

void CMockTimerQueue::SetNextTimeout(
   const Milliseconds nextTimeout)
{
   m_nextTimeout = nextTimeout;
}

bool CMockTimerQueue::WaitForNextTimeout(
   const Milliseconds timeout)
{
   return m_nextTimeoutEvent.Wait(timeout);
}

bool CMockTimerQueue::WaitForOnTimer(
   const Milliseconds timeout)
{
   const bool ok = m_onTimerEvent.Wait(timeout);

   if (ok && waitForOnTimerWaitComplete)
   {
      m_onTimerWaitCompleteEvent.Set();
   }

   return ok;
}

void CMockTimerQueue::OnTimer()
{
   CReentrantLockableObject::Owner lock(m_lock);

   if (m_setTimers.empty())
   {
      throw CTestException(_T("CMockTimerQueue::OnTimer()"), _T("Timer not set"));
   }

   const TimerDetails details = m_setTimers.front();

   m_setTimers.pop_front();

   details.timer.OnTimer(details.userData);

   m_onTimerEvent.Set();

   if (waitForOnTimerWaitComplete)
   {
      m_onTimerWaitCompleteEvent.Wait();
   }
}

bool CMockTimerQueue::IsTimerSet() const
{
   CReentrantLockableObject::Owner lock(m_lock);

   return !m_setTimers.empty();
}

Milliseconds CMockTimerQueue::GetNextTimeout()
{
   CReentrantLockableObject::Owner lock(m_lock);

   LogMessage(_T("GetNextTimeout"));

   m_nextTimeoutEvent.Set();

   return m_nextTimeout;

}

bool CMockTimerQueue::BeginTimeoutHandling()
{
   CReentrantLockableObject::Owner lock(m_lock);

   LogMessage(_T("BeginTimeoutHandling"));

   return !m_setTimers.empty();
}

void CMockTimerQueue::HandleTimeout()
{
   CReentrantLockableObject::Owner lock(m_lock);

   LogMessage(_T("HandleTimeout"));

   m_nextTimeout = INFINITE;

   OnTimer();
}

void CMockTimerQueue::EndTimeoutHandling()
{
   LogMessage(_T("EndTimeoutHandling"));
}

CMockTimerQueue::Handle CMockTimerQueue::CreateTimer()
{
   CReentrantLockableObject::Owner lock(m_lock);

   Handle handle = ::InterlockedIncrement(&m_nextTimer);

   LogMessage(_T("CreateTimer: ") + ToString(handle));

   return handle;
}

#pragma warning(disable: 4100)

bool CMockTimerQueue::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   CReentrantLockableObject::Owner lock(m_lock);

   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("SetTimer: ") + ToString(handle) + _T(": ") + ToString(timeout));
   }
   else
   {
      LogMessage(_T("SetTimer: ") + ToString(timeout));
   }

   bool wasPending = false;

#if (_MSC_VER == 1400)
   for (SetTimers::iterator it = m_setTimers.begin(), end = m_setTimers.end();
#else
   for (SetTimers::const_iterator it = m_setTimers.begin(), end = m_setTimers.end();
#endif
      it != end;
      ++it)
   {
      if (it->handle == handle)
      {
         m_setTimers.erase(it);

         wasPending = true;

         break;
      }
   }

   m_setTimers.push_back(TimerDetails(handle, timer, userData));

   m_nextTimeout = 0;

   return wasPending;
}

bool CMockTimerQueue::CancelTimer(
   const Handle &handle)
{
   CReentrantLockableObject::Owner lock(m_lock);

   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("CancelTimer: ") + ToString(handle));
   }
   else
   {
      LogMessage(_T("CancelTimer"));
   }

   bool wasSet = false;

   for (SetTimers::iterator it = m_setTimers.begin(), end = m_setTimers.end();
      it != end;
      ++it)
   {
      if (it->handle == handle)
      {
         m_setTimers.erase(it);

         wasSet = true;

         break;
      }
   }

   return wasSet;
}

bool CMockTimerQueue::DestroyTimer(
   Handle &handle)
{
   CReentrantLockableObject::Owner lock(m_lock);

   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("DestroyTimer: ") + ToString(handle));
   }
   else
   {
      LogMessage(_T("DestroyTimer"));
   }

   bool wasSet = false;

   for (SetTimers::iterator it = m_setTimers.begin(), end = m_setTimers.end();
      it != end;
      ++it)
   {
      if (it->handle == handle)
      {
         m_setTimers.erase(it);

         wasSet = true;

         break;
      }
   }

   handle = InvalidHandleValue;

   return wasSet;
}

bool CMockTimerQueue::DestroyTimer(
   const Handle &handle)
{
   CReentrantLockableObject::Owner lock(m_lock);

   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("DestroyTimer: ") + ToString(handle));
   }
   else
   {
      LogMessage(_T("DestroyTimer"));
   }

   bool wasSet = false;

   for (SetTimers::iterator it = m_setTimers.begin(), end = m_setTimers.end();
      it != end;
      ++it)
   {
      if (it->handle == handle)
      {
         m_setTimers.erase(it);

         wasSet = true;

         break;
      }
   }

   return wasSet;
}

void CMockTimerQueue::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData)
{
   CReentrantLockableObject::Owner lock(m_lock);

   LogMessage(_T("SetTimer: ") + ToString(timeout));

   m_setTimers.push_back(TimerDetails(InvalidHandleValue, timer, userData));
}

Milliseconds CMockTimerQueue::GetMaximumTimeout() const
{
   LogMessage(_T("GetMaximumTimeout"));

   return m_maxTimeout;
}

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueue::TimerExpiryThread
///////////////////////////////////////////////////////////////////////////////

CMockTimerQueue::TimerExpiryThread::TimerExpiryThread(
   CMockTimerQueue &timerQueue)
   :  m_thread(*this),
      m_timerQueue(timerQueue)
{
   m_thread.Start();
}

CMockTimerQueue::TimerExpiryThread::~TimerExpiryThread()
{

}

int CMockTimerQueue::TimerExpiryThread::Run()
{
   try
   {
      m_timerQueue.OnTimer();

      return 1;
   }
   catch(...)
   {
   }

   return 0;
}

void CMockTimerQueue::TimerExpiryThread::WaitForCompletion()
{
   m_thread.Wait();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////

