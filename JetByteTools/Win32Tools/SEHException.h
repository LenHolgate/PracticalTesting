#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_WIN32_SEH_EXCEPTION__
#define JETBYTE_TOOLS_WIN32_SEH_EXCEPTION__
///////////////////////////////////////////////////////////////////////////////
// File: SEHException.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors
// shall not be liable for any damages suffered by licensee as a result of
// using the software. In no event will JetByte Limited be liable for any
// lost revenue, profit or data, or for direct, indirect, special,
// consequential, incidental or punitive damages, however caused and regardless
// of the theory of liability, arising out of the use of or inability to use
// software, even if JetByte Limited has been advised of the possibility of
// such damages.
//
///////////////////////////////////////////////////////////////////////////////

#include <wtypes.h>

#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CSEHException
///////////////////////////////////////////////////////////////////////////////

/// A exception class used by a structured exception translator to translate
/// Win32 structured exceptions into C++ exceptions.
/// Note that it's a deliberate choice not to derive from CException. Win32
/// structured exceptions include things like "Stack Overflow" so you want to
/// be sure you know what you're doing if you catch one... We could split the
/// SE exception into two, ones we might be able to recover from and those that
/// we can't recover from and derive the ones we might be able to recover from
/// from CException, but...
/// \ingroup Win32
/// \ingroup Exceptions

class CSEHException
{
   public :

      class Translator;

      friend class Translator;

      unsigned int GetCode() const;

      const EXCEPTION_POINTERS &GetExceptionPointers() const;

      const CONTEXT &GetContext() const;

      _tstring GetWhere() const;

      const _tstring &GetMessage() const;

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

      Translator();

      ~Translator();

   private :

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

#endif // JETBYTE_TOOLS_WIN32_SEH_EXCEPTION__

///////////////////////////////////////////////////////////////////////////////
// End of file: SEHException.h
///////////////////////////////////////////////////////////////////////////////
