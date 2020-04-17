#pragma once
#ifndef JETBYTE_TOOLS_ADMIN_TARGET_WINDOWS_VERSION_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_TARGET_WINDOWS_VERSION_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TargetWindowsVersion.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2015 JetByte Limited.
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

/// \file TargetWindowsVersion.h
/// This file is included by Admin.h and is used to set the NTDDI_VERSION and
/// _WIN32_WINNT version defines.
/// We use a separate header file for this so that it's easy to have multiple
/// copies for different target platforms so that the continuous integration
/// build can copy a single file around to change the target platform.
/// \ingroup Admin

/// This define determines which version of Windows the build is targetting
/// and, subsequently, which functionality is available from the Windows header
/// files. See <a href="http://msdn2.microsoft.com/en-us/library/aa383745.aspx">here</a>
/// for more details and (in later Windows SDKs, sdkddkver.h.

#define NTDDI_VERSION   0x0A000000     // WIN10
#define _WIN32_WINNT    0x0A00         // WIN10

#endif // JETBYTE_TOOLS_ADMIN_TARGET_WINDOWS_VERSION_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TargetWindowsVersion.h
///////////////////////////////////////////////////////////////////////////////
