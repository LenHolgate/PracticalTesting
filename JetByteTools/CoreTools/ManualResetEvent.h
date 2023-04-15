#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ManualResetEvent.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 1997 JetByte Limited.
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

#include "Event.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CManualResetEvent
///////////////////////////////////////////////////////////////////////////////

class CManualResetEvent : public IWaitable
{
   public :

      explicit CManualResetEvent(
         const CEvent::InitialState initialState = CEvent::NonSignaled)
         :  m_event(CEvent::ManualReset, initialState)
      {

      }

      CManualResetEvent(
         const CManualResetEvent &rhs) = delete;

      CManualResetEvent &operator=(
         const CManualResetEvent &rhs) = delete;

      /// Set the event to the signalled state.

      void Set()
      {
         m_event.Set();
      }

      /// Resets the event to the unsignalled state.

      void Reset()
      {
         m_event.Reset();
      }

      // Implement IWaitable

      HANDLE GetWaitHandle() const override
      {
         return m_event.GetWaitHandle();
      }

      void Wait() const override
      {
         m_event.Wait();
      }

      bool Wait(
         const Milliseconds timeout) const override
      {
         return m_event.Wait(timeout);
      }

   private :

      CEvent m_event;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ManualResetEvent.h
///////////////////////////////////////////////////////////////////////////////
