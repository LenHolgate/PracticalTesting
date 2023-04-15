#pragma once
///////////////////////////////////////////////////////////////////////////////
// File: RunOneTestOfMany.h
///////////////////////////////////////////////////////////////////////////////
//
// The code in this file is released under the The MIT License (MIT)
//
// Copyright (c) 2021 JetByte Limited.
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

#include "JetByteTools/CoreTools/TickCountProvider.h"

#include <random>

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Email::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// CRunOneTestOfMany
///////////////////////////////////////////////////////////////////////////////

class CRunOneTestOfMany
{
   public :

      static size_t GetRandomNumberInRange(
         const size_t max)
      {
         std::mt19937 randomNumberGenerator;

         randomNumberGenerator.seed(JetByteTools::Core::CTickCountProvider::TickCount());

         std::uniform_int_distribution<size_t> distribution(0, max - 1);

         return distribution(randomNumberGenerator);
      }

      explicit CRunOneTestOfMany(
         const size_t totalNumberOfTests)
         :  m_currentTest(0),
            m_testToRun(GetRandomNumberInRange(totalNumberOfTests))
      {
      }

      bool RunThisTest()
      {
         return m_currentTest++ == m_testToRun;
      }

   private :

      size_t m_currentTest;

      const size_t m_testToRun;
};

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools

///////////////////////////////////////////////////////////////////////////////
// End of file: RunOneTestOfMany.h
///////////////////////////////////////////////////////////////////////////////
