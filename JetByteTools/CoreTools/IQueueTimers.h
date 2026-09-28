#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IQueueTimers.h
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

#include "Types.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// IQueueTimers
///////////////////////////////////////////////////////////////////////////////

/// An interface representing a class that manages timers that implement the
/// IQueueTimers::Timer interface and which have their
/// IQueueTimers::Timer::OnTimer() method called when the timer expires.
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

      enum SetTimerIf : BYTE
      {
         SetTimerAlways,
         SetTimerIfNotSet
      };

      virtual bool SetTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         SetTimerIf setTimerIf = SetTimerAlways,
         bool *pOptionalFirstToExpireHasChanged = nullptr) = 0;

      template <typename T>
      bool SetTimerWithRefCountedUserData(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         T *pUserData,
         SetTimerIf setTimerIf = SetTimerAlways,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      template <typename T>
      bool SetTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer,
         Milliseconds timeout,
         UserData userData,
         SetTimerIf setTimerIf = SetTimerAlways,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      /// Update a timer if it is set and if the condition is true and set the timer if it is not set.
      /// Updating a timer will set the timeout, timer and user data to the newly supplied values. If the timer
      /// is not updated because the condition is false then nothing is changed.
      /// UpdateAlways will always update.
      /// UpdateAlwaysNoTimeoutChange will always update JUST timer and user data.
      /// If you supply pWasUpdated then it is set to true if anything was changed and false if not.

      enum UpdateTimerIf : BYTE
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
         bool *pWasUpdated = nullptr,
         bool *pOptionalFirstToExpireHasChanged = nullptr) = 0;

      template <typename T>
      bool UpdateTimerWithRefCountedUserData(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         T *pUserData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      template <typename T>
      bool UpdateTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer,
         Milliseconds timeout,
         UserData userData,
         UpdateTimerIf updateIf,
         bool *pWasUpdated = nullptr,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      /// Cancel a timer that was previously set with SetTimer().
      /// Returns true if the timer was pending and false if the timer was not pending.

      virtual bool CancelTimer(
         const Handle &handle,
         bool *pOptionalFirstToExpireHasChanged = nullptr) = 0;

      template <typename T>
      bool CancelTimerWithRefCountedUserData(
         const Handle &handle,
         T &userData,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      template <typename T>
      bool CancelTimerWithRefCountedUserData(
         const Handle &handle,
         T *pUserData,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      template <typename T>
      bool CancelTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      /// Destroy a timer that was previously created with CreateTimer()
      /// and update the variable passed in to contain InvalidHandleValue.
      /// Note that it is not permitted to call DestroyHandle() on a
      /// handle that contains the InvalidHandleValue value and an exception
      /// is thrown in this case. Returns true if the timer was pending and
      /// false if the timer was not pending.

      virtual bool DestroyTimer(
         Handle &handle,
         bool *pOptionalFirstToExpireHasChanged = nullptr) = 0;

      template <typename T, typename H>
      bool DestroyTimerWithRefCountedUserData(
         H &handle,
         T &userData,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      template <typename T, typename H>
      bool DestroyTimerWithRefCountedUserData(
         H &handle,
         T *pUserData,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      template <typename T, typename H>
      bool DestroyTimerWithRefCountedTimer(
         H &handle,
         T &timer,
         bool *pOptionalFirstToExpireHasChanged = nullptr);

      /// Destroy a timer that was previously created with CreateTimer().
      /// Returns true if the timer was pending and false if the timer was not pending.

      virtual bool DestroyTimer(
         const Handle &handle,
         bool *pOptionalFirstToExpireHasChanged = nullptr)
      {
         Handle _handle = handle;

         return DestroyTimer(_handle, pOptionalFirstToExpireHasChanged);
      }

      /// Create and set a single use timer.
      /// Note that calling SetTimer() will cause any timers that have expired to be
      /// processed before the new timer is set.

      virtual void SetTimer(
         Timer &timer,
         Milliseconds timeout,
         UserData userData,
         bool *pOptionalFirstToExpireHasChanged = nullptr) = 0;

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
   const SetTimerIf setTimerIf,
   bool *pOptionalFirstToExpireHasChanged)
{
   pUserData->AddRef();

   try
   {
      const bool wasPending = SetTimer(
         handle,
         timer,
         timeout,
         reinterpret_cast<UserData>(pUserData),
         setTimerIf,
         pOptionalFirstToExpireHasChanged);

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
   const SetTimerIf setTimerIf,
   bool *pOptionalFirstToExpireHasChanged)
{
   timer.AddRef();

   try
   {
      const bool wasPending = SetTimer(
         handle,
         timer,
         timeout,
         userData,
         setTimerIf,
         pOptionalFirstToExpireHasChanged);

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
   bool *pWasUpdated,
   bool *pOptionalFirstToExpireHasChanged)
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
         pWasUpdated,
         pOptionalFirstToExpireHasChanged);

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
   bool *pWasUpdated,
   bool *pOptionalFirstToExpireHasChanged)
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
         pWasUpdated,
         pOptionalFirstToExpireHasChanged);

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
   T *pUserData,
   bool *pOptionalFirstToExpireHasChanged)
{
   const bool wasPending = CancelTimer(handle, pOptionalFirstToExpireHasChanged);

   if (wasPending)
   {
      pUserData->Release();
   }

   return wasPending;
}

template <typename T>
bool IQueueTimers::CancelTimerWithRefCountedUserData(
   const Handle &handle,
   T &userData,
   bool *pOptionalFirstToExpireHasChanged)
{
   return CancelTimerWithRefCountedUserData(handle, &userData, pOptionalFirstToExpireHasChanged);
}

template <typename T>
bool IQueueTimers::CancelTimerWithRefCountedTimer(
   const Handle &handle,
   T &timer,
   bool *pOptionalFirstToExpireHasChanged)
{
   return CancelTimerWithRefCountedUserData(handle, &timer, pOptionalFirstToExpireHasChanged);
}

template <typename T, typename H>
bool IQueueTimers::DestroyTimerWithRefCountedUserData(
   H &handle,
   T *pUserData,
   bool *pOptionalFirstToExpireHasChanged)
{
   const bool wasPending = DestroyTimer(handle, pOptionalFirstToExpireHasChanged);

   if (wasPending)
   {
      pUserData->Release();
   }

   return wasPending;
}

template <typename T, typename H>
bool IQueueTimers::DestroyTimerWithRefCountedUserData(
   H &handle,
   T &userData,
   bool *pOptionalFirstToExpireHasChanged)
{
   return DestroyTimerWithRefCountedUserData(handle, &userData, pOptionalFirstToExpireHasChanged);
}

template <typename T, typename H>
bool IQueueTimers::DestroyTimerWithRefCountedTimer(
   H &handle,
   T &timer,
   bool *pOptionalFirstToExpireHasChanged)
{
   return DestroyTimerWithRefCountedUserData(handle, &timer, pOptionalFirstToExpireHasChanged);
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

      typedef IQueueTimers::Handle Handle;

      Timer() = default;

      Timer(
         const Timer &rhs) = default;

      Timer &operator=(
         const Timer &rhs) = default;

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

      ~RefCountedTimer() override = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IQueueTimers.h
///////////////////////////////////////////////////////////////////////////////
