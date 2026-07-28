#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: DateTime.h
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

#include "Types.h"
#include "tstring.h"

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Core {

///////////////////////////////////////////////////////////////////////////////
// Classes defined in other files...
///////////////////////////////////////////////////////////////////////////////

class IProvideCurrentDateTime;

///////////////////////////////////////////////////////////////////////////////
// CDateTime
///////////////////////////////////////////////////////////////////////////////

// this conversion is still a WIP. 
// right now we have CDateTime which is UTC. CLocalTime which isn't used anywhere
// should simply replace the SecondsSinceEpocToTm() and UpdateTmAndReturnSecondsSinceEpoc()
// functions to use mktime, etc. rather than timegm

// there's still an issue of being able to construct invalid times
// no framework code actually uses local time anywhere
// plus there's the slight confusion over the use of a separate DateTimeComponents struct

struct DateTimeComponents
{
   explicit DateTimeComponents(
      const IProvideCurrentDateTime &timeProvider);

   DateTimeComponents();

   DateTimeComponents(
      int year,
      int month,
      int day,
      int hour,
      int minute,
      int second);

   DateTimeComponents(
      int year,
      int month,
      int day,
      int hour,
      int minute,
      int second,
      int milliseconds);

   DateTimeComponents(
      const DateTimeComponents &rhs) = default;

   DateTimeComponents &operator=(
      const DateTimeComponents &rhs) = default;

   int year;
   int month;
   int day;

   int hour;
   int minute;
   int second;

   int milliseconds;

   int dayOfWeek;
   int dayOfYear;
};

//bool IsValid(
//   const DateTimeComponents &components);
//
//bool MakeValid(
//   DateTimeComponents &components);
//
//bool IsValidTime(
//   const DateTimeComponents &components);
//
//bool IsValidDate(
//   const DateTimeComponents &components);


class CDateTime : public DateTimeComponents
{
   public :

      static const CDateTime Epoc;

      static const CDateTime InvalidDateTime;

      typedef uint64 SecondsSinceEpoc;
      typedef uint64 MillisecondsSinceEpoc;

      static void GetUTCDateTime(
         DateTimeComponents &currentTime);

      enum class InitialSetting : BYTE
      {
         Epoc,
         Zero,
         Invalid,
         Now
      };

      explicit CDateTime(
         InitialSetting setting);

      explicit CDateTime(
         const IProvideCurrentDateTime &timeProvider);

      explicit CDateTime(
         const _tstring &yyyymmddhhmmssmmm);

      explicit CDateTime(
         MillisecondsSinceEpoc millisecondsSinceEpoc);

      explicit CDateTime(
         const DateTimeComponents &components);

      CDateTime() = delete;

      CDateTime(
         const CDateTime &rhs) = default;

      CDateTime &operator=(
         const CDateTime &rhs) = default;

      bool TryParseDate(
         const JetByteTools::Core::_tstring &ddmmyyyy);

      void ParseDate(
         const JetByteTools::Core::_tstring &ddmmyyyy);

      bool TryParseTime(
         const JetByteTools::Core::_tstring &hhmmssmmm);

      void ParseTime(
         const JetByteTools::Core::_tstring &hhmmssmmm);

      void CopyTo(
         DateTimeComponents &rhs) const;

      void CopyDateTo(
         DateTimeComponents &rhs) const;

      void CopyTimeTo(
         DateTimeComponents &rhs) const;

      bool IsValid() const;

      MillisecondsSinceEpoc GetAsMillisecondsSinceEpoc() const;

      SecondsSinceEpoc GetAsSecondsSinceEpoc() const;

      _tstring GetAsDatabaseDateTimeStamp() const;

      std::string GetAsDatabaseDateTimeStampA() const;

      _tstring GetAsHHMMSS() const;

      _tstring GetAsHHMMSSMMM() const;

      _tstring GetAsYYYYMMDD() const;

      void AddDays(int days);

      void AddMonths(int months);

      void AddYears(int years);
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Core
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Core
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: DateTime.h
///////////////////////////////////////////////////////////////////////////////
