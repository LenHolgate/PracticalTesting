#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: DeclareDerivedClass.h
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

#include "JetByteTools/Admin/Platform.h"
#include "JetByteTools/Admin/Alignment.h"
#include "JetByteTools/Admin/DeclareSimpleDerivedClass.h"

#if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
#define JETBYTE_CORE_PLATFORM_NAMESPACE Windows
#else
#define JETBYTE_CORE_PLATFORM_NAMESPACE Unix
#endif

#define DECLARE_CORE_DERIVED_CLASS(_Class, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_CLASS(_Class, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_CACHE_FRIENDLY_CORE_DERIVED_CLASS(_Class1, _Class2, _BaseNamespace) namespace JetByteTools::Core { DECLARE_CACHE_FRIENDLY_DERIVED_CLASS(_Class1, JetByteTools::Core::_BaseNamespace::_Class2); }

#define DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_CORE_DERIVED_CLASS_TWO_ARG_AND_DEFAULT_CTOR(_Class, _Arg1of1Type1, _Arg2of1Type, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_CLASS_TWO_ARG_AND_DEFAULT_CTOR(_Class, _Arg1of1Type1, _Arg2of1Type, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_CORE_DERIVED_CLASS_ONE_ARG_TWO_ARG_AND_DEFAULT_CTOR(_Class, _ArgType1of1Type, _ArgType1of2Type, _ArgType2of2Type, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_CLASS_ONE_ARG_TWO_ARG_AND_DEFAULT_CTOR(_Class, _ArgType1of1Type, _ArgType1of2Type, _ArgType2of2Type, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_CORE_DERIVED_TEMPLATE_CLASS_1(_Class, _Args, _BaseNamespace) namespace JetByteTools::Core { DECLARE_DERIVED_TEMPLATE_CLASS_1(_Class, _Args, JetByteTools::Core::_BaseNamespace::_Class); }

#define DECLARE_PLATFORM_SPECIFIC_CORE_DERIVED_CLASS(_Class) DECLARE_CORE_DERIVED_CLASS(_Class, JETBYTE_CORE_PLATFORM_NAMESPACE)

#define DECLARE_PLATFORM_SPECIFIC_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType) DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType, JETBYTE_CORE_PLATFORM_NAMESPACE)

#define DECLARE_PLATFORM_SPECIFIC_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType) DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType, JETBYTE_CORE_PLATFORM_NAMESPACE)

#define DECLARE_PLATFORM_SPECIFIC_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type) DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, JETBYTE_CORE_PLATFORM_NAMESPACE)

#define DECLARE_PLATFORM_SPECIFIC_CORE_DERIVED_CLASS_TWO_ARG_AND_DEFAULT_CTOR(_Class, _ArgType1of1Type, _ArgType1of2Type) DECLARE_CORE_DERIVED_CLASS_TWO_ARG_AND_DEFAULT_CTOR(_Class, _ArgType1of1Type, _ArgType1of2Type, JETBYTE_CORE_PLATFORM_NAMESPACE)

#define DECLARE_PLATFORM_SPECIFIC_CORE_DERIVED_CLASS_ONE_ARG_TWO_ARG_AND_DEFAULT_CTOR(_Class, _ArgType1of1Type, _ArgType1of2Type, _ArgType2of2Type) DECLARE_CORE_DERIVED_CLASS_ONE_ARG_TWO_ARG_AND_DEFAULT_CTOR(_Class, _ArgType1of1Type, _ArgType1of2Type, _ArgType2of2Type, JETBYTE_CORE_PLATFORM_NAMESPACE)

///////////////////////////////////////////////////////////////////////////////
// End of file: DeclareDerivedClass.h
///////////////////////////////////////////////////////////////////////////////
