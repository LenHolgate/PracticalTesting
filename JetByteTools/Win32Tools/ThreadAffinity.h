#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_THREAD_AFFINITY_INCLUDED__
#define JETBYTE_TOOLS_WIN32_THREAD_AFFINITY_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ThreadAffinity.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2005 JetByte Limited.
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
         const SelectHow selectHow,
         const DWORD_PTR affinityMask = 0);

      explicit CThreadAffinity(
         const SelectHow selectHow,
         const DWORD_PTR affinityMask = 0);

      ~CThreadAffinity();

   private :

      const DWORD_PTR m_previousThreadAffinity;

      const HANDLE m_hThread;

      /// No copies do not implement
      CThreadAffinity(const CThreadAffinity &rhs);
      /// No copies do not implement
      CThreadAffinity &operator=(const CThreadAffinity &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_WIN32_THREAD_AFFINITY_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ThreadAffinity.h
///////////////////////////////////////////////////////////////////////////////

