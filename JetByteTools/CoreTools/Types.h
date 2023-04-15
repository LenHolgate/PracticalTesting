#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Types.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
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

#include "JetByteTools/Admin/Platform.h"

#if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
#include "JetByteTools/CoreTools/Windows/Types.h"
#endif

#include <stdint.h>
struct _OVERLAPPED;

typedef _OVERLAPPED OVERLAPPED, *LPOVERLAPPED;

struct _OVERLAPPED_ENTRY;

typedef _OVERLAPPED_ENTRY OVERLAPPED_ENTRY, *LPOVERLAPPED_ENTRY;

union _LARGE_INTEGER;

typedef _LARGE_INTEGER LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

union _ULARGE_INTEGER;

typedef _ULARGE_INTEGER ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;

inline uint64_t make64(uint32_t high, uint32_t low)
{
    return static_cast<uint64_t>(high) << 32 | low;
}

inline uint32_t high32(uint64_t value)
{
   return value >> 32;
}

inline uint32_t low32(uint64_t value)
{
    return static_cast<uint32_t>(value);
}

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

///////////////////////////////////////////////////////////////////////////////
// End of file: Types.h
///////////////////////////////////////////////////////////////////////////////
