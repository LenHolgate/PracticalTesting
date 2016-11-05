#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_I_MONITOR_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_MONITOR_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IMonitorThreadedCallbackTimerQueue.h
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

#include "IMonitorCallbackTimerQueue.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IMonitorThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// An interface to allow a class to monitor the operation of an instance of
/// CCallbackTimerQueue.
/// \ingroup Interfaces
/// \ingroup ProtectedDestructors
/// \ingroup Monitoring

class IMonitorThreadedCallbackTimerQueue : public IMonitorCallbackTimerQueue
{
   public :

      enum ContentionLocation
      {
         CreateTimerContention,
         SetTimerContention,
         SetOneOffTimerContention,
         CancelTimerContention,
         DestroyTimerContention,
         TimerProcessingContention,
         GetNextTimeoutContention
      };

      virtual void OnTimerProcessingContention(
         const ContentionLocation location) = 0;

      virtual void OnTimerProcessingStarted() = 0;

      virtual void OnTimerProcessingStopped() = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      ~IMonitorThreadedCallbackTimerQueue() {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_I_MONITOR_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IMonitorThreadedCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
