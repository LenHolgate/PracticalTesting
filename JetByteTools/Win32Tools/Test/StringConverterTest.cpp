///////////////////////////////////////////////////////////////////////////////
// File: StringConverterTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
//
// JetByte Limited grants you ("Licensee") a non-exclusive, royalty free, 
// licence to use, modify and redistribute this software in source and binary 
// code form, provided that i) this copyright notice and licence appear on all 
// copies of the software; and ii) Licensee does not utilize the software in a 
// manner which is disparaging to JetByte Limited.
//
// This software is provided "as is" without a warranty of any kind. All 
// express or implied conditions, representations and warranties, including
// any implied warranty of merchantability, fitness for a particular purpose
// or non-infringement, are hereby excluded. JetByte Limited and its licensors 
// shall not be liable for any damages suffered by licensee as a result of 
// using, modifying or distributing the software or its derivatives. In no
// event will JetByte Limited be liable for any lost revenue, profit or data,
// or for direct, indirect, special, consequential, incidental or punitive
// damages, however caused and regardless of the theory of liability, arising 
// out of the use of or inability to use software, even if JetByte Limited 
// has been advised of the possibility of such damages.
//
// This software is not designed or intended for use in on-line control of 
// aircraft, air traffic, aircraft navigation or aircraft communications; or in 
// the design, construction, operation or maintenance of any nuclear 
// facility. Licensee represents and warrants that it will not use or 
// redistribute the Software for such purposes. 
//
///////////////////////////////////////////////////////////////////////////////

#include "JetByteTools\Admin\Admin.h"

#include "StringConverterTest.h"

#include "..\StringConverter.h"

#include "JetByteTools\Win32Tools\Utils.h"

#include "JetByteTools\TestTools\TestException.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;

using std::string;
using std::wstring;

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Win32 {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CStringConverterTest
///////////////////////////////////////////////////////////////////////////////

void CStringConverterTest::TestAll()
{
   TestAtoW();
}

void CStringConverterTest::TestAtoW()
{
   const _tstring functionName = _T("CStringConverterTest::TestAtoW");
   
   Output(functionName + _T(" - start"));

   {
      const string input = "This is a string";
   
      const wstring output = CStringConverter::AtoW(input);

      const wstring expected = L"This is a string";

      THROW_ON_FAILURE(functionName, output == expected);
   }

   {
      const size_t stringSize = 2000000;

      const string input(stringSize, 'A');

      const wstring output = CStringConverter::AtoW(input);

      const wstring expected(stringSize, 'A');

      THROW_ON_FAILURE(functionName, output == expected);
   }

   {
      const string input;

      const wstring output = CStringConverter::AtoW(input);

      const wstring expected;

      THROW_ON_FAILURE(functionName, output == expected);
   }



   Output(functionName + _T(" - stop"));
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Win32::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace Win32
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: StringConverterTest.cpp
///////////////////////////////////////////////////////////////////////////////

