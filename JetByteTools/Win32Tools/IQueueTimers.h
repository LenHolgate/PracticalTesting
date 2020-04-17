#pragma once
#ifndef JETBYTE_TOOLS_I_QUEUE_TIMERS_INCLUDED__
#define JETBYTE_TOOLS_I_QUEUE_TIMERS_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: IQueueTimers.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "JetByteTools/Admin/Types.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// IQueueTimers
///////////////////////////////////////////////////////////////////////////////

/// An interface representing a class that manages timers that implement the
/// IQueueTimers::Timer interface and and which have their
/// IQueueTimers::Timer::OnTimer() method called when the the timer expires.
/// See <a href="http://www.lenholgate.com/archives/000389.html">here</a>
/// for more details.
/// \ingroup Timers
/// \ingroup Interfaces
/// \ingroup ProtectedDestructors

class IQueueTimers
{
   public :

      /// User data that can be passed to Timer via the OnTimer() call when
      /// the timeout expires.

      typedef ULONG_PTR UserData;

      /// The Timer interface.

      class Timer;

      /// A reference counted version of the timer interface.

      class RefCountedTimer;

      /// A handle to a timer that has been created. This can be passed to
      /// SetTimer(), CancelTimer() and DestroyTimer() and is created with
      /// CreateTimer().

      typedef ULONG_PTR Handle;

      /// The value that represents an invalid handle that cannot be used.

      static Handle InvalidHandleValue;

      /// Create a timer and return a Handle to it.

      virtual Handle CreateTimer() = 0;

      // TODO - potentially add a handle wrapper that can do the 'if set' and update stuff
      // this would avoid locking for AO's

      /// Returns true if the timer is currently set.

      virtual bool TimerIsSet(
         const Handle &handle) const = 0;

      /// Set a timer that was previously created with CreateTimer().
      /// Returns true if the timer was previously pending for another timeout and
      /// false if the timer was not already pending. Note that calling SetTimer()
      /// will cause any timers that have expired to be processed before the new
      /// timer is set.

      enum SetTimerIf
      {
         SetTimerAlways,
         SetTimerIfNotSet
      };

      virtual bool SetTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         SetTimerIf setTimerIf = SetTimerAlways) = 0;

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::SetTimerWithRefCountedUserData) Ignoring return value of function

      template <typename T>
      bool SetTimerWithRefCountedUserData(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         T *pUserData,
         SetTimerIf setTimerIf = SetTimerAlways);

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::SetTimerWithRefCountedTimer) Ignoring return value of function

      template <typename T>
      bool SetTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer,
         Milliseconds timeout,
         UserData userData,
         SetTimerIf setTimerIf = SetTimerAlways);

      /// Update a timer if it is set and if the condition is true and set the timer if it is not set.
      /// Updating a timer will set the timeout, timer and user data to the newly supplied values. If the timer
      /// is not updated because the condition is false then nothing is changed.
      /// UpdateAlways will always update.
      /// UpdateAlwaysNoTimeoutChange will always update JUST timer and user data.
      /// If you supply pWasUpdated then it is set to true if anything was changed and false if not.

      enum UpdateTimerIf
      {
         UpdateTimerIfNewTimeIsSooner,
         UpdateTimerIfNewTimeIsLater,
         UpdateAlways,
         UpdateAlwaysNoTimeoutChange,
      };

      virtual bool UpdateTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr) = 0;

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::SetTimerWithRefCountedUserData) Ignoring return value of function

      template <typename T>
      bool UpdateTimerWithRefCountedUserData(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         T *pUserData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr);

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::SetTimerWithRefCountedTimer) Ignoring return value of function

      template <typename T>
      bool UpdateTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer,
         Milliseconds timeout,
         UserData userData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr);

      /// Cancel a timer that was previously set with SetTimer().
      /// Returns true if the timer was pending and false if the timer was not pending.

      virtual bool CancelTimer(
         const Handle &handle) = 0;

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::CancelTimerWithRefCountedUserData) Ignoring return value of function

      template <typename T>
      bool CancelTimerWithRefCountedUserData(
         const Handle &handle,
         T &userData);

      template <typename T>
      bool CancelTimerWithRefCountedUserData(
         const Handle &handle,
         T *pUserData);

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::CancelTimerWithRefCountedTimer) Ignoring return value of function

      template <typename T>
      bool CancelTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer);

      /// Destroy a timer that was previously created with CreateTimer()
      /// and update the variable passed in to contain InvalidHandleValue.
      /// Note that it is not permitted to call DestroyHandle() on a
      /// handle that contains the InvalidHandleValue value and an exception
      /// is thrown in this case. Returns true if the timer was pending and
      /// false if the timer was not pending.

      virtual bool DestroyTimer(
         Handle &handle) = 0;

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::DestroyTimerWithRefCountedUserData) Ignoring return value of function

      template <typename T>
      bool DestroyTimerWithRefCountedUserData(
         Handle &handle,
         T &userData);

      template <typename T>
      bool DestroyTimerWithRefCountedUserData(
         Handle &handle,
         T *pUserData);

      //lint -esym(534, JetByteTools::Win32::IQueueTimers::DestroyTimerWithRefCountedTimer) Ignoring return value of function

      template <typename T>
      bool DestroyTimerWithRefCountedTimer(
         Handle &handle,
         T &timer);

      /// Destroy a timer that was previously created with CreateTimer().
      /// Returns true if the timer was pending and false if the timer was not pending.

      virtual bool DestroyTimer(
         const Handle &handle) = 0;

      template <typename T>
      bool DestroyTimerWithRefCountedUserData(
         const Handle &handle,
         T &userData);

      template <typename T>
      bool DestroyTimerWithRefCountedUserData(
         const Handle &handle,
         T *pUserData);

      template <typename T>
      bool DestroyTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer);

      /// Create and set a single use timer.
      /// Note that calling SetTimer() will cause any timers that have expired to be
      /// processed before the new timer is set.

      virtual void SetTimer(
         Timer &timer,
         Milliseconds timeout,
         UserData userData) = 0;

      /// Returns the maximum timeout value that can be set. Note that this may differ
      /// between instances of the objects that implement this interface.

      virtual Milliseconds GetMaximumTimeout() const = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~IQueueTimers() = default;
};

template <typename T>
bool IQueueTimers::SetTimerWithRefCountedUserData(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   T *pUserData,
   const SetTimerIf setTimerIf)
{
   pUserData->AddRef();

   try
   {
      const bool wasPending = SetTimer(
         handle,
         timer,
         timeout,
         reinterpret_cast<UserData>(pUserData),
         setTimerIf);

      if (wasPending)
      {
         pUserData->Release();
      }

      return wasPending;
   }
   catch (...)
   {
      pUserData->Release();

      throw;
   }
}

template <typename T>
bool IQueueTimers::SetTimerWithRefCountedTimer(
   const Handle &handle,
   T &timer,
   const Milliseconds timeout,
   const UserData userData,
   const SetTimerIf setTimerIf)
{
   timer.AddRef();

   try
   {
      const bool wasPending = SetTimer(
         handle,
         timer,
         timeout,
         userData,
         setTimerIf);

      if (wasPending)
      {
         timer.Release();
      }

      return wasPending;
   }
   catch (...)
   {
      timer.Release();

      throw;
   }
}

template <typename T>
bool IQueueTimers::UpdateTimerWithRefCountedUserData(
   const Handle &handle,
   Timer &timer,
   const Milliseconds timeout,
   T *pUserData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated)
{
   pUserData->AddRef();

   try
   {
      const bool wasPending = UpdateTimer(
         handle,
         timer,
         timeout,
         reinterpret_cast<UserData>(pUserData),
         updateIf,
         pWasUpdated);

      if (wasPending)
      {
         pUserData->Release();
      }

      return wasPending;
   }
   catch (...)
   {
      pUserData->Release();

      throw;
   }
}

template <typename T>
bool IQueueTimers::UpdateTimerWithRefCountedTimer(
   const Handle &handle,
   T &timer,
   const Milliseconds timeout,
   const UserData userData,
   const UpdateTimerIf updateIf,
   bool *pWasUpdated)
{
   timer.AddRef();

   try
   {
      const bool wasPending = UpdateTimer(
         handle,
         timer,
         timeout,
         userData,
         updateIf,
         pWasUpdated);

      if (wasPending)
      {
         timer.Release();
      }

      return wasPending;
   }
   catch (...)
   {
      timer.Release();

      throw;
   }
}

template <typename T>
bool IQueueTimers::CancelTimerWithRefCountedUserData(
   const Handle &handle,
   T *pUserData)
{
   const bool wasPending = CancelTimer(handle);

   if (wasPending)
   {
      pUserData->Release();
   }

   return wasPending;
}

template <typename T>
bool IQueueTimers::CancelTimerWithRefCountedUserData(
   const Handle &handle,
   T &userData)
{
   return CancelTimerWithRefCountedUserData(handle, &userData);
}

template <typename T>
bool IQueueTimers::CancelTimerWithRefCountedTimer(
   const Handle &handle,
   T &timer)
{
   return CancelTimerWithRefCountedUserData(handle, &timer);
}

template <typename T>
bool IQueueTimers::DestroyTimerWithRefCountedUserData(
   Handle &handle,
   T *pUserData)
{
   const bool wasPending = DestroyTimer(handle);

   if (wasPending)
   {
      pUserData->Release();
   }

   return wasPending;
}

template <typename T>
bool IQueueTimers::DestroyTimerWithRefCountedUserData(
   Handle &handle,
   T &userData)
{
   return DestroyTimerWithRefCountedUserData(handle, &userData);
}

template <typename T>
bool IQueueTimers::DestroyTimerWithRefCountedTimer(
   Handle &handle,
   T &timer)
{
   return DestroyTimerWithRefCountedUserData(handle, &timer);
}

template <typename T>
bool IQueueTimers::DestroyTimerWithRefCountedUserData(
   const Handle &handle,
   T *pUserData)
{
   const bool wasPending = DestroyTimer(handle);

   if (wasPending)
   {
      pUserData->Release();
   }

   return wasPending;
}

template <typename T>
bool IQueueTimers::DestroyTimerWithRefCountedUserData(
   const Handle &handle,
   T &userData)
{
   return DestroyTimerWithRefCountedUserData(handle, &userData);
}

template <typename T>
bool IQueueTimers::DestroyTimerWithRefCountedTimer(
   const Handle &handle,
   T &timer)
{
   return DestroyTimerWithRefCountedUserData(handle, &timer);
}

///////////////////////////////////////////////////////////////////////////////
// IQueueTimers::Timer
///////////////////////////////////////////////////////////////////////////////

/// An interface to a timer that can be set with IQueueTimers.

class IQueueTimers::Timer
{
   public :

      /// User data that can be passed to Timer via the OnTimer() call when
      /// the timeout expires.

      typedef IQueueTimers::UserData UserData;

      /// Called after the timer expires.

      virtual void OnTimer(
         UserData userData) = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~Timer() = default;
};

class IQueueTimers::RefCountedTimer : public Timer
{
   public :

      virtual void AddRef() = 0;

      virtual void Release() = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~RefCountedTimer() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // JETBYTE_TOOLS_I_QUEUE_TIMERS_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: IQueueTimers.h
///////////////////////////////////////////////////////////////////////////////
