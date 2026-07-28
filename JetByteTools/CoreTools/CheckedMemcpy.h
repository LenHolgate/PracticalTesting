#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: CheckedMemcpy.h
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
#include "ToString.h"
#include "CheckedStaticCast.h"

#if JETBYTE_CORE_DUMP_ON_CHECKED_MEMCPY_FAILURES == 1
#include "CrashDumpGenerator.h"
#endif

#include <cstring>   // for memcpy

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

static void ReportMemcpyError(
   const _tstring &where,
   const _tstring &message)
{
   #if JETBYTE_CORE_DUMP_ON_CHECKED_MEMCPY_FAILURES == 1

   static const _tstring type(_T("CheckedMemcpyFailure"));
   
   CCrashDumpGenerator::GenerateDumpHere(type);

   #endif

   throw CException(where, message);
}

static void MemCpyWrapper(
   void *pDestination,
   const void *pSource,
   const size_t byteLength)
{
   #if JETBYTE_CORE_VALIDATE_MEMCPY_RANGES_DO_NOT_OVERLAP == 1
   if ((pSource >= pDestination && pSource < static_cast<BYTE *>(pDestination) + byteLength) ||
       (pDestination >= pSource && pDestination < static_cast<const BYTE *>(pSource) + byteLength))
   {
      ReportMemcpyError(_T("CheckedMemcpy()"), _T("Cannot use memcpy for overlapping ranges"));
   }
   #endif

   memcpy(pDestination, pSource, byteLength);
}

template <typename T>
void CheckedMemcpy(
   T *pDestination,
   const size_t destinationSize,
   const T *pSource,
   const size_t dataLengthToCopy)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedMemcpy()"), _T("Destination is a null pointer"));
   }

   if (destinationSize < dataLengthToCopy)
   {
      ReportMemcpyError(_T("CheckedMemcpy()"), _T("Destination is a not large enough: ") + ToString(destinationSize) + _T(" < ") + ToString(dataLengthToCopy));
   }

   MemCpyWrapper(pDestination, pSource, dataLengthToCopy * sizeof(T));
}

template <typename T>
void CheckedMemcpyToOffset(
   T *pDestination,
   const size_t destinationSize,
   const size_t offsetInDestination,
   const T *pSource,
   const size_t dataLengthToCopy)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedMemcpyToOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSize < dataLengthToCopy)
   {
      ReportMemcpyError(_T("CheckedMemcpyToOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSize) + _T(" < ") + ToString(dataLengthToCopy));
   }

   if (destinationSize < offsetInDestination + dataLengthToCopy)
   {
      ReportMemcpyError(
         _T("CheckedMemcpyToOffset()"),
         _T("Destination (") + ToString(destinationSize) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopy) +
         _T(" to offset ") + ToString(offsetInDestination));
   }

   MemCpyWrapper(pDestination + offsetInDestination, pSource, dataLengthToCopy * sizeof(T));
}

static void UnCheckedByteCopy(
   void *pDestination,
   const void *pSource,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("UnCheckedByteCopy()"), _T("Destination is a null pointer"));
   }

   MemCpyWrapper(pDestination, pSource, dataLengthToCopyInBytes);
}

static void CheckedByteCopy(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const void *pSource,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteCopy()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteCopy()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   MemCpyWrapper(pDestination, pSource, dataLengthToCopyInBytes);
}

template <typename OffsetType, typename T>
OffsetType CheckedByteCopyValueToOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const OffsetType offsetInDestinationInBytes,
   const T &value)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteCopyValueToOffset()"), _T("Destination is a null pointer"));
   }

   const size_t dataLengthToCopyInBytes = sizeof(T);

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteCopyValueToOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (destinationSizeInBytes < offsetInDestinationInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteCopyValueToOffset()"),
         _T("Destination (") + ToString(destinationSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" to offset ") + ToString(offsetInDestinationInBytes));
   }

   MemCpyWrapper(static_cast<BYTE *>(pDestination) + offsetInDestinationInBytes, static_cast<const void *>(&value), dataLengthToCopyInBytes);

   return checked_static_cast<OffsetType>(offsetInDestinationInBytes + dataLengthToCopyInBytes);
}

template <typename OffsetType>
OffsetType CheckedByteCopyToOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const OffsetType offsetInDestinationInBytes,
   const void *pSource,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedMemcpyToOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedMemcpyToOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (destinationSizeInBytes < offsetInDestinationInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedMemcpyToOffset()"),
         _T("Destination (") + ToString(destinationSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" to offset ") + ToString(offsetInDestinationInBytes));
   }

   MemCpyWrapper(static_cast<BYTE *>(pDestination) + offsetInDestinationInBytes, pSource, dataLengthToCopyInBytes);

   return checked_static_cast<OffsetType>(offsetInDestinationInBytes + dataLengthToCopyInBytes);
}

template <typename OffsetType>
OffsetType CheckedByteCopyFromOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const void *pSource,
   const size_t sourceSizeInBytes,
   const OffsetType offsetInSourceInBytes,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteCopyFromOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteCopyFromOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (sourceSizeInBytes < offsetInSourceInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteCopyFromOffset()"),
         _T("Source (") + ToString(sourceSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" from offset ") + ToString(offsetInSourceInBytes));
   }

   MemCpyWrapper(pDestination, static_cast<const BYTE *>(pSource) + offsetInSourceInBytes, dataLengthToCopyInBytes);

   return checked_static_cast<OffsetType>(offsetInSourceInBytes + dataLengthToCopyInBytes);
}

static void CheckedByteCopyToOffsetFromOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const size_t offsetInDestinationInBytes,
   const void *pSource,
   const size_t sourceSizeInBytes,
   const size_t offsetInSourceInBytes,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteCopyToOffsetFromOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteCopyToOffsetFromOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (destinationSizeInBytes < offsetInDestinationInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteCopyToOffsetFromOffset()"),
         _T("Destination (") + ToString(destinationSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" to offset ") + ToString(offsetInDestinationInBytes));
   }

   if (sourceSizeInBytes < offsetInSourceInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteCopyToOffsetFromOffset()"),
         _T("Source (") + ToString(sourceSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" from offset ") + ToString(offsetInSourceInBytes));
   }

   MemCpyWrapper(static_cast<BYTE *>(pDestination) + offsetInDestinationInBytes, static_cast<const BYTE *>(pSource) + offsetInSourceInBytes, dataLengthToCopyInBytes);
}

template <typename OffsetType>
OffsetType CheckedByteMoveToOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const OffsetType offsetInDestinationInBytes,
   const void *pSource,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteMoveToOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteMoveToOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (destinationSizeInBytes < offsetInDestinationInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteMoveToOffset()"),
         _T("Destination (") + ToString(destinationSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" to offset ") + ToString(offsetInDestinationInBytes));
   }

   memmove(static_cast<BYTE *>(pDestination) + offsetInDestinationInBytes, pSource, dataLengthToCopyInBytes);

   return checked_static_cast<OffsetType>(offsetInDestinationInBytes + dataLengthToCopyInBytes);
}

template <typename OffsetType>
OffsetType CheckedByteMoveFromOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const void *pSource,
   const size_t sourceSizeInBytes,
   const OffsetType offsetInSourceInBytes,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteMoveFromOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteMoveFromOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (sourceSizeInBytes < offsetInSourceInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteMoveFromOffset()"),
         _T("Source (") + ToString(sourceSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" from offset ") + ToString(offsetInSourceInBytes));
   }

   memmove(pDestination, static_cast<const BYTE *>(pSource) + offsetInSourceInBytes, dataLengthToCopyInBytes);

   return checked_static_cast<OffsetType>(offsetInSourceInBytes + dataLengthToCopyInBytes);
}

static void CheckedByteMoveToOffsetFromOffset(
   void *pDestination,
   const size_t destinationSizeInBytes,
   const size_t offsetInDestinationInBytes,
   const void *pSource,
   const size_t sourceSizeInBytes,
   const size_t offsetInSourceInBytes,
   const size_t dataLengthToCopyInBytes)
{
   if (!pDestination)
   {
      ReportMemcpyError(_T("CheckedByteMoveToOffsetFromOffset()"), _T("Destination is a null pointer"));
   }

   if (destinationSizeInBytes < dataLengthToCopyInBytes)
   {
      ReportMemcpyError(_T("CheckedByteMoveToOffsetFromOffset()"), _T("Destination is a not large enough: ") + ToString(destinationSizeInBytes) + _T(" < ") + ToString(dataLengthToCopyInBytes));
   }

   if (destinationSizeInBytes < offsetInDestinationInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteMoveToOffsetFromOffset()"),
         _T("Destination (") + ToString(destinationSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" to offset ") + ToString(offsetInDestinationInBytes));
   }

   if (sourceSizeInBytes < offsetInSourceInBytes + dataLengthToCopyInBytes)
   {
      ReportMemcpyError(
         _T("CheckedByteMoveToOffsetFromOffset()"),
         _T("Source (") + ToString(sourceSizeInBytes) +
         _T( ") is a not large enough to copy ") + ToString(dataLengthToCopyInBytes) +
         _T(" from offset ") + ToString(offsetInSourceInBytes));
   }

   memmove(static_cast<BYTE *>(pDestination) + offsetInDestinationInBytes, static_cast<const BYTE *>(pSource) + offsetInSourceInBytes, dataLengthToCopyInBytes);
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: CheckedMemcpy.h
///////////////////////////////////////////////////////////////////////////////

