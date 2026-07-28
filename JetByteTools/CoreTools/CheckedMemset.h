#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CheckedMemset.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2023 JetByte Limited.
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

#include "Types.h"
#include "Exception.h"

#if JETBYTE_CORE_DUMP_ON_CHECKED_MEMSET_FAILURES == 1
#include "CrashDumpGenerator.h"
#endif

#include <cstring>   // for memset

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

static void ReportMemSetError(
   const _tstring &where,
   const _tstring &message)
{
   #if JETBYTE_CORE_DUMP_ON_CHECKED_MEMSET_FAILURES == 1

   static const _tstring type(_T("CheckedMemsetFailure"));

   CCrashDumpGenerator::GenerateDumpHere(type);

   #endif

   throw CException(where, message);
}

template <typename T>
void CheckedMemset(
   T *pDestination,
   const size_t destinationSize,
   const T value)
{
   if (!pDestination)
   {
      ReportMemSetError(_T("CheckedMemset()"), _T("Destination is a null pointer"));
   }

   if (!destinationSize)
   {
      ReportMemSetError(_T("CheckedMemset()"), _T("destinationSize is zero"));
   }

   for (size_t i = 0; i < destinationSize; ++i)
   {
      *pDestination = value;
   }
}

inline void CheckedMemset(
   void *pDestination,
   const size_t destinationSize,
   const int value)
{
   if (!pDestination)
   {
      ReportMemSetError(_T("CheckedMemset()"), _T("Destination is a null pointer"));
   }

   if (!destinationSize)
   {
      ReportMemSetError(_T("CheckedMemset()"), _T("destinationSize is zero"));
   }

   memset(pDestination, value, destinationSize);
}

template <typename T>
void CheckedZeroObjectContents(
   T *pObject)
{
   if (!pObject)
   {
      ReportMemSetError(_T("CheckedZeroObjectContents()"), _T("pObject is a null pointer"));
   }

   memset(pObject, 0, sizeof(T));
}

static void CheckedZeroMemory(
   void *pDestination,
   const size_t destinationSizeInBytes)
{
   if (!pDestination)
   {
      ReportMemSetError(_T("CheckedZeroMemory()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes)
   {
      memset(pDestination, 0, destinationSizeInBytes);
   }
}

static void CheckedZeroMemoryAtOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const size_t startOffset,
   const size_t bytesToSet)
{
   if (!pDestination)
   {
      ReportMemSetError(_T("CheckedZeroMemoryAtOffset()"), _T("Destination is a null pointer"));
   }

   if (!destinationSizeInBytes)
   {
      ReportMemSetError(_T("CheckedZeroMemoryAtOffset()"), _T("destinationSizeInBytes is zero"));
   }

   if (startOffset + bytesToSet > destinationSizeInBytes)
   {
      ReportMemSetError(_T("CheckedZeroMemoryAtOffset()"), _T("Start offset + bytes to set is larger than destination size"));
   }

   if (bytesToSet)
   {
      memset(static_cast<BYTE *>(pDestination) + startOffset, 0, bytesToSet);
   }
}

static void CheckedByteSet(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const BYTE value)
{
   if (!pDestination)
   {
      ReportMemSetError(_T("CheckedByteSet()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes)
   {
      memset(pDestination, value, destinationSizeInBytes);
   }
}

static void CheckedByteSetAtOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const BYTE value,
   const size_t startOffset,
   const size_t bytesToSet)
{
   if (!pDestination)
   {
      ReportMemSetError(_T("CheckedByteSet()"), _T("Destination is a null pointer"));
   }

   if (!destinationSizeInBytes)
   {
      ReportMemSetError(_T("CheckedByteSet()"), _T("destinationSizeInBytes is zero"));
   }

   if (startOffset + bytesToSet > destinationSizeInBytes)
   {
      ReportMemSetError(_T("CheckedByteSet()"), _T("Start offset + bytes to set is larger than destination size"));
   }
   
   memset(static_cast<BYTE *>(pDestination) + startOffset, value, bytesToSet);
}

static void CheckedByteSetAtOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const BYTE value,
   const size_t startOffset)
{
   CheckedByteSetAtOffset(pDestination, destinationSizeInBytes, value, startOffset, destinationSizeInBytes - startOffset);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CheckedByteSet.h
///////////////////////////////////////////////////////////////////////////////

