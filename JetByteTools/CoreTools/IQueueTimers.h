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

      /// Set a timer that was previously created with CreateTimer().
      /// Returns true if the timer was previously pending for another timeout and
      /// false if the timer was not already pending. Note that calling SetTimer()
      /// will cause any timers that have expired to be processed before the new
      /// timer is set.

      virtual bool SetTimer(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         UserData userData) = 0;


      template <typename T>
      bool SetTimerWithRefCountedUserData(
         const Handle &handle,
         Timer &timer,
         Milliseconds timeout,
         T *pUserData);


      template <typename T>
      bool SetTimerWithRefCountedTimer(
         const Handle &handle,
         T &timer,
         Milliseconds timeout,
         UserData userData);

      /// Cancel a timer that was previously set with SetTimer().
      /// Returns true if the timer was pending and false if the timer was not pending.

      virtual bool CancelTimer(
         const Handle &handle) = 0;


      template <typename T>
      bool CancelTimerWithRefCountedUserData(
         const Handle &handle,
         T &userData);

      template <typename T>
      bool CancelTimerWithRefCountedUserData(
         const Handle &handle,
         T *pUserData);


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


      template <typename T>
      bool DestroyTimerWithRefCountedUserData(
         Handle &handle,
         T &userData);

      template <typename T>
      bool DestroyTimerWithRefCountedUserData(
         Handle &handle,
         T *pUserData);


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
   T *pUserData)
{
   pUserData->AddRef();

   try
   {
      const bool wasPending = SetTimer(
         handle,
         timer,
         timeout,
         reinterpret_cast<UserData>(pUserData));

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
   const UserData userData)
{
   timer.AddRef();

   try
   {
      const bool wasPending = SetTimer(
         handle,
         timer,
         timeout,
         userData);

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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IQueueTimers.h
///////////////////////////////////////////////////////////////////////////////
