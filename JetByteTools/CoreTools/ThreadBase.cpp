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
#include "Exception.h"
#include "LockableObject.h"
#include "ToString.h"

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

///////////////////////////////////////////////////////////////////////////////
// CThread
///////////////////////////////////////////////////////////////////////////////

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

void CThreadBase::NotifyThreadStartListeners()
{
   try
   {
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadBase.cpp
///////////////////////////////////////////////////////////////////////////////
