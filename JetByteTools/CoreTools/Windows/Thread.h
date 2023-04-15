#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Thread.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "JetByteTools/CoreTools/LockableObject.h"
#include "JetByteTools/CoreTools/IWaitable.h"
#include "JetByteTools/CoreTools/tstring.h"
#include "JetByteTools/CoreTools/ThreadId.h"
#include "JetByteTools/CoreTools/ThreadBase.h"

#include "JetByteTools/Win32Tools/SmartHandle.h"

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools
{
   namespace Core
   {
      class IRunnable;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CThread
///////////////////////////////////////////////////////////////////////////////

class CThread : public IWaitable, public CThreadBase
{
   public :

      /// Create a thread to run the supplied instance of IRunnable.

      explicit CThread(
         IRunnable &runnable);

      CThread(
         const CThread &rhs) = delete;

      ~CThread() override = default;

      CThread &operator=(
         const CThread &rhs) = delete;

      /// Start the thread running.

      void Start();

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
         ThreadId threadID,
         const _tstring &threadName);

      // Implement IWaitable

      HANDLE GetWaitHandle() const override;

      void Wait() const override;

      bool Wait(
         Milliseconds timeout) const override;

   private :

      void InternalStart(
         bool startSuspended);

      mutable CLockableObject m_lock;

      static unsigned int __stdcall ThreadFunction(void *pV);

      Win32::CSmartHandle m_hThread;

      ThreadId m_threadID;

      IRunnable &m_runnable;

};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Thread.h
///////////////////////////////////////////////////////////////////////////////

