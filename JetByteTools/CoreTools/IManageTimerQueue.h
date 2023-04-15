#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IManageTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
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

#include "Types.h"

#include "IQueueTimers.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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

      ~IManageTimerQueue() override = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IQueueTimers.h
///////////////////////////////////////////////////////////////////////////////
