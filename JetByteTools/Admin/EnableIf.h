#pragma once
#ifndef JETBYTE_TOOLS_ADMIN_ENABLE_IF_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_ENABLE_IF_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: EnableIf.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2017 JetByte Limited.
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

#endif // JETBYTE_TOOLS_ADMIN_ENABLE_IF_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: EnableIf.h
///////////////////////////////////////////////////////////////////////////////
