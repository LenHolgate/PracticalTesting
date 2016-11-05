#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_ADMIN_FORWARD_DECLARE_WINSOCK_TYPES_WRAPPER_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_FORWARD_DECLARE_WINSOCK_TYPES_WRAPPER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ForwardDeclareWinsockTypes.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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

/// \file ForwardDeclareWinsockTypes.h
/// There are various issues with using various early versions of the platform
/// SDK (or using VC6 and no platform SDK) with the winsock2.h header. This is
/// part of a crude attempt to avoid the problems. It doesn't work very well...
/// \ingroup Admin

#include <wtypes.h>

#if (JETBYTE_NO_PLATFORM_SDK == 1)

typedef unsigned int    u_int;

typedef u_int SOCKET;

#else

typedef UINT_PTR SOCKET;

#endif

struct sockaddr;
struct sockaddr_in6;

#endif // JETBYTE_TOOLS_ADMIN_FORWARD_DECLARE_WINSOCK_TYPES_WRAPPER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ForwardDeclareWinsockTypes.h
///////////////////////////////////////////////////////////////////////////////
