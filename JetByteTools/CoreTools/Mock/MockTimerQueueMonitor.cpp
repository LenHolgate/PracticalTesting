///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueueMonitor.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
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

#include "MockTimerQueueMonitor.h"

#include "JetByteTools/TestTools/TestException.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueueMonitor
///////////////////////////////////////////////////////////////////////////////

CMockTimerQueueMonitor::CMockTimerQueueMonitor()
   :  logMessages(true),
      m_numTimersActive(0)
{

}

bool CMockTimerQueueMonitor::NoTimersAreActive() const
{
   return m_numTimersActive.Value() == 0;
}

void CMockTimerQueueMonitor::OnTimerCreated()
{
   m_numTimersActive.Increment();

   if (logMessages)
   {
      LogMessage(_T("OnTimerCreated"));
   }
}

void CMockTimerQueueMonitor::OnOneOffTimerSet()
{
   if (logMessages)
   {
      LogMessage(_T("OnOneOffTimerSet"));
   }
}

void CMockTimerQueueMonitor::OnTimerSet(
   const bool wasPending)
{
   if (logMessages)
   {
      if (wasPending)
      {
         LogMessage(_T("OnTimerSet: Was Pending"));
      }
      else
      {
         LogMessage(_T("OnTimerSet"));
      }
   }
}

void CMockTimerQueueMonitor::OnTimerUpdated(
   const bool wasPending,
   const bool wasUpdated)
{
   if (logMessages)
   {
      _tstring message;

      if (wasPending)
      {
         message = _T(": WasPending");
      }

      if (wasUpdated)
      {
         if (message.length() == 0)
         {
            message += _T(":");
         }

         message += _T(" WasUpdated");
      }

      LogMessage(_T("OnTimerUpdated") + message);
   }
}

void CMockTimerQueueMonitor::OnTimer()
{
   if (logMessages)
   {
      LogMessage(_T("OnTimer"));
   }
}

void CMockTimerQueueMonitor::OnTimerCancelled(
   const bool wasPending)
{
   if (logMessages)
   {
      if (wasPending)
      {
         LogMessage(_T("OnTimerCancelled: Was Pending"));
      }
      else
      {
         LogMessage(_T("OnTimerCancelled"));
      }
   }
}

void CMockTimerQueueMonitor::OnTimerDestroyed(
   const bool wasPending)
{
   if (wasPending)
   {
      LogMessage(_T("OnTimerDestroyed: Was Pending"));
   }
   else
   {
      LogMessage(_T("OnTimerDestroyed"));
   }
}

void CMockTimerQueueMonitor::OnTimerDeleted()
{
   m_numTimersActive.Decrement();

   if (logMessages)
   {
      LogMessage(_T("OnTimerDeleted"));
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueueMonitor.cpp
///////////////////////////////////////////////////////////////////////////////
