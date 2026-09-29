///////////////////////////////////////////////////////////////////////////////
// File: TestTimerQueueFacade.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2026 JetByte Limited.
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

#include "TestTimerQueueFacade.h"
#include "MockTimerQueue.h"

#include "JetByteTools/CoreTools/ISupportTimerQueueFacade.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestLog;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestTimerQueueFacade
///////////////////////////////////////////////////////////////////////////////

CTestTimerQueueFacade::CTestTimerQueueFacade(
   CMockTimerQueue &timerQueue)
   :  CTestTimerQueueFacade(
         timerQueue,
         timerQueue)
{
}

CTestTimerQueueFacade::CTestTimerQueueFacade(
   CTestLog &log,
   ISupportTimerQueueFacade &impl)
   :  CTestLog(&log),
      waitForShutdownDuringDestruction(false),
      m_impl(impl)
{
}

CTestTimerQueueFacade::~CTestTimerQueueFacade()
{
   LogMessage(_T("[~CTestTimerQueueFacade]"));

   if (waitForShutdownDuringDestruction)
   {
      WaitForShutdownToComplete();
   }
}

void CTestTimerQueueFacade::HandleTimeouts()
{
   LogMessage(_T("[HandleTimeouts]"));

   if (BeginTimeoutHandling())
   {
      do
      {
         LogMessage(_T("[HandleTimeouts]"));

         m_impl.HandleTimeouts();

         EndTimeoutHandling();
      }
      while (BeginTimeoutHandling());
   }
}

bool CTestTimerQueueFacade::BeginTimeoutHandling()
{
   LogMessage(_T("[BeginTimeoutHandling]"));

   return m_impl.BeginTimeoutHandling();
}

void CTestTimerQueueFacade::EndTimeoutHandling()
{
   LogMessage(_T("[EndTimeoutHandling]"));

   m_impl.EndTimeoutHandling();
}

void CTestTimerQueueFacade::BeginShutdown()
{
   LogMessage(_T("[BeginShutdown]"));

   m_impl.BeginShutdown(*this);
}

bool CTestTimerQueueFacade::WaitForShutdownToComplete(
   const Milliseconds timeout)
{
   LogMessage(_T("[WaitForShutdownToComplete]"));

   return m_impl.WaitForShutdownToComplete(*this, timeout);
}

CTestTimerQueueFacade::Handle CTestTimerQueueFacade::CreateTimer()
{
   LogMessage(_T("[CreateTimer]"));

   return m_impl.CreateTimer();
}

bool CTestTimerQueueFacade::TimerIsSet(
   const Handle &handle) const
{
   LogMessage(_T("[TimerIsSet]"));

   return m_impl.TimerIsSet(handle);
}

bool CTestTimerQueueFacade::SetTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const SetTimerIf setTimerIf,
   bool *pOptionalFirstToExpireHasChanged)
{
   LogMessage(_T("[SetTimer]"));

   return m_impl.SetTimer(
      handle,
      timer,
      timeout,
      userData,
      setTimerIf,
      pOptionalFirstToExpireHasChanged);
}

bool CTestTimerQueueFacade::UpdateTimer(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated,
   bool *pOptionalFirstToExpireHasChanged)
{
   LogMessage(_T("[UpdateTimer]"));

   return m_impl.UpdateTimer(
      handle,
      timer,
      timeout,
      userData,
      updateIf,
      pWasUpdated,
      pOptionalFirstToExpireHasChanged);
}

bool CTestTimerQueueFacade::CancelTimer(
   const Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   LogMessage(_T("[CancelTimer]"));

   return m_impl.CancelTimer(
      handle,
      pOptionalFirstToExpireHasChanged);
}

bool CTestTimerQueueFacade::DestroyTimer(
   Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   LogMessage(_T("[DestroyTimer]"));

   return m_impl.DestroyTimer(
      handle,
      pOptionalFirstToExpireHasChanged);
}

bool CTestTimerQueueFacade::DestroyTimer(
   const Handle &handle,
   bool *pOptionalFirstToExpireHasChanged)
{
   LogMessage(_T("[DestroyTimer]"));

   return m_impl.DestroyTimer(
      handle,
      pOptionalFirstToExpireHasChanged);
}

void CTestTimerQueueFacade::SetTimer(
   Timer &timer,
   const Milliseconds timeout,
   const UserData userData,
   bool *pOptionalFirstToExpireHasChanged)
{
   LogMessage(_T("[SetTimer]"));

   return m_impl.SetTimer(
      timer,
      timeout,
      userData,
      pOptionalFirstToExpireHasChanged);
}

Milliseconds CTestTimerQueueFacade::GetMaximumTimeout() const
{
   LogMessage(_T("[GetMaximumTimeout]"));

   return m_impl.GetMaximumTimeout();
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestTimerQueueFacade.cpp
///////////////////////////////////////////////////////////////////////////////
