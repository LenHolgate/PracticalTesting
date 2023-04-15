#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: IMonitorCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2010 JetByte Limited.
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// IMonitorCallbackTimerQueue
///////////////////////////////////////////////////////////////////////////////

/// An interface to allow a class to monitor the operation of an instance of
/// CCallbackTimerQueue.
/// \ingroup Interfaces
/// \ingroup ProtectedDestructors
/// \ingroup Monitoring

class IMonitorCallbackTimerQueue
{
   public :

      virtual void OnTimerCreated() = 0;

      virtual void OnOneOffTimerSet() = 0;

      virtual void OnTimerSet(
         bool wasPending) = 0;

      virtual void OnTimerUpdated(
         bool wasPending,
         bool wasUpdated) = 0;

      virtual void OnTimer() = 0;

      virtual void OnTimerCancelled(
         bool wasPending) = 0;

      virtual void OnTimerDestroyed(
         bool wasPending) = 0;

      virtual void OnTimerDeleted() = 0;

   protected :

      /// We never delete instances of this interface; you must manage the
      /// lifetime of the class that implements it.

      virtual ~IMonitorCallbackTimerQueue() = default;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: IMonitorCallbackTimerQueue.h
///////////////////////////////////////////////////////////////////////////////
