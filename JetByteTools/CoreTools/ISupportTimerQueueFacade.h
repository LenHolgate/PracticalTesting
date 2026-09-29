#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ISupportTimerQueueFacade.h
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

#include "IHandleTimerQueueTimeouts.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// ISupportTimerQueueFacade
///////////////////////////////////////////////////////////////////////////////

class ISupportTimerQueueFacade : public IHandleTimerQueueTimeouts
{
   public :

      /// Get the number of milliseconds until the next timer is due to fire.
      /// Or INFINITE if no timer is set.

      virtual Milliseconds GetNextTimeout() = 0;

      /// Handle the timeouts for any timers that were collected in a call
      /// to IHandleTimerQueueTimeouts::BeginTimeoutHandling(). Note that in
      /// an implementation that is safe for use in a multithreaded situation
      /// it is NOT acceptable to hold a lock that will prevent concurrent
      /// calls to any of the methods on IQueueTimers.

      virtual size_t HandleTimeouts() = 0;

      /// Start to handle timeouts due to shutdown. This call should not block.
      /// The shutdown process MUST use the supplied interface and be
      /// implemented in terms of calls to BeginTimeoutHandling(), HandleTimeouts()
      /// and EndTimeoutHandling().
      
      virtual void BeginShutdown(
         IHandleTimerQueueTimeouts &handler) = 0;

      /// Avoid ambiguity

      using IHandleTimerQueueTimeouts::BeginShutdown;

      /// End the handling of timeouts due to shutdown. This call CAN block.
      /// The shutdown process MUST use the supplied interface and be
      /// implemented in terms of calls to BeginTimeoutHandling(), HandleTimeouts()
      /// and EndTimeoutHandling().

      virtual bool WaitForShutdownToComplete(
         IHandleTimerQueueTimeouts &handler,
         Milliseconds timeout = INFINITE) = 0;

      /// Avoid ambiguity

      using IHandleTimerQueueTimeouts::WaitForShutdownToComplete;

      ~ISupportTimerQueueFacade() override = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ISupportTimerQueueFacade.h
///////////////////////////////////////////////////////////////////////////////
