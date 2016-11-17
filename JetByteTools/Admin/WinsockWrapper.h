#pragma once
#ifndef JETBYTE_TOOLS_ADMIN_WINSOCK_WRAPPER_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_WINSOCK_WRAPPER_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: WinsockWrapper.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2003 JetByte Limited.
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

/// \file JetByteTools\Admin\WinsockWrapper.h
/// There are various issues with using various early versions of the platform
/// SDK (or using VC6 and no platform SDK) with the winsock2.h header. This is
/// part of a crude attempt to avoid the problems.
/// \ingroup Admin

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#endif // JETBYTE_TOOLS_ADMIN_WINSOCK_WRAPPER_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: WinsockWrapper.h
///////////////////////////////////////////////////////////////////////////////
