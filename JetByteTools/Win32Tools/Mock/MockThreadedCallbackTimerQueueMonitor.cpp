///////////////////////////////////////////////////////////////////////////////
// File: MockThreadedCallbackTimerQueueMonitor.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"

#include "MockThreadedCallbackTimerQueueMonitor.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
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
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockThreadedCallbackTimerQueueMonitor.cpp
///////////////////////////////////////////////////////////////////////////////
