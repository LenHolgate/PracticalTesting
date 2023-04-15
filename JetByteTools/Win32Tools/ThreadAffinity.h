#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: ThreadAffinity.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2005 JetByte Limited.
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CThreadAffinity
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms686247.aspx">SetThreadAffinityMask
/// API</a>.
/// \ingroup Threading

class CThreadAffinity
{
   public :

      enum SelectHow
      {
         DontChangeAffinity,
         SelectSingleProcessor,
         UseAffinityMask
      };

      CThreadAffinity(
         HANDLE hThread,
         SelectHow selectHow,
         DWORD_PTR affinityMask = 0);

      explicit CThreadAffinity(
         SelectHow selectHow,
         DWORD_PTR affinityMask = 0);

      CThreadAffinity(
         const CThreadAffinity &rhs) = delete;

      ~CThreadAffinity();

      CThreadAffinity &operator=(
         const CThreadAffinity &rhs) = delete;

   private :

      const DWORD_PTR m_previousThreadAffinity;

      HANDLE m_hThread;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadAffinity.h
///////////////////////////////////////////////////////////////////////////////

