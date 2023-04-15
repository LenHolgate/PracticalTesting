#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: Warnings.h
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

#pragma warning(disable: 4355)   // 'this': used in base member initializer list

//#pragma warning(disable: 4373)   // 'x': virtual function overrides 'y', previous versions of the compiler
                                 // (VS2008 and earlier) did not override when parameters only differed by
                                 // const/volatile qualifiers

#pragma warning(disable: 4201)   // nonstandard extension used : nameless struct/union

#pragma warning(disable: 4503)   // decorated name length exceeded, name was truncated

#pragma warning(disable: 4505)   // unreferenced local function has been removed

#pragma warning(disable: 4510)   // default constructor could not be generated
#pragma warning(disable: 4511)   // copy constructor could not be generated
#pragma warning(disable: 4512)   // assignment operator could not be generated
#pragma warning(disable: 4514)   // unreferenced inline function has been removed
#pragma warning(disable: 4610)   // can never be instantiated - user defined constructor required

#pragma warning(disable: 4702)   // unreachable code
#pragma warning(disable: 4710)   // function 'x' not inlined
#pragma warning(disable: 4786)   // identifier was truncated to '255' characters in the debug information

// Things we need to disable to be able to compile with /Wall

#pragma warning(disable: 4548)   // expression before comma has no effect; expected expression with side-effect
#pragma warning(disable: 4820)   // 'x' bytes padding added after data member 'y'
#pragma warning(disable: 4265)   // class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4668)   // 'x' is not defined as a preprocessor macro, replaceing with '0' for '#if/#elif'
#pragma warning(disable: 4619)   // #pragma warning : there is no warning number 'x'
#pragma warning(disable: 4571)   // catch (...) semantics changed since VC 7.1; SEH are no longer caught
#pragma warning(disable: 4917)   // 'x' : a GUID can only be associated with a class, interface or namespace
#pragma warning(disable: 4365)   // conversion from 'x' to 'y', signed/unsigned mismatch
#pragma warning(disable: 4640)   // 'x' : construction of local static object is not thread-safe
#pragma warning(disable: 4625)   // 'x' : copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626)   // 'x' : assignment operator could not be generated because a base class assignment operator is inaccessible

// This is needed for VS2005 without any platform sdk, basestd.h is full of these..

#pragma warning(disable: 4826)   // Conversion from 'x' to 'y' is sign-extended. This may cause unexpected runtime behavior.
#pragma warning(disable: 4191)   // type cast' : unsafe conversion from 'x' to 'y' Calling this function through the result pointer may cause your program to fail

#pragma warning(disable: 4370)   // layout of class has changed from a previous version of the compiler due to better packing
#pragma warning(disable: 4351)   // new behavior: elements of array 'x' will be default initialized

#pragma warning(disable: 4555)   // expression has no effect; expected expression with side-effect
#pragma warning(disable: 4574)   // 'x' is defined to be '0': did you mean to use '#if x'?

#pragma warning(disable: 4324)   // 'x' : structure was padded due to __declspec(align())

// In atlconv.h, atlalloc.h, etc
#pragma warning(disable: 4987)   // nonstandard extension used: 'throw (...)'

#pragma warning(disable: 4711)   // function 'x' selected for automatic inline expansion

#pragma warning(disable: 4748)   // /GS can not protect parameters and local variables
                                 // from local buffer overrun because optimizations
                                 // are disabled in function

#pragma warning(disable: 4200)   // nonstandard extension used : zero-sized array in struct/union
                                 // Cannot generate copy-ctor or copy-assignment operator when UDT
                                 // contains a zero-sized array

#pragma warning(disable: 4350)   // behavior change: 'x' called instead of 'y'
#pragma warning(disable: 4005)   // 'x' macro redefinition

//#pragma warning(disable: 4615)   // #pragma warning : unknown user warning type
//#pragma warning(disable: 4616)   // warning number 'x' out of range, must be between 'y' and 'z'

#pragma warning(disable: 6322)   // Except block empty.

#pragma warning(disable: 4435)   // 'x' : Object layout under /vd2 will change due to virtual base 'y'

#pragma warning(disable: 4091)   // 'typedef ': ignored on left of 'x' when no variable is declared
#pragma warning(disable: 4456)   // declaration of 'x' hides previous local declaration
#pragma warning(disable: 4457)   // declaration of 'x' hides function parameter
#pragma warning(disable: 4458)   // declaration of 'x' hides class member
#pragma warning(disable: 4623)   // 'x': default constructor was implicitly defined as deleted
#pragma warning(disable: 4774)   // 'x' : format string expected in argument 3 is not a string literal
#pragma warning(disable: 5025)   // 'x': move assignment operator was implicitly defined as deleted
#pragma warning(disable: 5026)   // 'x': move constructor was implicitly defined as deleted because a base class move constructor is inaccessible or deleted
#pragma warning(disable: 5027)   // 'x': move assignment operator was implicitly defined as deleted because a base class move assignment operator is inaccessible or deleted

#if _MSC_VER >= 1911
#pragma warning(disable: 4768)   // __declspec attributes before linkage specification are ignored
#pragma warning(disable: 5045)   // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning(disable: 4868)   // compiler may not enforce left-to-right evaluation order in braced initializer list
#endif

#if _MSC_VER >= 1910
#pragma warning(disable: 5031)   // #pragma warning(pop): likely mismatch, popping warning state pushed in different file (compiling source file 'x')
#pragma warning(disable: 5032)   // #pragma warning(push) with no corresponding #pragma warning(pop) (compiling source file 'x')
#endif

#if _MSC_VER >= 1925
#pragma warning(disable: 5204)   // 'x': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly 
#endif

#pragma warning(disable: 5220)   // 'x': a non-static data member with a volatile qualified type no longer implies that compiler generated copy/move constructors and copy/move assignment operators are not trivial 

#pragma warning(disable: 4371)   // 'x': layout of class may have changed from a previous version of the compiler due to better packing of member 'y'

#pragma warning(disable: 4189)   // 'x': local variable is initialized but not referenced
#pragma warning(disable: 5246)   // 'x': the initialization of a subobject should be wrapped in braces (compiling source file y)
#pragma warning(disable: 5240)   // 'x': attribute is ignored in this syntactic position (compiling source file y)

#pragma warning(disable: 5262)   // implicit fall-through occurs here; are you missing a break statement? Use [[fallthrough]] when a break statement is intentionally omitted between cases
#pragma warning(disable: 5264)   // 'x': 'const' variable is not used

#pragma warning(disable: 6387)   // 'x' could be '0'.

#pragma warning(disable: 26110)  // Caller failing to hold lock <y> before calling function <x>.
#pragma warning(disable: 28159)  // Consider using another function instead (GetTickCount vs GetTickCount64)
#pragma warning(disable: 26135)  // Missing locking annotation
#pragma warning(disable: 26165)  // Possibly failing to release lock
#pragma warning(disable: 26167)  // Possibly releasing unheld lock
#pragma warning(disable: 26401)  // Do not delete a raw pointer that is not an owner<T> (i.11).
#pragma warning(disable: 26403)  // Reset or explicitly delete an owner<T> pointer 'x' (r.3).
#pragma warning(disable: 26409)  // Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
#pragma warning(disable: 26415)  // Smart pointer parameter 'x' is used only to access contained pointer. Use T* or T& instead (r.30).
#pragma warning(disable: 26417)  // Shared pointer parameter 'x' is passed by reference and not reset or reassigned. Use T* or T& instead (r.35).
#pragma warning(disable: 26418)  // Shared pointer parameter 'x' is not copied or moved. Use T* or T& instead (r.36).
#pragma warning(disable: 26426)  // Global initializer calls a non-constexpr function 'x' (i.22).
#pragma warning(disable: 26432)  // If you define or delete any default operation in the type 'x', define or delete them all (c.21).
#pragma warning(disable: 26433)  // If you define or delete any default operation in the type 'x', define or delete them all (c.21).
#pragma warning(disable: 26436)  // The type x' with a virtual function needs either public virtual or protected non-virtual destructor (c.35).
#pragma warning(disable: 26439)  // This kind of function may not throw. Declare it 'noexcept' (f.6).
#pragma warning(disable: 26440)  // Function 'x' can be declared 'noexcept' (f.6).
//#pragma warning(disable: 26443)  // Overriding destructor should not use explicit 'override' or 'virtual' specifiers (c.128).
#pragma warning(disable: 26446)  // Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
#pragma warning(disable: 26447)  // The function is declared 'noexcept' but calls function 'x' which may throw exceptions (f.6).

#pragma warning(disable: 26451)  // Arithmetic overflow: Using operator '+' on a 4 byte value and then casting the result to a 8 byte value. Cast the value to the wider type before calling operator '+' to avoid overflow (io.2).

#pragma warning(disable: 26455)  // Default constructor may not throw. Declare it 'noexcept' (f.6).
#pragma warning(disable: 26460)  // The reference argument 'x' for function 'y' can be marked as const (con.3).
#pragma warning(disable: 26461)  // The pointer argument 'x' for function 'y' can be marked as a pointer to const (con.3).
#pragma warning(disable: 26471)  // Don't use reinterpret_cast. A cast from void* can use static_cast (type.1).
#pragma warning(disable: 26472)  // Don't use a static_cast for arithmetic conversions. Use brace initialization, gsl::narrow_cast or gsl::narrow (type.1).
#pragma warning(disable: 26474)  // Don't cast between pointer types when the conversion could be implicit (type.1).
#pragma warning(disable: 26481)  // Don't use pointer arithmetic. Use span instead (bounds.1).
#pragma warning(disable: 26490)  // Don't use const_cast to cast away const (type.3).
#pragma warning(disable: 26491)  // Don't use static_cast downcasts (type.2).
#pragma warning(disable: 26492)  // Don't use reinterpret_cast (type.1).
#pragma warning(disable: 26496)  // The variable 'x' is assigned only once, mark it as const (con.4).
#pragma warning(disable: 26497)  // The function 'x' could be marked constexpr if compile-time evaluation is desired (f.4).
#pragma warning(disable: 26812)  // The enum type 'x' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).
#pragma warning(disable: 26814)  // The const variable 'x' can be computed at compile-time. Consider using constexpr (con.5).
#pragma warning(disable: 28204)  // Only one of this overload is annotated
#pragma warning(disable: 28251)  // Inconsistent annotation for function
#pragma warning(disable: 28252)  // Inconsistent annotation for function
#pragma warning(disable: 28253)  // Inconsistent annotation for function

#pragma warning(disable: 26485) // Expression 'x': No array to pointer decay (bounds.3).
#pragma warning(disable: 26457) // (void) should not be used to ignore return values, use 'std::ignore =' instead (es.48).
#pragma warning(disable: 26473) // Don't cast between pointer types where the source type and the target type are the same (type.1).
#pragma warning(disable: 26429) // Symbol 'pStore' is never tested for nullness, it can be marked as not_null (f.23).
#pragma warning(disable: 26482) // Only index into arrays using constant expressions (bounds.2).
#pragma warning(disable: 26434) // Function 'x' hides a non-virtual function 'y'.

#pragma warning(disable: 6246) // Local declaration of 'x' hides declaration of the same name in outer scope.

// The following macro "SuppressNoSymbolsFoundWarning()" can be put into a file
// in order suppress the MS Visual C++ Linker warning 4221
//
// warning LNK4221: no public symbols found; archive member will be inaccessible
//
// This warning occurs when a file has no externally visible symbols which may be
// dependent on configuration #defines and options.

#define SuppressNoSymbolsFoundWarning()   namespace { char NoEmptyFileDummy##__LINE__; }

#define JETBYTE_WARNING_SUPPRESS_PUSH \
__pragma(warning(push))

#define JETBYTE_WARNING_SUPPRESS_ENUM_VALUE_NOT_HANDLED_IN_SWITCH \
JETBYTE_WARNING_SUPPRESS_PUSH \
__pragma(warning(disable : 4061))     /* enumerator 'X' in switch of enum 'Y' is not explicitly handled by a case label */

#define JETBYTE_WARNING_SUPPRESS_DTOR_THROWS \
JETBYTE_WARNING_SUPPRESS_PUSH \
__pragma(warning(disable : 4297))     /* function assumed not to throw an exception but does */ \
__pragma(warning(disable : 4722))     /* destructor never returns, potential memory leak */

#define JETBYTE_WARNING_SUPPRESS_NOT_A_STRING_LITERAL \
JETBYTE_WARNING_SUPPRESS_PUSH \
__pragma(warning(disable : 4774))      // format string expected in argument 3 is not a string literal

#define JETBYTE_WARNING_SUPPRESS_UNHANDLED_SWITCH_ENUM \
JETBYTE_WARNING_SUPPRESS_PUSH \
__pragma(warning(disable: 4061))
__pragma(warning(disable: 26818))   // Switch statement does not cover all cases. Consider adding a 'default' label (es.79)

#define JETBYTE_WARNING_SUPPRESS_NULL_POINTER_ARITHMETIC \
JETBYTE_WARNING_SUPPRESS_PUSH

#define JETBYTE_WARNING_SUPPRESS_IMPLICIT_FALLTHROUGH \
JETBYTE_WARNING_SUPPRESS_PUSH
__pragma(warning(disable: 26819))   // Unannotated fallthrough between switch labels (es.78).

#define JETBYTE_WARNING_SUPPRESS_POP \
__pragma(warning(pop))

///////////////////////////////////////////////////////////////////////////////
// End of file: Warnings.h
///////////////////////////////////////////////////////////////////////////////
