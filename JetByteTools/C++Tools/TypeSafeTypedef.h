#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef JETBYTE_TOOLS_TYPE_SAFE_TYPEDEF_INCLUDED__
#define JETBYTE_TOOLS_TYPE_SAFE_TYPEDEF_INCLUDED__
///////////////////////////////////////////////////////////////////////////////
// File: TypeSafeTypedef.h 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2007 JetByte Limited.
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
// CTypeSafeTypedef
///////////////////////////////////////////////////////////////////////////////

template <typename baseType, typename uniqueBase>
class TTypeSafeTypedef : public uniqueBase
{
   public :

      TTypeSafeTypedef()
      {
      }

      explicit TTypeSafeTypedef(
         const baseType &rhs)
         : m_value(rhs)
      {
      }

      TTypeSafeTypedef(
         const TTypeSafeTypedef &rhs)
         :  m_value(rhs.m_value)
      {
      }

      TTypeSafeTypedef &operator=(
         const baseType &rhs)
      {
         m_value = rhs;

         return *this;
      }

      TTypeSafeTypedef &operator=(
         const TTypeSafeTypedef<baseType, uniqueBase> &rhs)
      {
         m_value = rhs.m_value;

         return *this;
      }

      operator baseType() const
      {
         return m_value;
      }

      baseType GetValue() const
      {
         return m_value;
      }

   private :

      baseType m_value;

};

///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////

#define TYPESAFE_TYPEDEF(type, name)   \
   class name##TTypeSafeTypedef__ {};  \
   typedef JetByteTools::TTypeSafeTypedef<type, name##TTypeSafeTypedef__> name; 

///////////////////////////////////////////////////////////////////////////////
// Namespace: JetByteTools
///////////////////////////////////////////////////////////////////////////////

} // End of namespace JetByteTools 

#endif // JETBYTE_TOOLS_TYPE_SAFE_TYPEDEF_INCLUDED__

///////////////////////////////////////////////////////////////////////////////
// End of file: TypeSafeTypedef.h 
///////////////////////////////////////////////////////////////////////////////
