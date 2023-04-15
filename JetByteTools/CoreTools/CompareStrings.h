#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CompareStrings.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2017 JetByte Limited.
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

#include "tstring.h"
#include "StringVector.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

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
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CompareStrings.h
///////////////////////////////////////////////////////////////////////////////
