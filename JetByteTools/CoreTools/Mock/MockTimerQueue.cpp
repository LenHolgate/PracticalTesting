///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools/Admin/Admin.h"

#include "MockTimerQueue.h"

#include "JetByteTools/CoreTools/ToString.h"

#include "JetByteTools/TestTools/TestException.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;
using JetByteTools::Test::CTestLog;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueue
///////////////////////////////////////////////////////////////////////////////

CMockTimerQueue::CMockTimerQueue()
   :  waitForOnTimerWaitComplete(false),
      includeHandleValuesInLogs(true),
      logSetTimerEvenIfNotSet(true),
      returnCorrectTimeoutSetForNextTimeout(false),
      m_nextTimer(0),
      m_maxTimeout(0xFFFFFFFE),
      m_nextTimeout(INFINITE)
{

}

CMockTimerQueue::CMockTimerQueue(
   CTestLog *pLinkedLog)
   : CTestLog(pLinkedLog),
      waitForOnTimerWaitComplete(false),
      includeHandleValuesInLogs(true),
      logSetTimerEvenIfNotSet(true),
      returnCorrectTimeoutSetForNextTimeout(false),
      m_nextTimer(0),
      m_maxTimeout(0xFFFFFFFE),
      m_nextTimeout(INFINITE)
{
}

void CMockTimerQueue::SetNextTimeout(
   const Milliseconds nextTimeout)
{
   m_nextTimeout = nextTimeout;
}

bool CMockTimerQueue::WaitForNextTimeout(
   const Milliseconds timeout) const
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

bool CMockTimerQueue::WaitForSetTimer(
   const Milliseconds timeout)
{
   return m_onSetTimerEvent.Wait(timeout);
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

   const Handle handle = m_nextTimer.Increment();

   LogMessage(_T("CreateTimer: ") + GetHandleAsString(handle));

   return handle;
}

#pragma warning(disable: 4100)

bool CMockTimerQueue::TimerIsSet(
   const Handle &handle) const
{
   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("TimerIsSet: ") + GetHandleAsString(handle));
   }
   else
   {
      LogMessage(_T("TimerIsSet"));
   }

   bool timerIsSet = false;

   if (!m_setTimers.empty())
   {
      for (auto it = m_setTimers.begin(), end = m_setTimers.end();
         !timerIsSet && it != end;
         ++it)
      {
         if (it->handle == handle)
         {
            timerIsSet = true;
         }
      }
   }

   return timerIsSet;
}

bool CMockTimerQueue::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const SetTimerIf setTimerIf)
{
   bool wasPending = false;

   bool wasSet = false;

   CReentrantLockableObject::Owner lock(m_lock);

   for (SetTimers::const_iterator it = m_setTimers.begin(), end = m_setTimers.end();
      it != end;
      ++it)
   {
      if (it->handle == handle)
      {
         if (setTimerIf == SetTimerAlways)
         {
            m_setTimers.erase(it);
         }

         wasPending = true;

         break;
      }
   }

   if (setTimerIf == SetTimerAlways || !wasPending)
   {
      m_setTimers.push_back(TimerDetails(handle, timer, timeout, userData));


      if (returnCorrectTimeoutSetForNextTimeout)
      {
         if (timeout < m_nextTimeout)
         {

            m_nextTimeout = timeout;
         }
      }
      else
      {

         m_nextTimeout = 0;
      }


      wasSet = true;
   }

   if (wasSet || logSetTimerEvenIfNotSet)
   {
      if (setTimerIf == SetTimerAlways || logSetTimerEvenIfNotSet)
      {
         if (includeHandleValuesInLogs)
         {
            LogMessage(_T("SetTimer: ") + GetHandleAsString(handle) + _T(": ") + ToString(timeout));
         }
         else
         {
            LogMessage(_T("SetTimer: ") + ToString(timeout));
         }
      }
      else
      {
         if (includeHandleValuesInLogs)
         {
            LogMessage(_T("SetTimer: ") + GetHandleAsString(handle) + _T(": ") + ToString(timeout) + _T(" Set: ") + BoolAsString(wasSet));
         }
         else
         {
            LogMessage(_T("SetTimer: ") + ToString(timeout) + _T(" Set: ") + BoolAsString(wasSet));
         }
      }
   }

   m_onSetTimerEvent.Set();

   return wasPending;
}

bool CMockTimerQueue::UpdateTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated)
{
   bool updated = false;

   CReentrantLockableObject::Owner lock(m_lock);

   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("UpdateTimer: ") + GetHandleAsString(handle) + _T(": ") + ToString(timeout));
   }
   else
   {
      LogMessage(_T("UpdateTimer: ") + ToString(timeout));
   }

   bool wasSet = false;

   for (auto & m_setTimer : m_setTimers)
   {
      if (m_setTimer.handle == handle)
      {
         if (updateIf == UpdateAlways ||
             updateIf == UpdateAlwaysNoTimeoutChange ||
            (updateIf == UpdateTimerIfNewTimeIsLater && timeout > m_setTimer.timeout) ||
            (updateIf == UpdateTimerIfNewTimeIsSooner && timeout < m_setTimer.timeout))
         {
            updated = true;

            m_setTimer.userData = userData;
            m_setTimer.timer = timer;

            if (updateIf != UpdateAlwaysNoTimeoutChange)
            {
               m_setTimer.timeout = timeout;
            }
         }

         wasSet = true;

         break;
      }
   }

   if (!wasSet)
   {
      // timer wasn't already pending so we will set it...

      m_setTimers.push_back(TimerDetails(handle, timer, timeout, userData));

      m_nextTimeout = 0;

      updated = true;
   }

   if (pWasUpdated)
   {
      *pWasUpdated = updated;
   }

   return wasSet;
}
bool CMockTimerQueue::CancelTimer(
   const Handle &handle)
{
   CReentrantLockableObject::Owner lock(m_lock);

   if (includeHandleValuesInLogs)
   {
      LogMessage(_T("CancelTimer: ") + GetHandleAsString(handle));
   }
   else
   {
      LogMessage(_T("CancelTimer"));
   }

   bool wasSet = false;

   for (auto it = m_setTimers.begin(), end = m_setTimers.end();
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
      LogMessage(_T("DestroyTimer: ") + GetHandleAsString(handle));
   }
   else
   {
      LogMessage(_T("DestroyTimer"));
   }

   bool wasSet = false;

   for (auto it = m_setTimers.begin(), end = m_setTimers.end();
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

   for (auto it = m_setTimers.begin(), end = m_setTimers.end();
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

   m_setTimers.push_back(TimerDetails(InvalidHandleValue, timer, timeout, userData));
}

Milliseconds CMockTimerQueue::GetMaximumTimeout() const
{
   LogMessage(_T("GetMaximumTimeout"));

   return m_maxTimeout;
}

_tstring CMockTimerQueue::GetHandleAsString(
   const Handle handle) const
{
   const auto it = m_timerNames.find(handle);

   if (it != m_timerNames.end())
   {
      return it->second;
   }

   return ToString(handle);
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

unsigned int CMockTimerQueue::TimerExpiryThread::Run()
{
   try
   {
      m_timerQueue.OnTimer();

      return 1;
   }
   catch (...)
   {
   }

   return 0;
}

void CMockTimerQueue::TimerExpiryThread::WaitForCompletion() const
{
   m_thread.Wait();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueue.cpp
///////////////////////////////////////////////////////////////////////////////
