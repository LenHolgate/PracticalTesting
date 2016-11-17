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

typedef unsigned long DWORD;

typedef unsigned char BYTE;

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

#if defined(_WIN64)
    typedef __int64 INT_PTR;
    typedef unsigned __int64 UINT_PTR;

    typedef __int64 LONG_PTR;
    typedef unsigned __int64 ULONG_PTR;
#else
    typedef int INT_PTR;
    typedef unsigned int UINT_PTR;

    typedef long LONG_PTR;
    typedef unsigned long ULONG_PTR;
#endif

#endif // JETBYTE_TOOLS_ADMIN_TYPES_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Types.h
///////////////////////////////////////////////////////////////////////////////
