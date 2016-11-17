#pragma once
#ifndef JETBYTE_TOOLS_WIN32_CRITICAL_SECTION_INCLUDED__
#define JETBYTE_TOOLS_WIN32_CRITICAL_SECTION_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CriticalSection.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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

#if (JETBYTE_DEPRECATE_CRITICAL_SECTION == 0)

#include <wtypes.h>

#include "ICriticalSection.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCriticalSection
///////////////////////////////////////////////////////////////////////////////

/// A class that wraps the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms682530.aspx">Critical Section API</a>
/// and that implements the ICriticalSection interface.
/// The addition of the ICriticalSection interface has increased the size of
/// this object and added a layer of indirection into the Enter() and Leave()
/// calls. If this proves to be a performance problem then we can simply create
/// a 'CSmallAndFastCriticalSection which doesn't inherit from the interface.
/// \ingroup Synchronization

class CCriticalSection : public ICriticalSection
{
   public :

      /// Creates a critical section object.

      CCriticalSection();

      /// Creates a critical section object and allows you to specify the spin
      /// count.

      explicit CCriticalSection(
         const DWORD spinCount);

      ~CCriticalSection();

      void SetSpinCount(
         const DWORD spinCount);

      // Implement ICriticalSection

      virtual bool TryEnter();

      virtual void Enter();

      virtual void Leave();

      virtual bool IsLockedByThisThread() const;

   private :

      CRITICAL_SECTION m_crit;

      /// No copies do not implement
      CCriticalSection(const CCriticalSection &rhs);
      /// No copies do not implement
      CCriticalSection &operator=(const CCriticalSection &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_DEPRECATE_CRITICAL_SECTION

#endif // JETBYTE_TOOLS_WIN32_CRITICAL_SECTION_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CriticalSection.h
///////////////////////////////////////////////////////////////////////////////
