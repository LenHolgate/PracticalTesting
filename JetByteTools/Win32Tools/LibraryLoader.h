#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: LibraryLoader.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2004 JetByte Limited.
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

#include <wtypes.h>

#include "JetByteTools/CoreTools/tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CLibraryLoader
///////////////////////////////////////////////////////////////////////////////

/// A class which dynamically loads dlls.
/// \ingroup Win32

class CLibraryLoader
{
   public :

      /// Creates a library loader that does not automatically load a dll.

      CLibraryLoader();

      /// Creates a library loader and loads the specified dll.

      explicit CLibraryLoader(
         const JetByteTools::Core::_tstring &fileName,
         bool failWithException = true);

      /// Creates a library loader and attaches it to the supplied module
      /// handle.

      explicit CLibraryLoader(
         HMODULE hModule);

      CLibraryLoader(
         const CLibraryLoader &rhs) = delete;

      /// Frees any resources.

      ~CLibraryLoader();

      CLibraryLoader &operator=(
         const CLibraryLoader &rhs) = delete;

      /// Returns false if no library is loaded.

      bool IsLoaded() const;

      /// Access the HMODULE...

      HMODULE GetHMODULE() const;

      /// Loads the supplied library. If run is false then the library is loaded
      /// with the DONT_RESOLVE_DLL_REFERENCES flag.

      void LoadLibrary(
         const JetByteTools::Core::_tstring &fileName,
         bool run = true);

      /// Loads the supplied library. If run is false then the library is loaded
      /// with the DONT_RESOLVE_DLL_REFERENCES flag. Returns false if the library
      /// cannot be loaded

      bool LoadIfPossible(
         const JetByteTools::Core::_tstring &fileName,
         bool run = true);

      /// Looks up a function by name in the loaded library and returns a pointer
      /// to it. Returns null if the function doesn't exist in the library.

      FARPROC GetOptionalProcAddress(
         const JetByteTools::Core::_tstring &functionName) const;

      /// Looks up a function by name in the loaded library and returns a pointer
      /// to it. Throws an exception if the function doesn't exist in the library.

      FARPROC GetProcAddress(
         const JetByteTools::Core::_tstring &functionName) const;

      /// Returns true if the function exists in the library.

      bool IsValidProcAddress(
         const JetByteTools::Core::_tstring &functionName) const;

      /// Returns the function address as an offset relative to the library's base
      /// address.

      ULONG_PTR GetProcAddressOffset(
         const JetByteTools::Core::_tstring &functionName) const;

      /// Returns the functions address from the loaded library as a pointer into
      /// the supplied hModule. Use this if you want to locate a function in a
      /// module that has been loaded at a different base address to the base
      /// address that the library loaded by the library loader was loaded at.
      /// Usually used for cross process library function location...

      FARPROC GetProcAddressInThisModule(
         const JetByteTools::Core::_tstring &functionName,
         HMODULE hModule) const;

      /// Frees the library.

      void FreeLibrary();

   private :

      HMODULE m_hModule;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LibraryLoader.h
///////////////////////////////////////////////////////////////////////////////
