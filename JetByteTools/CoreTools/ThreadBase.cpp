///////////////////////////////////////////////////////////////////////////////
// File: ThreadBase.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2021 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"

#include "ThreadBase.h"
#include "ThreadId.h"

#include "IListenToThreadNaming.h"
#include "IListenToThreadStart.h"
#include "IListenToThreadStop.h"
#include "LockableObject.h"
#include "ToString.h"
#include "ErrorCodeToErrorMessage.h"
#include "ThreadLocalStorage.h"
#include "DebugTrace.h"

#pragma hdrstop

#include <deque>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// File level statics
///////////////////////////////////////////////////////////////////////////////

static CLockableObject s_lock;

#if (JETBYTE_CORE_TRACK_THREAD_NAMES == 1)

static CThreadBase::ThreadNames s_threadNames;

#endif

typedef std::deque<IListenToThreadNaming *> ThreadNameListeners;

static ThreadNameListeners s_threadNameListeners;

typedef std::deque<IListenToThreadStart *> ThreadStartListeners;

static ThreadStartListeners s_threadStartListeners;

typedef std::deque<IListenToThreadStop *> ThreadStopListeners;

static ThreadStopListeners s_threadStopListeners;

static bool s_threadPrimaryProcessorGroupIsSet = false;

static WORD s_threadPrimaryProcessorGroup = 0;

static DWORD_PTR s_threadAffinityMask = 0;

static CThreadLocalStorage s_tls;

///////////////////////////////////////////////////////////////////////////////
// CThreadBase
///////////////////////////////////////////////////////////////////////////////

void CThreadBase::SetAsFrameworkThread()
{
   s_tls.SetValue(1);
}

void CThreadBase::SetAsNonFrameworkThread()
{
   s_tls.SetValue(static_cast<DWORD>(0));
}

bool CThreadBase::IsFrameworkThread()
{
   return s_tls.GetValueAsDWORD() == 1;
}

void CThreadBase::SetThreadName(
   const _tstring &threadID,
   const _tstring &threadName)
{
   CLockableObject::Owner lock(s_lock);

   for (auto *pListener : s_threadNameListeners)
   {
      if (pListener)
      {
         pListener->OnThreadNaming(threadID, threadName);
      }
   }

#if (JETBYTE_CORE_TRACK_THREAD_NAMES == 1)

   //if (threadID != CThreadNameInfo::CurrentThreadID)
   //{
      s_threadNames[threadID] = threadName;
   //}
   //else
   //{
   //   s_threadNames[Core::GetCurrentThreadId()] = threadName;
   //}

#endif
}

#if (JETBYTE_CORE_TRACK_THREAD_NAMES == 1)

CThreadBase::ThreadNames CThreadBase::GetThreadNames()
{
   CLockableObject::Owner lock(s_lock);

   return s_threadNames;
}

#endif

void CThreadBase::AddThreadNameListener(
   IListenToThreadNaming &listener)
{
   CLockableObject::Owner lock(s_lock);

   s_threadNameListeners.push_back(&listener);
}

void CThreadBase::RemoveThreadNameListener(
   IListenToThreadNaming &listener)
{
   CLockableObject::Owner lock(s_lock);

   const auto it = std::find(s_threadNameListeners.begin(), s_threadNameListeners.end(), &listener);

   if (it != s_threadNameListeners.end())
   {
      *it = nullptr;
   }
}

void CThreadBase::AddThreadStartListener(
   IListenToThreadStart &listener)
{
   CLockableObject::Owner lock(s_lock);

   s_threadStartListeners.push_back(&listener);
}

void CThreadBase::RemoveThreadStartListener(
   IListenToThreadStart &listener)
{
   CLockableObject::Owner lock(s_lock);

   const auto it = std::find(s_threadStartListeners.begin(), s_threadStartListeners.end(), &listener);

   if (it != s_threadStartListeners.end())
   {
      *it = nullptr;
   }
}

void CThreadBase::AddThreadStopListener(
   IListenToThreadStop &listener)
{
   CLockableObject::Owner lock(s_lock);

   s_threadStopListeners.push_back(&listener);
}

void CThreadBase::RemoveThreadStopListener(
   IListenToThreadStop &listener)
{
   CLockableObject::Owner lock(s_lock);

   const auto it = std::find(s_threadStopListeners.begin(), s_threadStopListeners.end(), &listener);

   if (it != s_threadStopListeners.end())
   {
      *it = nullptr;
   }
}

void CThreadBase::NotifyThreadStartListeners(
   const _tstring &threadID)
{
   (void)threadID;

   try
   {
      SetThreadAfinityMaskForThisThreadIfNecessary();

      SetAsFrameworkThread();

      CLockableObject::Owner lock(s_lock);

      for (auto *pListener : s_threadStartListeners)
      {
         if (pListener)
         {
            pListener->OnThreadStarted();
         }
      }
   }
   catch(...)
   {
   }
}

void CThreadBase::NotifyThreadStopListeners()
{
   try
   {
      CLockableObject::Owner lock(s_lock);

      for (auto *pListener : s_threadStopListeners)
      {
         if (pListener)
         {
            pListener->OnThreadStopped();
         }
      }
   }
   catch(...)
   {
   }
}

void CThreadBase::SetPrimaryProcessorGroupForAllThreads(
   WORD group)
{
   s_threadPrimaryProcessorGroup = group;

   s_threadPrimaryProcessorGroupIsSet = true;
}

void CThreadBase::SetThreadAfinityMaskForAllThreads(
   const DWORD_PTR affinityMask)
{
   s_threadAffinityMask = affinityMask;

   SetThreadAfinityMaskForThisThreadIfNecessary();
}

DWORD_PTR CThreadBase::GetThreadAfinityMaskForAllThreads()
{
   return s_threadAffinityMask;
}

bool CThreadBase::ThreadAfinityMaskHasBeenSetForAllThreads()
{
   return s_threadAffinityMask != 0;
}

void CThreadBase::SetThreadAfinityMaskForThisThreadIfNecessary()
{
#ifdef JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM
#if (_WIN32_WINNT < 0x0601)
#error _WIN32_WINNT >= 0x0601 required for SetThreadGroupAffinity
#endif
   if (s_threadAffinityMask)
   {
      if (s_threadPrimaryProcessorGroupIsSet)
      {
         GROUP_AFFINITY affinity{};

         affinity.Group = s_threadPrimaryProcessorGroup;

         affinity.Mask = s_threadAffinityMask;

         GROUP_AFFINITY previousAffinity{};

         if (SetThreadGroupAffinity(GetCurrentThread(), &affinity, &previousAffinity))
         {
            OutputEx(_T("SetThreadGroupAffinity to: Group: ") + 
               ToString(s_threadPrimaryProcessorGroup) + _T(" Mask: ") + PointerToString(reinterpret_cast<void*>(s_threadAffinityMask)) +
               _T(" from Group: ") + ToString(previousAffinity.Group) + _T(" Mask: ") + PointerToString(reinterpret_cast<void*>(previousAffinity.Mask)));
         }
         else
         {
            const DWORD lastError = GetLastError();

            OutputEx(_T("Failed to SetThreadGroupAffinity to: Group: ") + 
               ToString(s_threadPrimaryProcessorGroup) + _T(" Mask: ") + PointerToString(reinterpret_cast<void*>(s_threadAffinityMask)) +
               _T(" - ") + ErrorCodeToErrorMessage(lastError));
         }
      }
      else
      {
         const DWORD_PTR previousAffinity = SetThreadAffinityMask(GetCurrentThread(), s_threadAffinityMask);

         if (previousAffinity)
         {
            OutputEx(_T("SetThreadAffinityMask to: ") + PointerToString(reinterpret_cast<void*>(s_threadAffinityMask)) + _T(" from: ") + PointerToString(reinterpret_cast<void*>(previousAffinity)));
         }
         else
         {
            const DWORD lastError = GetLastError();

            OutputEx(_T("Failed to SetThreadAffinityMask to: ") + PointerToString(reinterpret_cast<void*>(s_threadAffinityMask)) + _T(" - ") + ErrorCodeToErrorMessage(lastError));
         }
      }
   }
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadBase.cpp
///////////////////////////////////////////////////////////////////////////////
