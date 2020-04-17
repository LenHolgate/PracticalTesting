#pragma once
#ifndef JETBYTE_TOOLS_WIN32_I_CRITICAL_SECTION_INCLUDED__
#define JETBYTE_TOOLS_WIN32_I_CRITICAL_SECTION_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: ICriticalSection.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// ICriticalSection
///////////////////////////////////////////////////////////////////////////////

/// An interface onto the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms682530.aspx">Critical
/// Section API</a>.
/// \ingroup Synchronization
/// \ingroup Interfaces

class ICriticalSection
{
   public :

      static void EnsureLockedByThisThread(
         const ICriticalSection &crit);

      static void EnsureIsNotLockedByThisThread(
         const ICriticalSection &crit);

      /// A class that takes ownership of an instance of ICriticalSection. That
      /// is it calls Enter() in the constructor and Leave() in the destructor
      /// and can therefore be used to support \ref RAII "scope based" locking
      /// and unlocking of instances of ICriticalSection.
      /// \ingroup Synchronization
      /// \ingroup RAII

      class Owner
      {
         public:

            explicit Owner(
               ICriticalSection &crit);

            Owner(
               const Owner &rhs) = delete;

            ~Owner();

            Owner &operator=(
               const Owner &rhs) = delete;

         private :

            ICriticalSection &m_crit;
      };

      /// A class that may take ownership of an instance of ICriticalSection.
      /// That is it calls Enter() in the constructor and Leave() in the
      /// destructor (but only if locked passed as true to the constructor)
      /// and can therefore be used to support \ref RAII "scope based" locking
      /// and unlocking of instances of ICriticalSection.
      /// \ingroup Synchronization
      /// \ingroup RAII

      class ConditionalOwner
      {
         public:

            explicit ConditionalOwner(
               ICriticalSection &crit,
               bool locked = true);

            ConditionalOwner(
               const ConditionalOwner &rhs) = delete;

            ~ConditionalOwner();

            ConditionalOwner &operator=(
               const ConditionalOwner &rhs) = delete;

            void Leave();

         private :

            ICriticalSection &m_crit;

            bool m_locked;
      };

      /// A class that may take ownership of an instance of ICriticalSection.
      /// If you call Enter() or TryEnter() on this class it will keep track
      /// of the fact that the critical section has been entered and will
      /// call Leave() in the destructor. This can therefore be used to
      /// support \ref RAII "scope based" locking and unlocking of instances
      /// of ICriticalSection.
      /// \ingroup Synchronization
      /// \ingroup RAII

      class PotentialOwner
      {
         public:

            explicit PotentialOwner(
               ICriticalSection &crit);

            PotentialOwner(
               const PotentialOwner &rhs) = delete;

            ~PotentialOwner();

            PotentialOwner &operator=(
               const PotentialOwner &rhs) = delete;

            void Enter();

            bool TryEnter();

            void Leave();

         private :

            ICriticalSection &m_crit;

            bool m_locked;
      };

      /// Try to enter the critical section and lock other threads outside,
      /// returns true if successful and false if some other thread already
      /// has the critical section locked1.

      virtual bool TryEnter() = 0;

      /// Enter the critical section and lock other threads outside.

      virtual void Enter() = 0;

      /// Leave the critical section and allow another thread to enter.

      virtual void Leave() = 0;

      /// Returns true if the critical section is currently locked by this
      /// thread.

      virtual bool IsLockedByThisThread() const = 0;

      /// Instances of this interface can be deleted by their users.

      virtual ~ICriticalSection() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_DEPRECATE_CRITICAL_SECTION

#endif // JETBYTE_TOOLS_WIN32_I_CRITICAL_SECTION_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: ICriticalSection.h
///////////////////////////////////////////////////////////////////////////////
