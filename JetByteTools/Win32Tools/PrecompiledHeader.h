#pragma once
#ifndef JETBYTE_TOOLS_WIN32_PRECOMPILED_HEADER_INCLUDED__
#define JETBYTE_TOOLS_WIN32_PRECOMPILED_HEADER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: PrecompiledHeader.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 JetByte Limited.
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

#include "AutoResetEvent.h"
#include "CallbackTimerQueueEx.h"
#include "CallbackTimerWheel.h"
#include "CompareStrings.h"
#include "CriticalSection.h"
#include "LockableObject.h"
#include "DebugTrace.h"
#include "Event.h"
#include "Exception.h"
#include "ExpandableBuffer.h"
#include "GlobalErrorHandler.h"
#include "ICriticalSection.h"
#include "IKernelObjectName.h"
#include "IListenToThreadNaming.h"
#include "ILogMessages.h"
#include "IManageTimerQueue.h"
#include "IProvideTickCount.h"
#include "IProvideTickCount64.h"
#include "IQueueTimers.h"
#include "IRunnable.h"
#include "IWaitable.h"
#include "ManualResetEvent.h"
#include "MessageLog.h"
#include "NullCallbackTimerQueueMonitor.h"
#include "NullMessageLog.h"
#include "NullThreadedCallbackTimerQueueMonitor.h"
#include "PerformanceCounter.h"
#include "PerThreadErrorHandler.h"
#include "Process.h"
#include "SEHException.h"
#include "SimpleMessageLog.h"
#include "SmartHandle.h"
#include "StringConverter.h"
#include "StringVector.h"
#include "SystemTime.h"
#include "Thread.h"
#include "ThreadAffinity.h"
#include "ThreadedCallbackTimerQueue.h"
#include "ThreadLocalStorage.h"
#include "TickCount64Provider.h"
#include "TickCountProvider.h"
#include "ToString.h"
#include "tstring.h"
#include "Utils.h"
#include "Win32Exception.h"
#include "CrtReportHook.h"
#include "IntrusiveRedBlackTree.h"
#include "IntrusiveRedBlackTreeNode.h"
#include "IntrusiveSetNode.h"

#endif // JETBYTE_TOOLS_WIN32_PRECOMPILED_HEADER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: PrecompiledHeader.h
///////////////////////////////////////////////////////////////////////////////