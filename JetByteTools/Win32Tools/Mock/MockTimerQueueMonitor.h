#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_MOCK_TIMER_QUEUE_MONITOR_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MOCK_TIMER_QUEUE_MONITOR_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueueMonitor.h
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

#include "JetByteTools\TestTools\TestLog.h"

#include "JetByteTools\Win32Tools\IMonitorCallbackTimerQueue.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTimerQueueMonitor
///////////////////////////////////////////////////////////////////////////////

/// A mock object that implements IMonitorCallbackTimerQueue.
/// \ingroup Win32ToolsMocks

class CMockTimerQueueMonitor :
   public IMonitorCallbackTimerQueue,
   public JetByteTools::Test::CTestLog
{
   public :

      CMockTimerQueueMonitor();

      bool logMessages;

      bool NoTimersAreActive() const;

      // Implement IManageTimerQueue

      virtual void OnTimerCreated();

      virtual void OnOneOffTimerSet();

      virtual void OnTimerSet(
         const bool wasPending);

      virtual void OnTimer();

      virtual void OnTimerCancelled(
         const bool wasPending);

      virtual void OnTimerDestroyed(
         const bool wasPending);

      virtual void OnTimerDeleted();

   private :

      volatile long m_numTimersActive;

      /// No copies do not implement
      CMockTimerQueueMonitor(const CMockTimerQueueMonitor &rhs);
      /// No copies do not implement
      CMockTimerQueueMonitor &operator=(const CMockTimerQueueMonitor &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MOCK_TIMER_QUEUE_MONITOR_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueueMonitor.h
///////////////////////////////////////////////////////////////////////////////
