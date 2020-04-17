#pragma once
#ifndef JETBYTE_TOOLS_ADMIN_COMPILER_NAME_INCLUDED__
#define JETBYTE_TOOLS_ADMIN_COMPILER_NAME_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: CompilerName.h
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2009 JetByte Limited.
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

#include <tchar.h>

#ifdef _MSC_VER

#if (_MSC_VER == 1300)
#define JETBYTE_COMPILER_NAME_A "VS2002 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2002 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio .Net (2002) [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio .Net (2002) [UNSUPPORTED]"
#elif (_MSC_VER == 1310)
#define JETBYTE_COMPILER_NAME_A "VS2003 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2003 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio .Net (2003) [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio .Net (2003) [UNSUPPORTED]"
#elif (_MSC_VER == 1400)
#define JETBYTE_COMPILER_NAME_A "VS2005 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2005 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2005 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2005 [UNSUPPORTED]"
#elif (_MSC_VER == 1500)
#define JETBYTE_COMPILER_NAME_A "VS2008 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2008 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2008 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2008 [UNSUPPORTED]"
#elif (_MSC_VER == 1600)
#define JETBYTE_COMPILER_NAME_A "VS2010 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2010 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2010 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2010 [UNSUPPORTED]"
#elif (_MSC_VER == 1700)
#define JETBYTE_COMPILER_NAME_A "VS2012 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2012 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2012 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2012 [UNSUPPORTED]"
#elif (_MSC_VER == 1800)
#define JETBYTE_COMPILER_NAME_A "VS2013 [UNSUPPORTED]"
#define JETBYTE_COMPILER_NAME_W L"VS2013 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2013 [UNSUPPORTED]"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2013 [UNSUPPORTED]"
#elif (_MSC_VER == 1900)
#define JETBYTE_COMPILER_NAME_A "VS2015"
#define JETBYTE_COMPILER_NAME_W L"VS2015"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2015"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2015"
#elif (_MSC_VER == 1910)
#define JETBYTE_COMPILER_NAME_A "VS2017"
#define JETBYTE_COMPILER_NAME_W L"VS2017"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017"
#elif (_MSC_VER == 1911)
#define JETBYTE_COMPILER_NAME_A "VS2017 (15.3)"
#define JETBYTE_COMPILER_NAME_W L"VS2017 (15.3)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017 (15.3)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017 (15.3)"
#elif (_MSC_VER == 1912)
#define JETBYTE_COMPILER_NAME_A "VS2017 (15.5)"
#define JETBYTE_COMPILER_NAME_W L"VS2017 (15.5)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017 (15.5)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017 (15.5)"
#elif (_MSC_VER == 1913)
#define JETBYTE_COMPILER_NAME_A "VS2017 (15.6)"
#define JETBYTE_COMPILER_NAME_W L"VS2017 (15.6)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017 (15.6)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017 (15.6)"
#elif (_MSC_VER == 1914)
#define JETBYTE_COMPILER_NAME_A "VS2017 (15.7)"
#define JETBYTE_COMPILER_NAME_W L"VS2017 (15.7)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017 (15.7)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017 (15.7)"
#elif (_MSC_VER == 1915)
#define JETBYTE_COMPILER_NAME_A "VS2017 (15.8)"
#define JETBYTE_COMPILER_NAME_W L"VS2017 (15.8)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017 (15.8)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017 (15.8)"
#elif (_MSC_VER == 1916)
#define JETBYTE_COMPILER_NAME_A "VS2017 (15.9)"
#define JETBYTE_COMPILER_NAME_W L"VS2017 (15.9)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2017 (15.9)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2017 (15.9)"
#elif (_MSC_VER == 1920)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16)"
#elif (_MSC_VER == 1921)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16.1)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16.1)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16.1)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16.1)"
#elif (_MSC_VER == 1922)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16.2)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16.2)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16.2)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16.2)"
#elif (_MSC_VER == 1923)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16.3)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16.3)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16.3)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16.3)"
#elif (_MSC_VER == 1924)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16.4)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16.4)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16.4)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16.4)"
#elif (_MSC_VER == 1925)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16.5)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16.5)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16.5)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16.5)"
#elif (_MSC_VER == 1926)
#define JETBYTE_COMPILER_NAME_A "VS2019 (16.6)"
#define JETBYTE_COMPILER_NAME_W L"VS2019 (16.6)"
#define JETBYTE_COMPILER_LONG_NAME_A "Visual Studio 2019 (16.6)"
#define JETBYTE_COMPILER_LONG_NAME_W L"Visual Studio 2019 (16.6)"
#elif (JETBYTE_ALLOW_UNTESTED_COMPILE_ENV == 0)
#error Unsupported compiler.
#else
#define JETBYTE_COMPILER_NAME_A "Unknown compiler"
#define JETBYTE_COMPILER_NAME_W L"Unknown compiler"
#define JETBYTE_COMPILER_LONG_NAME_A "Unknown compiler"
#define JETBYTE_COMPILER_LONG_NAME_W L"Unknown compiler"
#endif
#else
#error _MSC_VER is not defined, unsupported compiler.
#endif

#ifdef _UNICODE
#define JETBYTE_COMPILER_NAME JETBYTE_COMPILER_NAME_W
#define JETBYTE_COMPILER_LONG_NAME JETBYTE_COMPILER_LONG_NAME_W
#else
#define JETBYTE_COMPILER_NAME JETBYTE_COMPILER_NAME_A
#define JETBYTE_COMPILER_LONG_NAME JETBYTE_COMPILER_LONG_NAME_A
#endif

#endif // JETBYTE_TOOLS_ADMIN_COMPILER_NAME_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: CompilerName.h
///////////////////////////////////////////////////////////////////////////////
