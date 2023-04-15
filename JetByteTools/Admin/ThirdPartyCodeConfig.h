#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ThirdPartyCodeConfig.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2021 JetByte Limited.
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

// The Server Framework can be set to use some third-party code but the user
// of the framework needs to 'opt-in' to this usage if they agree and accept
// the license terms of the third-party code that is used.

// The purpose of this header file is to allow fine-grained control over the
// third-party code that is used. The user of the framework is responsible
// for accepting the licenses and JetByte Limited provides no warranties over
// the usage of this third-party code or it's fittness for purpose.

// Note that some framework functionality will not be available if various
// pieces of third-party code is disabled. Please contact JetByte Limited if
// you require us to provide alternative implementations of any third-party
// code that you do not wish to enable but find that is required for your
// usage of the framework.

///////////////////////////////////////////////////////////////////////////////
// By editing this file the user accepts all responsibility for usage of any
// third-party code by the framework.
///////////////////////////////////////////////////////////////////////////////

// Uncomment the following to allow usage of third-party code by the framework.
// This is a 'master off switch' that will turn off ALL third-party code
// if it is not defined.

#define JETBYTE_ADMIN_ENABLE_THIRD_PARTY_CODE

// Uncomment the following to turn off build warnings about third-party code
// that is NOT enabled.

//#define JETBYTE_ADMIN_DISABLE_DISABLED_THIRD_PARTY_CODE_WARNINGS

// Uncomment the following to turn off the display of third-party license
// details during the build.

#define JETBYTE_ADMIN_SHOW_THIRD_PARTY_CODE_LICENCES 0

// Uncomment the following to turn off the display of third-party license
// details during the build for code that was built from scratch after
// looking at the third-party code....

#define JETBYTE_ADMIN_SHOW_DERIVED_THIRD_PARTY_CODE_LICENCES 0

// The following turns on ALL third party code.

#define JETBYTE_ADMIN_ENABLE_ALL_THIRD_PARTY_CODE

///////////////////////////////////////////////////////////////////////////////
// End of file: ThirdPartyCodeConfig.h
///////////////////////////////////////////////////////////////////////////////
