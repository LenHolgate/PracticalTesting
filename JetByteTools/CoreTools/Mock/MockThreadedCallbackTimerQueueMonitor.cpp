///////////////////////////////////////////////////////////////////////////////
// File: MockThreadedCallbackTimerQueueMonitor.cpp
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

#include "MockThreadedCallbackTimerQueueMonitor.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockThreadedCallbackTimerQueueMonitor
///////////////////////////////////////////////////////////////////////////////

void CMockThreadedCallbackTimerQueueMonitor::OnTimerProcessingContention(
   const ContentionLocation /*location*/)
{
   LogMessage(_T("OnTimerProcessingContention"));
}

void CMockThreadedCallbackTimerQueueMonitor::OnTimerProcessingStarted()
{
   LogMessage(_T("OnTimerProcessingStarted"));
}

void CMockThreadedCallbackTimerQueueMonitor::OnTimerProcessingStopped()
{
   LogMessage(_T("OnTimerProcessingStopped"));
}

void CMockThreadedCallbackTimerQueueMonitor::OnTimerCreated()
{
   LogMessage(_T("OnTimerCreated"));
}

void CMockThreadedCallbackTimerQueueMonitor::OnOneOffTimerSet()
{
   LogMessage(_T("OnOneOffTimerSet"));
}

void CMockThreadedCallbackTimerQueueMonitor::OnTimerSet(
   const bool wasPending)
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

void CMockThreadedCallbackTimerQueueMonitor::OnTimerUpdated(
   const bool wasPending,
   const bool wasUpdated)
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

void CMockThreadedCallbackTimerQueueMonitor::OnTimer()
{
   LogMessage(_T("OnTimer"));
}

void CMockThreadedCallbackTimerQueueMonitor::OnTimerCancelled(
   const bool wasPending)
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

void CMockThreadedCallbackTimerQueueMonitor::OnTimerDestroyed(
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

void CMockThreadedCallbackTimerQueueMonitor::OnTimerDeleted()
{
   LogMessage(_T("OnTimerDeleted"));
}


///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockThreadedCallbackTimerQueueMonitor.cpp
///////////////////////////////////////////////////////////////////////////////
