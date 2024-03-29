#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: NullMessageLog.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2007 JetByte Limited.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the �Software�), to deal
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

#include "ILogMessages.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CNullMessageLog
///////////////////////////////////////////////////////////////////////////////

/// An object that implements ILogMessages and does nothing.
/// \ingroup NullObj
/// \ingroup Logging

class CNullMessageLog : public ILogMessages
{
   public :

      void SetThreadIdentifier(
         const std::string & /*identifier*/) override {}

      void SetThreadIdentifier(
         const std::wstring & /*identifier*/) override {}

      void SetLogName(
         const std::string & /*name*/) override {}

      void SetLogName(
         const std::wstring & /*name*/) override {}

      void LogMessage(
         const VectorOfLines & /*messages*/) override {}

      void LogMessage(
         const DequeOfLines & /*messages*/) override {}

      void LogMessage(
         const std::string & /*message*/) override {}

      void LogMessage(
         const std::wstring & /*message*/) override {}

      void LogMessage(
         const char * const /*pString*/) override {}

      void LogMessage(
         const wchar_t * const /*pString*/) override {}

      void LogMessage(
         const char * const /*pString*/,
         const DataLength /*stringLength*/) override {}

      void LogMessage(
         const wchar_t * const /*pString*/,
         const DataLength /*stringLength*/) override {}
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: NullMessageLog.h
///////////////////////////////////////////////////////////////////////////////
