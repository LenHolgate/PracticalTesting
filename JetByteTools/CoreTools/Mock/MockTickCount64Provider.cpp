///////////////////////////////////////////////////////////////////////////////
// File: MockTickCount64Provider.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2008 JetByte Limited.
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

#include "MockTickCount64Provider.h"

#include "JetByteTools/CoreTools/ToString.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Mock {

///////////////////////////////////////////////////////////////////////////////
// CMockTickCount64Provider
///////////////////////////////////////////////////////////////////////////////

CMockTickCount64Provider::CMockTickCount64Provider()
   :  logMessages(true),
      logTickCount(true),
      m_tickCount(0)
{
}

CMockTickCount64Provider::CMockTickCount64Provider(
   const ULONGLONG tickCount)
   :  logMessages(true),
      logTickCount(true),
      m_tickCount(tickCount)
{
}

void CMockTickCount64Provider::SetTickCount(
   const ULONGLONG tickCount)
{
   m_tickCount = tickCount;
}

ULONGLONG CMockTickCount64Provider::GetTickCount64() const
{
   if (logMessages)
   {
      if (logTickCount)
      {
         LogMessage(_T("GetTickCount: ") + ToString(m_tickCount));
      }
      else
      {
         LogMessage(_T("GetTickCount"));
      }
   }

   return m_tickCount;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Mock
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Mock
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: MockTickCount64Provider.cpp
///////////////////////////////////////////////////////////////////////////////
