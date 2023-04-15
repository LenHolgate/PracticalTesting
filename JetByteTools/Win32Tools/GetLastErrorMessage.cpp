///////////////////////////////////////////////////////////////////////////////
// File: GetLastErrorMessage.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2018 JetByte Limited.
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

#include "GetLastErrorMessage.h"
#include "Win32Exception.h"

#include "JetByteTools/CoreTools/ToString.h"

#pragma hdrstop

#include <tchar.h>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Core::_tstring;
using JetByteTools::Core::ToString;
using JetByteTools::Core::ToStringA;
using JetByteTools::Core::ToStringW;

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// GetLastErrorMessage
///////////////////////////////////////////////////////////////////////////////

_tstring GetLastErrorMessageIfPossible(
   const DWORD last_error,
   const bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      return _T("");
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

_tstring GetLastErrorMessageIfPossible(
   const HMODULE hModule,
   const DWORD last_error,
   const bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
      hModule,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      return _T("");
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

_tstring GetLastErrorMessage(
   const DWORD last_error,
   const bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = GetLastError();

      if (thisError == 8)
      {
         return _T("Failed to obtain error string - Not enough storage is available to process this command.");
      }

      if (thisError != last_error)
      {
         return GetLastErrorMessage(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return _T("Failed to obtain error string: ") + ToString(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

_tstring GetLastErrorMessage(
   const HMODULE hModule,
   const DWORD last_error,
   const bool stripTrailingLineFeed)
{
   TCHAR errmsg[512];

   if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
      hModule,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = GetLastError();

      if (thisError == 8)
      {
         return _T("Failed to obtain error string - Not enough storage is available to process this command.");
      }

      if (thisError != last_error)
      {
         return GetLastErrorMessage(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return _T("Failed to obtain error string: ") + ToString(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = _tcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

string GetLastErrorMessageA(
   const DWORD last_error,
   const bool stripTrailingLineFeed)
{
   CHAR errmsg[512];

   if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = GetLastError();

      if (thisError == 8)
      {
         return "Failed to obtain error string - Not enough storage is available to process this command.";
      }

      if (thisError != last_error)
      {
         return GetLastErrorMessageA(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return "Failed to obtain error string: " + ToStringA(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = strlen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}

wstring GetLastErrorMessageW(
   const DWORD last_error,
   const bool stripTrailingLineFeed)
{
   wchar_t errmsg[512];

   if (!FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      last_error,
      0,
      errmsg,
      511,
      nullptr))
   {
      // if we fail, call ourself to find out why and return that error

      const DWORD thisError = GetLastError();

      if (thisError != last_error)
      {
         return GetLastErrorMessageW(thisError, stripTrailingLineFeed);
      }

      // But don't get into an infinite loop...

      return L"Failed to obtain error string: " + ToStringW(last_error);
   }

   if (stripTrailingLineFeed)
   {
      const size_t length = wcslen(errmsg);

      if (errmsg[length-1] == '\n')
      {
         errmsg[length-1] = 0;

         if (errmsg[length-2] == '\r')
         {
            errmsg[length-2] = 0;
         }
      }
   }

   return errmsg;
}
///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: GetLastErrorMessage.cpp
///////////////////////////////////////////////////////////////////////////////
