///////////////////////////////////////////////////////////////////////////////
// File: Admin.cpp
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

#include "JetByteTools\Admin\Admin.h"

#pragma hdrstop

#pragma MESSAGE(" Build configuration:---------------------------------------------------------------")

#ifdef JETBYTE_TOOLS_VERSION
#pragma MESSAGE(" Build configuration: JetByte Tools Library Version = " MACROASSTRING(JETBYTE_TOOLS_VERSION))
#else
#pragma MESSAGE("Build configuration: JetByte Tools Library Version = UNSET")
#endif

#pragma MESSAGE(" Build configuration: Target Platform can be adjusted by editing the values in Admin\\TargetWindowsVersion.h")


#ifdef _WIN32_WINNT
#if (_WIN32_WINNT < JETBYTE_MINIMUM_SUPPORTED_WINDOWS_VERSION)
#pragma MESSAGE(" **** Build configuration UNSUPPORTED ****")
#endif
#if (_WIN32_WINNT == 0x0500)
#pragma MESSAGE(" Build configuration: Target Platform = Windows 2000 - (_WIN32_WINNT = " MACROASSTRING(_WIN32_WINNT) ")")
#endif
#if (_WIN32_WINNT == 0x0501)
#pragma MESSAGE(" Build configuration: Target Platform = Windows XP - (_WIN32_WINNT = " MACROASSTRING(_WIN32_WINNT) ")")
#endif
#if (_WIN32_WINNT == 0x0502)
#pragma MESSAGE(" Build configuration: Target Platform = Windows Server 2003 - (_WIN32_WINNT = " MACROASSTRING(_WIN32_WINNT) ")")
#endif
#if (_WIN32_WINNT == 0x0600)
#pragma MESSAGE(" Build configuration: Target Platform = Windows Vista - (_WIN32_WINNT = " MACROASSTRING(_WIN32_WINNT) ")")
#endif


#else
#pragma MESSAGE(" Build configuration: _WIN32_WINNT = UNSET")
#endif

#ifdef NTDDI_VERSION
#if (NTDDI_VERSION == 0x05000000)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows 2000)")
#endif
#if (NTDDI_VERSION == 0x05000100)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows 2000 SP1)")
#endif
#if (NTDDI_VERSION == 0x05000200)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows 2000 SP2)")
#endif
#if (NTDDI_VERSION == 0x05000300)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows 2000 SP3)")
#endif
#if (NTDDI_VERSION == 0x05000400)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows 2000 SP4)")
#endif
#if (NTDDI_VERSION == 0x05010000)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows XP)")
#endif
#if (NTDDI_VERSION == 0x05010100)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows XP SP1)")
#endif
#if (NTDDI_VERSION == 0x05010200)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows XP SP2)")
#endif
#if (NTDDI_VERSION == 0x05020000)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows Server 2003)")
#endif
#if (NTDDI_VERSION == 0x05020100)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows Server 2003 SP1)")
#endif
#if (NTDDI_VERSION == 0x06000000)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows Vista)")
#endif
#if (NTDDI_VERSION == 0x06000100)
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = " MACROASSTRING(NTDDI_VERSION) " (Windows Vista SP1)")
#endif

#else
#pragma MESSAGE(" Build configuration: NTDDI_VERSION = UNSET")
#endif

#if (JETBYTE_USE_STL_PORT == 1)
#pragma MESSAGE("Build configuration: STLPort: YES")
#pragma MESSAGE("Build configuration: STLPort Version: " MACROASSTRING(JETBYTE_STL_PORT_VERSION))
#else
#pragma MESSAGE("Build configuration: STLPort: NO")
#endif

#if (JETBYTE_NO_PLATFORM_SDK == 1)
#pragma MESSAGE("Build configuration: Platform SDK: NO - VC6 only")
#else
#pragma MESSAGE("Build configuration: Platform SDK: YES")
#endif

#if (JETBYTE_ENABLE_DEPRECATED == 1)
#pragma MESSAGE("Build configuration: Deprecated code enabled: YES")
#else
#pragma MESSAGE("Build configuration: Deprecated code enabled: NO")
#endif

#if (JETBYTE_HAS_INTERLOCKED_64 == 1)
#pragma MESSAGE("Build configuration: Interlocked64 enabled: YES")
#else
#pragma MESSAGE("Build configuration: Interlocked64 enabled: NO")
#endif

#if (JETBYTE_SHOW_TODO == 1)
#pragma MESSAGE("Build configuration: TODO enabled: YES")
#else
#pragma MESSAGE("Build configuration: TODO enabled: NO")
#endif

#pragma MESSAGE("Build configuration:---------------------------------------------------------------")
#pragma MESSAGE("")
#pragma MESSAGE("")
#pragma MESSAGE("")

///////////////////////////////////////////////////////////////////////////////
// End of file: Admin.cpp
///////////////////////////////////////////////////////////////////////////////
