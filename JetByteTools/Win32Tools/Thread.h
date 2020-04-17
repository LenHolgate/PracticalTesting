#pragma once
#ifndef JETBYTE_TOOLS_WIN32_THREAD_INCLUDED__
#define JETBYTE_TOOLS_WIN32_THREAD_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Thread.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2002 JetByte Limited.
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

#include "SmartHandle.h"
#include "LockableObject.h"
#include "IWaitable.h"
#include "tstring.h"

#if (JETBYTE_TRACK_THREAD_NAMES == 1)
#include <map>
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IRunnable;
class IListenToThreadNaming;

///////////////////////////////////////////////////////////////////////////////
// CThread
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms684841.aspx">Threading
/// API</a> and provides a thread object that runs code which implements the
/// IRunnable interface.
/// \ingroup Threading

class CThread : public IWaitable
{
   public :

      static void AddThreadNameListener(
         IListenToThreadNaming &listener);

      static void RemoveThreadNameListener(
         IListenToThreadNaming &listener);

      /// Create a thread to run the supplied instance of IRunnable.

      explicit CThread(
         IRunnable &runnable);

      CThread(
         const CThread &rhs) = delete;

      virtual ~CThread() = default;

      CThread &operator=(
         const CThread &rhs) = delete;

      /// Start the thread running.

      void Start();

      /// Start the thread running and optionally start it in a suspended state.

      void Start(
         bool startSuspended);

      /// Start the thread in a suspended state.

      void StartSuspended();

      /// Start the thread with the specified thread priority.

      void StartWithPriority(
         int priority);

      /// Resume a suspended thread.

      void Resume();

      /// Terminate a thread and have it return the supplied exit code.
      /// Note that this should be used with great care!

      void Terminate(
         DWORD exitCode = 0);

      /// Returns true if the thread is running.

      bool IsRunning() const;

      /// Sets a threads name so that it can be queried in a debugger.

      void SetThreadName(
         const _tstring &threadName) const;

      /// Returns true if the thread object represents the thread that is calling
      /// IsThisThread().

      bool IsThisThread() const;

      /// Sets the current threads name so that it can be queried in a debugger.

      static void SetCurrentThreadName(
         const _tstring &threadName);

      /// Sets the supplied threads name so that it can be queried in a debugger.

      static void SetThreadName(
         DWORD threadID,
         const _tstring &threadName);

      void EnableThreadPriorityBoost();

      void DisableThreadPriorityBoost();

      bool ThreadPriorityBoostEnabled() const;

      void SetThreadPriority(
         int priority);

      int GetThreadPriority() const;

#if (JETBYTE_TRACK_THREAD_NAMES == 1)

      typedef std::map<DWORD, _tstring> ThreadNames;

      static ThreadNames GetThreadNames();

#endif

      // Implement IWaitable

      HANDLE GetWaitHandle() const override;

      void Wait() const override;

      bool Wait(
         Milliseconds timeout) const override;

   private :

      void InternalStart(
         bool startSuspended);

      void InternalResume();

      mutable CLockableObject m_lock;

      static unsigned int __stdcall ThreadFunction(void *pV);

      CSmartHandle m_hThread;

      DWORD m_threadID;

      IRunnable &m_runnable;

#if (JETBYTE_TRACK_THREAD_NAMES == 1)

      static ThreadNames s_threadNames;

#endif
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_THREAD_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Thread.h
///////////////////////////////////////////////////////////////////////////////

