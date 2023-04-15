#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: DeclareSimpleDerivedClass.h
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

#include "Platform.h"

#include "Alignment.h"

#ifdef DECLARE_DERIVED_CLASS
#undef DECLARE_DERIVED_CLASS
#endif

#define DECLARE_DERIVED_CLASS(_Derived, _Base) class _Derived : public _Base {}

#define DECLARE_CACHE_FRIENDLY_DERIVED_CLASS(_Derived, _Base)  class ALIGN_TO_CACHE_LINE _Derived : public _Base {}

#ifdef DECLARE_DERIVED_CLASS_SINGLE_ARG_CTOR
#undef DECLARE_DERIVED_CLASS_SINGLE_ARG_CTOR
#endif

#define DECLARE_DERIVED_CLASS_SINGLE_ARG_CTOR(_Derived, _ArgType, _Base) \
class _Derived : public _Base       \
{                                   \
   public :                         \
                                    \
      explicit _Derived(            \
         _ArgType arg)              \
         :  _Base(                  \
               arg)                 \
      {                             \
      }                             \
}

#ifdef DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR
#undef DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR
#endif

#define DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_DEFAULT_CTOR(_Derived, _ArgType, _Base) \
class _Derived : public _Base       \
{                                   \
   public :                         \
                                    \
      _Derived() = default;         \
                                    \
      explicit _Derived(            \
         _ArgType arg)              \
         :  _Base(                  \
               arg)                 \
      {                             \
      }                             \
}

#ifdef DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS
#undef DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS
#endif

#define DECLARE_DERIVED_CLASS_SINGLE_ARG_AND_TWO_ARG_CTORS(_Derived, _Arg1of1Type, _Arg1of2Type, _Arg2of2Type, _Base) \
class _Derived : public _Base       \
{                                   \
   public :                         \
                                    \
      explicit _Derived(            \
         _Arg1of1Type arg)          \
         :  _Base(                  \
               arg)                 \
      {                             \
      }                             \
                                    \
      explicit _Derived(            \
         _Arg1of2Type arg1,         \
         _Arg2of2Type arg2)         \
         :  _Base(                  \
               arg1,                \
               arg2)                \
      {                             \
      }                             \
}

#ifdef DECLARE_DERIVED_TEMPLATE_CLASS_1
#undef DECLARE_DERIVED_TEMPLATE_CLASS_1
#endif

#define DECLARE_DERIVED_TEMPLATE_CLASS_1(_Derived, _Args, _Base) template <typename _Args> class _Derived : public _Base<_Args> {}

///////////////////////////////////////////////////////////////////////////////
// End of file: DeclareSimpleDerivedClass.h
///////////////////////////////////////////////////////////////////////////////
