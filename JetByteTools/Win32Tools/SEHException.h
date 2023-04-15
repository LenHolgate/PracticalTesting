#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: SEHException.h
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

#include <wtypes.h>

#include "JetByteTools/CoreTools/Exception.h"
#include "JetByteTools/CoreTools/tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSEHException
///////////////////////////////////////////////////////////////////////////////

/// \ingroup Win32
/// \ingroup Exceptions

class CSEHException : public Core::CException
{
   public :

      class Translator;

      friend class Translator;

      unsigned int GetCode() const;

      const EXCEPTION_POINTERS &GetExceptionPointers() const;

      const CONTEXT &GetContext() const;

      Core::_tstring GetWhere() const override;

      Core::_tstring GetWhat() const override;

      Core::_tstring GetDetails() const override;

      std::string GetDetailsA() const override;

   protected :

      const unsigned int m_code;

      EXCEPTION_POINTERS *m_pPointers;

   private :

      CSEHException(
         unsigned int code,
         EXCEPTION_POINTERS *pPointers);
};

/// A class that acts as a structured exception translator.
/// Create one of these objects on each thread where you want Win32 structured
/// exceptions translated into CSEHException.
/// \ingroup Win32
/// \ingroup Exceptions

class CSEHException::Translator
{
   public :

      #if (JETBYTE_BREAK_SEH_EXCEPTION_TRANSLATOR_COMPATABILITY == 0)
      // Prefer using CPerThreadErrorHandler in place of this translator
      Translator();
      #endif
      explicit Translator(int NotUsedJustBreakCompatibility);

      ~Translator();

   private :

      static _se_translator_function ConstructionHelper();

      static void trans_func(
         unsigned int code,
         EXCEPTION_POINTERS *pPointers);

      _se_translator_function m_prev;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: SEHException.h
///////////////////////////////////////////////////////////////////////////////
