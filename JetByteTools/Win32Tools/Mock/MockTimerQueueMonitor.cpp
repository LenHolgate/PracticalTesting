///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueueMonitor.cpp
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

#include "JetByteTools\Admin\Admin.h"

#include "MockTimerQueueMonitor.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Win32::ToString;

using JetByteTools::Test::CTestException;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueueMonitor
///////////////////////////////////////////////////////////////////////////////

CMockTimerQueueMonitor::CMockTimerQueueMonitor()
   :  m_numTimersActive(0),
      logMessages(true)
{

}

bool CMockTimerQueueMonitor::NoTimersAreActive() const
{
   return m_numTimersActive == 0;
}

void CMockTimerQueueMonitor::OnTimerCreated()
{
   ::InterlockedIncrement(&m_numTimersActive);

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
   ::InterlockedDecrement(&m_numTimersActive);

   if (logMessages)
   {
      LogMessage(_T("OnTimerDeleted"));
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueueMonitor.cpp
///////////////////////////////////////////////////////////////////////////////

