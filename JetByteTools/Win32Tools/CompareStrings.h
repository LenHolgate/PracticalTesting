#pragma once
#ifndef COMPARE_STRINGS_INCLUDED__
#define COMPARE_STRINGS_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CompareStrings.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2017 JetByte Limited.
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

#include "tstring.h"
#include "StringVector.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {

///////////////////////////////////////////////////////////////////////////////
// CCompareStrings
///////////////////////////////////////////////////////////////////////////////

class CCompareStrings
{
   public :

      enum UseAlternativesMode
      {
         DoNotCheckAlternatives     = 0x000,
         CheckWildcards             = 0x001,
         CheckAlternatives          = 0x010,
         CheckRepeats               = 0x100,
         CheckAllAlternatives       = 0x111
      };

      static bool CompareStrings(
         const _tstring &expectedResult,
         const _tstring &actualResult,
         UseAlternativesMode useAlternatives,
         StringVector &errorMessages);

   private :

      static bool DoCompareStrings(
         const _tstring &expectedResult,
         const _tstring &actualResult,
         UseAlternativesMode useAlternatives);

      static bool DoCheckWildcards(
         const _tstring &expectedResult,
         const _tstring &actualResult,
         UseAlternativesMode useAlternatives);

};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Win32
} // End of namespace JetByteTools

#endif // COMPARE_STRINGS_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CompareStrings.h
///////////////////////////////////////////////////////////////////////////////
