#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ThreadBase.h
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

#include "ThreadId.h"
#include "tstring.h"

#if (JETBYTE_CORE_TRACK_THREAD_NAMES == 1)
#include <map>
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IListenToThreadNaming;
class IListenToThreadStart;
class IListenToThreadStop;

///////////////////////////////////////////////////////////////////////////////
// CThreadBase
///////////////////////////////////////////////////////////////////////////////

class CThreadBase
{
   public :

   static void AddThreadNameListener(
      IListenToThreadNaming &listener);

   static void RemoveThreadNameListener(
      IListenToThreadNaming &listener);

   static void AddThreadStartListener(
      IListenToThreadStart &listener);

   static void RemoveThreadStartListener(
      IListenToThreadStart &listener);

   static void AddThreadStopListener(
      IListenToThreadStop &listener);

   static void RemoveThreadStopListener(
      IListenToThreadStop &listener);


#if (JETBYTE_CORE_TRACK_THREAD_NAMES == 1)

   typedef std::map<_tstring, _tstring> ThreadNames;

   static ThreadNames GetThreadNames();

#endif

   protected :

      static void SetThreadName(
         const _tstring &threadID,
         const _tstring &threadName);

      void NotifyThreadStartListeners();

      void NotifyThreadStopListeners();
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools


///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadBase.h
///////////////////////////////////////////////////////////////////////////////
