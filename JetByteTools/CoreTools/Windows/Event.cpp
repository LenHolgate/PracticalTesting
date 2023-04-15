///////////////////////////////////////////////////////////////////////////////
// File: Event.cpp
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

#include "JetByteTools/Admin/Admin.h"

#include "Event.h"

#include "JetByteTools/CoreTools/Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

inline JetByteTools::Win32::CEvent::ResetType MapResetType(
   const CEvent::ResetType resetType)
{
   if (resetType == CEvent::ResetType::ManualReset)
   {
      return JetByteTools::Win32::CEvent::ResetType::ManualReset;
   }

   if (resetType == CEvent::ResetType::AutoReset)
   {
      return JetByteTools::Win32::CEvent::ResetType::AutoReset;
   }

   throw CException(
      _T("CEvent::MapResetType()"),
      _T("Unexpected reset type"));
}

inline JetByteTools::Win32::CEvent::InitialState MapInitialState(
   const CEvent::InitialState initialState)
{
   if (initialState == CEvent::InitialState::Signaled)
   {
      return JetByteTools::Win32::CEvent::InitialState::Signaled;
   }

   if (initialState == CEvent::InitialState::NonSignaled)
   {
      return JetByteTools::Win32::CEvent::InitialState::NonSignaled;
   }

   throw CException(
      _T("CEvent::MapInitialState()"),
      _T("Unexpected initial state"));
}

///////////////////////////////////////////////////////////////////////////////
// CEvent
///////////////////////////////////////////////////////////////////////////////

CEvent::CEvent(
   const ResetType resetType,
   const InitialState initialState)
   :  m_event(
         nullptr,
         MapResetType(resetType),
         MapInitialState(initialState))
{
}

void CEvent::Reset()
{
   m_event.Reset();
}

void CEvent::Set()
{
   m_event.Set();
}

HANDLE CEvent::GetWaitHandle() const
{
   return m_event.GetWaitHandle();
}

void CEvent::Wait() const
{
   m_event.Wait();
}

bool CEvent::Wait(
   const Milliseconds timeout) const
{
   return m_event.Wait(timeout);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: Event.cpp
///////////////////////////////////////////////////////////////////////////////
