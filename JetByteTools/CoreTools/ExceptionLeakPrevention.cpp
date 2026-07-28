///////////////////////////////////////////////////////////////////////////////
// File: ExceptionLeakPrevention.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2025 JetByte Limited.
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

#include "ExceptionLeakPrevention.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::string;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// File level static variables
///////////////////////////////////////////////////////////////////////////////

ExceptionProtectionHandlerFnc *s_pHandler = nullptr;

ULONG_PTR s_userData = 0;

///////////////////////////////////////////////////////////////////////////////
// Exception protection
///////////////////////////////////////////////////////////////////////////////

_tstring GetExceptionProtectionFailureTypeAsString(
   const ExceptionProtectionFailureType type)
{
   switch (type)
   {
      case ExceptionProtectionFailureType::General :
         return _T("Exception leaked");

      case ExceptionProtectionFailureType::Destructor:
         return _T("Exception leaked from destructor");

      case ExceptionProtectionFailureType::ThreadExit:
         return _T("Unexpected thread exit");

      case ExceptionProtectionFailureType::Timer:
         return _T("Exception in timer");

      default :
         return _T("Unknown exception leak");
   }
}

ExceptionProtectionHandlerFnc *SetExceptionProtectionHandler(
   ExceptionProtectionHandlerFnc *pHandler,
   ULONG_PTR userData)
{
   ExceptionProtectionHandlerFnc *pPrev = s_pHandler;

   s_pHandler = pHandler;

   s_userData = userData;

   return pPrev;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

  ///////////////////////////////////////////////////////////////////////////////
// End of file: ExceptionLeakPrevention.cpp
///////////////////////////////////////////////////////////////////////////////
