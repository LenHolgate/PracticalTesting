#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Types.h
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

/// \file Types.h
/// This file includes some typedefs for commonly used types.

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

/// A millisecond counter value
typedef unsigned long Milliseconds;
/// A second counter value
typedef unsigned long Seconds;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools

// This saves us having to include wtypes.h and basetsd.h

typedef void *HANDLE;

typedef unsigned short WORD;
typedef unsigned short USHORT;
typedef unsigned long DWORD;

#if defined(_WIN64)
    typedef __int64 INT_PTR, *PINT_PTR;
    typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

    typedef __int64 LONG_PTR, *PLONG_PTR;
    typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

    #define __int3264   __int64

#else

#if !defined _W64
#define _W64
#endif

   typedef _W64 int INT_PTR, *PINT_PTR;
   typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;

   typedef _W64 long LONG_PTR, *PLONG_PTR;
   typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

   #define __int3264   __int32

#endif

typedef UINT_PTR        SOCKET;

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

typedef DWORD * LPDWORD;
typedef DWORD * PDWORD;
typedef unsigned long ULONG;
typedef long LONG;
typedef long HRESULT;

typedef unsigned __int64 ULONGLONG;
typedef unsigned __int64 uint64;

typedef unsigned char BYTE;

#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file: Types.h
///////////////////////////////////////////////////////////////////////////////
