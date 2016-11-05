#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_SINGLETON_INCLUDED__
#define JETBYTE_TOOLS_SINGLETON_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: Singleton.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 1997 JetByte Limited.
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

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

namespace JetByteTools {

///////////////////////////////////////////////////////////////////////////////
// TSingleton
///////////////////////////////////////////////////////////////////////////////

/// A naive template Singleton class. Remember, Singletons are EVIL! and what's
/// more they're generally unnecessary.
/// See <a href="http://www.lenholgate.com/archives/000357.html">here</a> for 
/// more details.
/// \ingroup Templates
/// \ingroup Patterns

template <class T> class TSingleton
{
   public :

      // Construction and destruction

      virtual ~TSingleton();

      // Access to the one instance of the object

      static T &TheObject(); 

   protected :

      // We can only be created in our access function...

      TSingleton();
};

///////////////////////////////////////////////////////////////////////////////
// Construction and destriction
///////////////////////////////////////////////////////////////////////////////

template <class T> TSingleton<T>::TSingleton()
{
   // We can only be created in our access function...
}


template <class T> TSingleton<T>::~TSingleton()
{
   // Nothing to do, we just need to be virtual
}

///////////////////////////////////////////////////////////////////////////////
// Access to the one instance of the object
///////////////////////////////////////////////////////////////////////////////

template <class T> T &TSingleton<T>::TheObject()
{
   static T theObject;

   return theObject;
}

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_SINGLETON_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: Singleton.h 
///////////////////////////////////////////////////////////////////////////////
