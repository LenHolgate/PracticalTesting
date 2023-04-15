#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TargetWindowsVersion.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the �Software�), to deal
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
/// for more details and, in later Windows SDKs, sdkddkver.h.

#define NTDDI_VERSION   0x06000000     // VISTA
#define _WIN32_WINNT    0x0600         // VISTA

///////////////////////////////////////////////////////////////////////////////
// End of file: TargetWindowsVersion.h
///////////////////////////////////////////////////////////////////////////////
