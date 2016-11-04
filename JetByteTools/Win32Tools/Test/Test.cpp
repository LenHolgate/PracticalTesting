///////////////////////////////////////////////////////////////////////////////
// File: Test.cpp
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

#include <iostream>

#include "JetByteTools\TestTools\TestException.h"

#include "CallbackTimerTest.h"

#include "JetByteTools\Win32Tools\Exception.h"
#include "JetByteTools\Win32Tools\SEHException.h"
#include "JetByteTools\Win32Tools\StringConverter.h"

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -save
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using std::cout;
using std::endl;
using std::string;

using JetByteTools::Win32::CException;
using JetByteTools::Win32::CStringConverter;
using JetByteTools::Win32::CSEHException;

using JetByteTools::Test::CTestException;

using namespace JetByteTools::Win32::Test;

///////////////////////////////////////////////////////////////////////////////
// Program Entry Point
///////////////////////////////////////////////////////////////////////////////

int main(int /*argc*/, char * /*argv[ ]*/)
{
   CSEHException::Translator sehTranslator;

   bool ok = false;

   try
   {
      CCallbackTimerTest::TestAll();

      ok = true;
   }
   catch(const CTestException &e)
   {
      cout << "Test Exception: " << CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()) << endl;

      ok = false;
   }
   catch(const CException &e)
   {
      cout << "Exception: " << CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()) << endl;

      ok = false;
   }
   catch(const CSEHException &e)
   {
      cout << "Exception: " << CStringConverter::TtoA(e.GetWhere() + _T(" - ") + e.GetMessage()) << endl;

      ok = false;
   }
   catch(const char *p)
   {
      cout << "Exception: " << p << endl;
   }
   catch(...)
   {
      cout << "Unexpected exception" << endl;

      ok = false;
   }

   cout << "Test " << (ok ? "Passed" : "Failed") << endl;

   return ok ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////
// Lint options
//
//lint -restore
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// End of file: Test.cpp
///////////////////////////////////////////////////////////////////////////////

