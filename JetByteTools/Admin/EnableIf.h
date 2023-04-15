#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: EnableIf.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2017 JetByte Limited.
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

// SFINAE
// See: https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence

// We could use the xtr1common header to pull in std::enable_if on all compilers
// that we currently support but it's trivial to implement and the header pulls in
// lots more stuff...

template<bool C, typename T = void>
struct jb_enable_if {
   typedef T type;
};

template<typename T>
struct jb_enable_if<false, T> { };

// This can be much neater once all compilers support decltype

#define JETBYTE_HAS_MEMBER_FUNCTION(func, name)                                  \
    template<typename T1, typename Sign>                                         \
    struct name {                                                                \
        typedef char yes[1];                                                     \
        typedef char no [2];                                                     \
        template <typename U, U> struct type_check;                              \
        template <typename _1> static yes &chk(type_check<Sign, &_1::func > *);  \
        template <typename   > static no  &chk(...);                             \
        static bool const value = sizeof(chk<T1>(0)) == sizeof(yes);             \
    }

///////////////////////////////////////////////////////////////////////////////
// End of file: EnableIf.h
///////////////////////////////////////////////////////////////////////////////
