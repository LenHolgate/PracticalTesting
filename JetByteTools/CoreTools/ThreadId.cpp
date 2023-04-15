///////////////////////////////////////////////////////////////////////////////
// File: ThreadId.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "ThreadId.h"

#pragma hdrstop

#if (JETBYTE_CORE_THREAD_USE_STD_THREADING == 0)
#include <Windows.h>
#endif

#include "StringConverter.h"

#include <sstream>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// ThreadId
///////////////////////////////////////////////////////////////////////////////

#if (JETBYTE_CORE_THREAD_USE_STD_THREADING == 0)
const ThreadId InvalidThreadId = 0;

ThreadId GetCurrentThreadId()
{
   return ::GetCurrentThreadId();
}

#else
const ThreadId InvalidThreadId = std::thread::id();

ThreadId GetCurrentThreadId()
{
   return std::this_thread::get_id();
}
#endif

std::string ToStringA(
   const std::thread::id &id)
{
   std::ostringstream strm;

   strm << id;

   return strm.str();
}

#ifndef JETBYTE_TOOLS_ADMIN_UNIX_PLATFORM
std::wstring ToStringW(
   const std::thread::id &id)
{
   return CStringConverter::AtoW(ToStringA(id));
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadId.cpp
///////////////////////////////////////////////////////////////////////////////
