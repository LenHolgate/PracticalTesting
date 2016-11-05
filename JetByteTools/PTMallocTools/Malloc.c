///////////////////////////////////////////////////////////////////////////////
// File: Malloc.c
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 JetByte Limited.
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

/// \file JetByteTools\PTMallocTools\PrecompiledHeader.cpp
/// This file is compiled to produce the precompiled header in builds which use 
/// it.
/// \ingroup Precomp

#include "JetByteTools\Admin\Admin.h"

#pragma warning(disable: 4127)   // conditional expression is constant
#pragma warning(disable: 4255)   // 'x' : no function prototype given: converting '()' to '(void)'
#pragma warning(disable: 4267)   // 'x' : conversion from 'y' to 'z', possible loss of data

#include "Malloc.h"

#include "ptmalloc3\malloc.c"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// End of file: Malloc.c
///////////////////////////////////////////////////////////////////////////////

