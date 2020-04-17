#pragma once
#ifndef JETBYTE_TOOLS_ADMIN_TYPES_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_TYPES_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Types.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

/// \file Types.h
/// This file includes some typedefs for commonly used types.
/// \ingroup Admin

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

typedef unsigned short WORD;
typedef unsigned short USHORT;
typedef unsigned long DWORD;

typedef unsigned char BYTE;

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

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

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

typedef DWORD * LPDWORD;
typedef DWORD * PDWORD;
typedef unsigned long ULONG;
typedef long LONG;
typedef long HRESULT;

typedef unsigned char BYTE;

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

#endif // JETBYTE_TOOLS_ADMIN_TYPES_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Types.h
///////////////////////////////////////////////////////////////////////////////
