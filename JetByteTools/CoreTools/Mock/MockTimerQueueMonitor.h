#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: MockTimerQueueMonitor.h
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

#include "JetByteTools/TestTools/TestLog.h"

#include "JetByteTools/CoreTools/IMonitorCallbackTimerQueue.h"
#include "JetByteTools/CoreTools/AtomicLong.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
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

      CMockTimerQueueMonitor(
         const CMockTimerQueueMonitor &rhs) = delete;

      CMockTimerQueueMonitor &operator=(
         const CMockTimerQueueMonitor &rhs) = delete;

      bool logMessages;

      bool NoTimersAreActive() const;

      // Implement IManageTimerQueue

      void OnTimerCreated() override;

      void OnOneOffTimerSet() override;

      void OnTimerSet(
         bool wasPending) override;

      void OnTimerUpdated(
         bool wasPending,
         bool wasUpdated) override;

      void OnTimer() override;

      void OnTimerCancelled(
         bool wasPending) override;

      void OnTimerDestroyed(
         bool wasPending) override;

      void OnTimerDeleted() override;

   private :

      CAtomicLong m_numTimersActive;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTimerQueueMonitor.h
///////////////////////////////////////////////////////////////////////////////
