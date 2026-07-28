///////////////////////////////////////////////////////////////////////////////
// File: LocalTime.cpp
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2024 JetByte Limited.
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

#include "JetByteTools/Admin/Admin.h"

#include "LocalTime.h"
#include "IProvideLocalTime.h"

#include <ctime>

#include "Exception.h"

#pragma hdrstop

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// CLocalTime
///////////////////////////////////////////////////////////////////////////////

CLocalTime::CLocalTime(
   const IProvideLocalTime &timeProvider)
   :  CDateTime(timeProvider)
{
}

CLocalTime::CLocalTime(
   const InitialSetting setting)
   : CDateTime(setting == InitialSetting::Now ? InitialSetting::Invalid : setting)
{
   if (setting == InitialSetting::Now)
   {
      GetLocalTime(*this);
   }
}

void CLocalTime::GetLocalTime(
   DateTimeComponents &time)
{
   #if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)

   _timespec64 timespec{};

   if (TIME_UTC != _timespec64_get(&timespec, TIME_UTC))
   {
      throw CException(_T("CLocaltime::GetLocalTime()"), _T("_timespec64_get failed"));
   }
   #else
   timespec timespec{};

   if (TIME_UTC != timespec_get(&timespec, TIME_UTC))
   {
      throw CException(_T("CLocaltime::GetLocalTime()"), _T("_timespec64_get failed"));
   }
   #endif

   tm tm{};

   #if defined(JETBYTE_TOOLS_ADMIN_WINDOWS_PLATFORM)
   _localtime64_s(&tm, &timespec.tv_sec);
   #else
   localtime_r(&timespec.tv_sec, &tm);
   #endif

   //time.nanoseconds = timespec.tv_nsec;
   time.milliseconds = timespec.tv_nsec / 1000000;

   time.second = tm.tm_sec;
   time.minute = tm.tm_min;
   time.hour = tm.tm_hour;

   time.day = tm.tm_mday;
   time.month = tm.tm_mon + 1;
   time.year = tm.tm_year + 1900;

   time.dayOfWeek = tm.tm_wday;
   time.dayOfYear = tm.tm_yday;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: LocalTime.cpp
///////////////////////////////////////////////////////////////////////////////
