#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Thread.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"              // Allow config.h override
#include "DeclareDerivedClass.h"

#if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
#include "JetByteTools/CoreTools/Windows/Thread.h"
#endif


#if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR(CThread, IRunnable &, Windows);
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file: Thread.h
///////////////////////////////////////////////////////////////////////////////
