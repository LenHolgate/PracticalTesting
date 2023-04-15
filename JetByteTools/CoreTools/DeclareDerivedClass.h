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
#include "JetByteTools/Admin/DeclareSimpleDerivedClass.h"

// VS 2015 doesn't support nested namespaces, so do it the old way for now

#ifdef JETBYTE_TOOLS_CORE_NAMESPACE_START
#undef JETBYTE_TOOLS_CORE_NAMESPACE_START
#endif JETBYTE_TOOLS_CORE_NAMESPACE_START


#ifdef JETBYTE_TOOLS_CORE_NAMESPACE_END
#undef JETBYTE_TOOLS_CORE_NAMESPACE_END
#endif JETBYTE_TOOLS_CORE_NAMESPACE_END

#if (_MSC_VER == 1900)

#define JETBYTE_TOOLS_CORE_NAMESPACE_START namespace JetByteTools { namespace Core
#define JETBYTE_TOOLS_CORE_NAMESPACE_END }

#else

#define JETBYTE_TOOLS_CORE_NAMESPACE_START namespace JetByteTools::Core
#define JETBYTE_TOOLS_CORE_NAMESPACE_END 

#endif

#ifdef DECLARE_CORE_DERIVED_CLASS
#undef DECLARE_CORE_DERIVED_CLASS
#endif

#define DECLARE_CORE_DERIVED_CLASS(_Class, _BaseNamespace) JETBYTE_TOOLS_CORE_NAMESPACE_START { DECLARE_DERIVED_CLASS(_Class, JetByteTools::Core::_BaseNamespace::_Class); } JETBYTE_TOOLS_CORE_NAMESPACE_END

#ifdef DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR
#undef DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR
#endif

#define DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType, _BaseNamespace) JETBYTE_TOOLS_CORE_NAMESPACE_START { DECLARE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType, JetByteTools::Core::_BaseNamespace::_Class); } JETBYTE_TOOLS_CORE_NAMESPACE_END


#ifdef DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR
#undef DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR
#endif

#define DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType, _BaseNamespace) JETBYTE_TOOLS_CORE_NAMESPACE_START { DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType, JetByteTools::Core::_BaseNamespace::_Class); } JETBYTE_TOOLS_CORE_NAMESPACE_END


#ifdef DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS
#undef DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS
#endif

#define DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, _BaseNamespace) JETBYTE_TOOLS_CORE_NAMESPACE_START { DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, JetByteTools::Core::_BaseNamespace::_Class); } JETBYTE_TOOLS_CORE_NAMESPACE_END


#ifdef DECLARE_CORE_DERIVED_TEMPLATE_CLASS_1
#undef DECLARE_CORE_DERIVED_TEMPLATE_CLASS_1
#endif

#define DECLARE_CORE_DERIVED_TEMPLATE_CLASS_1(_Class, _Args, _BaseNamespace) JETBYTE_TOOLS_CORE_NAMESPACE_START { DECLARE_DERIVED_TEMPLATE_CLASS_1(_Class, _Args, JetByteTools::Core::_BaseNamespace::_Class); } JETBYTE_TOOLS_CORE_NAMESPACE_END

#define DECLARE_DERIVED_CLASS_FOR_WINDOWS(_Class) DECLARE_CORE_DERIVED_CLASS(_Class, Windows)

#define DECLARE_DERIVED_CLASS_SINGLE_ARG_CTOR_FOR_WINDOWS(_Class, _ArgType) DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Class, _ArgType, Windows)

#define DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR_FOR_WINDOWS(_Class, _ArgType) DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Class, _ArgType, Windows)

#define DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS_FOR_WINDOWS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type) DECLARE_CORE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Class, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, Windows)

///////////////////////////////////////////////////////////////////////////////
// End of file: DeclareDerivedClass.h
///////////////////////////////////////////////////////////////////////////////
