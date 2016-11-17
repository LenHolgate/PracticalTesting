#pragma once
#ifndef JETBYTE_TOOLS_WIN32_MOCK_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
#define JETBYTE_TOOLS_WIN32_MOCK_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TestThreadedCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 JetByte Limited.
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

#include "JetByteTools\Win32Tools\ThreadedCallbackTimerQueue.h"
#include "JetByteTools\Win32Tools\AutoResetEvent.h"

#include "JetByteTools\TestTools\TestLog.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CTestThreadedCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Win32ToolsMocks

class CTestThreadedCallbackTimerQueue :
   public CThreadedCallbackTimerQueue,
   public JetByteTools::Test::CTestLog
{
   public :

      explicit CTestThreadedCallbackTimerQueue(
         IManageTimerQueue &impl);

      void OnThreadTerminationException(
         const _tstring &message) override;

      bool WaitForThreadTerminationException(
         const Milliseconds timeout) const;

   private :

      JetByteTools::Win32::CAutoResetEvent m_exceptionEvent;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_MOCK_THREADED_CALLBACK_TIMER_QUEUE_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TestThreadedCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
