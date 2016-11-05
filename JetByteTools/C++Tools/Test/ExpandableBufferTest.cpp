///////////////////////////////////////////////////////////////////////////////
// File: ExpandableBufferTest.cpp
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2004 JetByte Limited.
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

#include "JetByteTools\Admin\Admin.h"

#include "ExpandableBufferTest.h"

#include "..\ExpandableBuffer.h"

#include "JetByteTools\Win32Tools\Utils.h"
#include "JetByteTools\Win32Tools\DebugTrace.h"

#include "JetByteTools\TestTools\TestException.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Using directives
///////////////////////////////////////////////////////////////////////////////

using JetByteTools::Test::CTestException;

using JetByteTools::Win32::Output;
using JetByteTools::Win32::_tstring;

using std::string;

//////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {
namespace Test {

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void PopulateBuffer(
	BYTE *pBuffer,
	const size_t size);

static void CheckBuffer(
	const _tstring &functionName,
	const BYTE * const pBuffer,
	const size_t size);

///////////////////////////////////////////////////////////////////////////////
// CExpandableBufferTest
///////////////////////////////////////////////////////////////////////////////

void CExpandableBufferTest::TestAll()
{
   TestConstruct();
   TestAssign();
   TestCopyConstruct();
   TestExpand();
   TestResize();
   TestRelease();
}

void CExpandableBufferTest::TestConstruct()
{
   const _tstring functionName = _T("CExpandableBufferTest::TestConstruct");
   
   Output(functionName + _T(" - start"));

   TExpandableBuffer<BYTE> byteBuffer;

   THROW_ON_FAILURE(functionName, 0 == byteBuffer.GetSize());

   TExpandableBuffer<int> intBuffer(10);

   THROW_ON_FAILURE(functionName, 10 == intBuffer.GetSize());

   TExpandableBuffer<string> stringBuffer(1000);

   THROW_ON_FAILURE(functionName, 1000 == stringBuffer.GetSize());

   Output(functionName + _T(" - stop"));
}

void CExpandableBufferTest::TestAssign()
{
   const _tstring functionName = _T("CExpandableBufferTest::TestAssign");
   
   Output(functionName + _T(" - start"));

   const size_t bufferSize = 500;

   TExpandableBuffer<BYTE> byteBuffer(bufferSize);

   BYTE *pBuffer1 = byteBuffer.GetBuffer();

   PopulateBuffer(pBuffer1, bufferSize);

   TExpandableBuffer<BYTE> byteBuffer2(10);

   BYTE *pBuffer2 = byteBuffer2.GetBuffer();
   
   memset(pBuffer2, 77, byteBuffer2.GetSize());

   byteBuffer2 = byteBuffer;

   THROW_ON_FAILURE(functionName, pBuffer2 != byteBuffer2.GetBuffer());
 
   THROW_ON_FAILURE(functionName, byteBuffer.GetBuffer() != byteBuffer2.GetBuffer());
 
   THROW_ON_FAILURE(functionName, pBuffer1 == byteBuffer.GetBuffer());

   THROW_ON_FAILURE(functionName, byteBuffer.GetSize() == byteBuffer2.GetSize());

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer2.GetSize());

   BYTE *pBuffer3 = byteBuffer2.GetBuffer();

   CheckBuffer(functionName, pBuffer1, bufferSize);
   CheckBuffer(functionName, pBuffer3, bufferSize);

   Output(functionName + _T(" - stop"));
}

void CExpandableBufferTest::TestCopyConstruct()
{
   const _tstring functionName = _T("CExpandableBufferTest::TestCopyConstruct");
   
   Output(functionName + _T(" - start"));

   const size_t bufferSize = 500;

   TExpandableBuffer<BYTE> byteBuffer(bufferSize);

   BYTE *pBuffer1 = byteBuffer.GetBuffer();

   PopulateBuffer(pBuffer1, bufferSize);

   TExpandableBuffer<BYTE> byteBuffer2(byteBuffer);

   THROW_ON_FAILURE(functionName, byteBuffer.GetBuffer() != byteBuffer2.GetBuffer());
 
   THROW_ON_FAILURE(functionName, pBuffer1 == byteBuffer.GetBuffer());

   THROW_ON_FAILURE(functionName, byteBuffer.GetSize() == byteBuffer2.GetSize());

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer2.GetSize());

   BYTE *pBuffer2 = byteBuffer2.GetBuffer();

   CheckBuffer(functionName, pBuffer1, bufferSize);
   CheckBuffer(functionName, pBuffer2, bufferSize);

   Output(functionName + _T(" - stop"));
}

void CExpandableBufferTest::TestExpand()
{
   const _tstring functionName = _T("CExpandableBufferTest::TestExpand");
   
   Output(functionName + _T(" - start"));

   const size_t bufferSize = 500;

   TExpandableBuffer<BYTE> byteBuffer(bufferSize);

   BYTE *pBuffer1 = byteBuffer.GetBuffer();

   PopulateBuffer(pBuffer1, bufferSize);

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer.Expand(0));

   THROW_ON_FAILURE(functionName, pBuffer1 == byteBuffer.GetBuffer());

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer.Expand(bufferSize));

   THROW_ON_FAILURE(functionName, pBuffer1 == byteBuffer.GetBuffer());

   const size_t newBufferSize = bufferSize * 3;

   THROW_ON_FAILURE(functionName, newBufferSize == byteBuffer.Expand(newBufferSize));

   BYTE *pBuffer2 = byteBuffer.GetBuffer();

   THROW_ON_FAILURE(functionName, pBuffer1 != pBuffer2);

   CheckBuffer(functionName, pBuffer2, bufferSize);

   Output(functionName + _T(" - stop"));
}

void CExpandableBufferTest::TestResize()
{
   const _tstring functionName = _T("CExpandableBufferTest::TestResize");
   
   Output(functionName + _T(" - start"));

   const size_t bufferSize = 500;

   TExpandableBuffer<BYTE> byteBuffer(bufferSize);

   BYTE *pBuffer1 = byteBuffer.GetBuffer();

   PopulateBuffer(pBuffer1, bufferSize);

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer.Resize(0));

   THROW_ON_FAILURE(functionName, pBuffer1 == byteBuffer.GetBuffer());

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer.Resize(bufferSize));

   THROW_ON_FAILURE(functionName, pBuffer1 == byteBuffer.GetBuffer());

   const size_t newBufferSize = bufferSize * 3;

   THROW_ON_FAILURE(functionName, newBufferSize == byteBuffer.Resize(newBufferSize));

   BYTE *pBuffer2 = byteBuffer.GetBuffer();

   THROW_ON_FAILURE(functionName, pBuffer1 != pBuffer2);

   Output(functionName + _T(" - stop"));
}

void CExpandableBufferTest::TestRelease()
{
   const _tstring functionName = _T("CExpandableBufferTest::TestRelease");
   
   Output(functionName + _T(" - start"));

   const size_t bufferSize = 500;

   TExpandableBuffer<BYTE> byteBuffer(bufferSize);

   BYTE *pBuffer1 = byteBuffer.GetBuffer();

   PopulateBuffer(pBuffer1, bufferSize);

   BYTE *pBuffer2 = byteBuffer.ReleaseBuffer();

   THROW_ON_FAILURE(functionName, 0 == byteBuffer.GetBuffer());

   THROW_ON_FAILURE(functionName, 0 == byteBuffer.GetSize());

   THROW_ON_FAILURE(functionName, pBuffer1 == pBuffer2);

   CheckBuffer(functionName, pBuffer2, bufferSize);

   delete [] pBuffer2;

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer.Resize(bufferSize));

   delete [] byteBuffer.ReleaseBuffer();

   THROW_ON_FAILURE(functionName, bufferSize == byteBuffer.Expand(bufferSize));

   delete [] byteBuffer.ReleaseBuffer();

   Output(functionName + _T(" - stop"));
}

///////////////////////////////////////////////////////////////////////////////
// Static helper functions
///////////////////////////////////////////////////////////////////////////////

static void PopulateBuffer(
	BYTE *pBuffer,
	const size_t size)
{
   for (size_t i = 0; i < size; ++i)
   {
      pBuffer[i] = static_cast<BYTE>(i % 256);
   }
}

static void CheckBuffer(
	const _tstring &functionName,
	const BYTE * const pBuffer,
	const size_t size)
{
   for (size_t i = 0; i < size; ++i)
   {
      THROW_ON_FAILURE(functionName, pBuffer[i] == i % 256);
   }
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools::Test
///////////////////////////////////////////////////////////////////////////////

} // End of namespace Test
} // End of namespace JetByteTools 

///////////////////////////////////////////////////////////////////////////////
// End of file: ExpandableBufferTest.cpp
///////////////////////////////////////////////////////////////////////////////

