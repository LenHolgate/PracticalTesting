#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: TickCount64Provider.h
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

#include <Windows.h>             // For GetTickCount()

#include "JetByteTools/CoreTools/IProvideTickCount64.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {
namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// CTickCount64Provider
///////////////////////////////////////////////////////////////////////////////

/// A class that implements IProvideTickCount64 and returns the tick count
/// directly from a call to the operating system
/// <a href="http://msdn2.microsoft.com/en-us/library/ms724411(VS.85).aspx">GetTickCount64()</a>
/// function.
/// \ingroup Timers

class CTickCount64Provider: public IProvideTickCount64
{
   public :

      CTickCount64Provider() = default;

      static ULONGLONG TickCount64()
      {
         return ::GetTickCount64();
      }

      ULONGLONG GetTickCount64() const override
      {
         return ::GetTickCount64();
      }
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core::Windows
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Windows
} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: TickCount64Provider.h
///////////////////////////////////////////////////////////////////////////////
