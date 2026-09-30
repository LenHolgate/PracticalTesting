#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TestTimerQueueFacade.h
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

#include "JetByteTools/CoreTools/ThreadedCallbackTimerQueue.h"

#include "JetByteTools/TestTools/TestLog.h"

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools
{
   namespace Core
   {
      class ISupportTimerQueueFacade;

      namespace Mock
      {
         class CMockTimerQueue;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestTimerQueueFacade
///////////////////////////////////////////////////////////////////////////////

class CTestTimerQueueFacade :
   public IHandleTimerQueueTimeouts,
   public JetByteTools::Test::CTestLog
{
   public :

      explicit CTestTimerQueueFacade(
         CMockTimerQueue &timerQueue);

      explicit CTestTimerQueueFacade(
         ISupportTimerQueueFacade &impl);

      CTestTimerQueueFacade(
         JetByteTools::Test::CTestLog &log,
         ISupportTimerQueueFacade &impl);

      ~CTestTimerQueueFacade() override;

      bool waitForShutdownDuringDestruction;

      void HandleTimeouts();

      // Implement IHandleTimerQueueTimeouts

      bool BeginTimeoutHandling() override;

      void EndTimeoutHandling() override;

      // Iplement IManageTimerQueue

      void BeginShutdown() override;

      bool WaitForShutdownToComplete(
         Milliseconds timeout = INFINITE) override;

      // Implement IQueueTimers

      Handle CreateTimer() override;

      bool TimerIsSet(
         const Handle &handle) const override;

      bool SetTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         SetTimerIf setTimerIf = SetTimerAlways,
         bool *pOptionalFirstToExpireHasChanged = nullptr) override;

      bool UpdateTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr,
         bool *pOptionalFirstToExpireHasChanged = nullptr) override;

      bool CancelTimer(
         const Handle &handle,
         bool *pOptionalFirstToExpireHasChanged = nullptr) override;

      bool DestroyTimer(
         Handle &handle,
         bool *pOptionalFirstToExpireHasChanged = nullptr) override;

      bool DestroyTimer(
         const Handle &handle,
         bool *pOptionalFirstToExpireHasChanged = nullptr) override;

      void SetTimer(
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         bool *pOptionalFirstToExpireHasChanged = nullptr) override;

      Milliseconds GetMaximumTimeout() const override;

   private :

      void Log(
         const JetByteTools::Core::_tstring &message) const;

      bool m_sharedLog;

      ISupportTimerQueueFacade &m_impl;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TestTimerQueueFacade.h
///////////////////////////////////////////////////////////////////////////////
