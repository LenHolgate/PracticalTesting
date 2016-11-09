#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_I_MANAGE_TIMER_QUEUES_INCLUDED__
#define JETBYTE_TOOLS_I_MANAGE_TIMER_QUEUES_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IManageTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2008 JetByte Limited.
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

#include "JetByteTools\Admin\Types.h"

#include "IQueueTimers.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IManagerTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// An interface representing a class that manages timers that implement the
/// IQueueTimers::Timer interface and and which have their
/// IQueueTimers::Timer::OnTimer() method called when the the timer expires.
/// See <a href="http://www.lenholgate.com/archives/000389.html">here</a>
/// for more details. Note that this interface extends IQueueTimers to
/// provide the ability to deal with the timer's timeouts via
/// IManageTimerQueue::BeginTimeoutHandling(), etc. This interface is designed for
/// composing timer queues, such as is done in CThreadedCallbackTimerQueue and
/// not for passing to users of timer queues.
/// \ingroup Timers
/// \ingroup Interfaces

class IManageTimerQueue : public IQueueTimers
{
   public :

      /// Get the number of milliseconds until the next timer is due to fire.
      /// Or INFINITE if no timer is set.

      virtual Milliseconds GetNextTimeout() = 0;

      /// Returns true if timers need to be handled. You should then call
      /// HandleTimeout() to handle the timeouts and finally call
      /// EndTimeoutHandling() to tell the queue that you are done.

      virtual bool BeginTimeoutHandling() = 0;

      /// Handle the timeout for the given timer handle. Note that in an
      /// implementation that is safe for use in a multi-threaded situation it
      /// is NOT acceptable to hold a lock that will prevent concurrent calls
      /// to any of the methods on IQueueTimers.

      virtual void HandleTimeout() = 0;

      /// Complete the handling of a timeout that was started with
      /// BeginTimeoutHandling(). Note that in an implementation that is safe for
      /// use in a multi-threaded situation it is acceptable to hold a lock whilst
      /// this method is called to prevent concurrent calls to any of the methods
      /// on IQueueTimers.

      virtual void EndTimeoutHandling() = 0;

      virtual ~IManageTimerQueue() {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_I_MANAGE_TIMER_QUEUES_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IQueueTimers.h
///////////////////////////////////////////////////////////////////////////////
