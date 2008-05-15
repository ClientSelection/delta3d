#include <dtUtil/refstring.h>
#ifdef __GNUG__
#  include <ext/hash_set>
#elif defined(_MSC_VER)
#  include <hash_set>
#else
#  include <set>
#endif

#define USE_TABLE 1

namespace dtUtil
{
   static size_t StringCount = 0;
#if USE_TABLE
#ifdef __GNUG__
   struct hashString
   {
     size_t operator()(const std::string& string) const
       { return __gnu_cxx::__stl_hash_string(string.c_str()); }
   };

   static __gnu_cxx::hash_set<std::string, hashString> StringSet(3000);
#elif defined(_MSC_VER)
   static stdext::hash_set<std::string> StringSet;   
#else
   static std::set<std::string> StringSet;
#endif
#endif
   size_t RefString::GetSharedStringCount() 
   {
      return StringCount;
   }

   /////////////////////////////////////////////////////////////
   RefString::RefString(const std::string& value): mString(NULL)
   {
      Intern(value);
   }

   /////////////////////////////////////////////////////////////
   RefString::RefString(const char* value): mString(NULL)
   {
      Intern(value);
   }

   /////////////////////////////////////////////////////////////
   RefString::RefString(const RefString& toCopy): mString(NULL)
   {
#if USE_TABLE
      //If we are using the table, we'll get the same pointer anyway.
      mString = toCopy.mString;
#else
      Intern(*toCopy.mString);
#endif
   }

   /////////////////////////////////////////////////////////////
   RefString::~RefString()
   {
#if !USE_TABLE
      delete mString;
      --StringCount;
#endif
   }

   /////////////////////////////////////////////////////////////
   RefString RefString::operator+(const std::string& string) const 
   { 
      return RefString(*mString + string);
   }

   /////////////////////////////////////////////////////////////
   RefString RefString::operator+(const RefString& refString) const
   { 
      return RefString(*mString + *refString.mString);
   }

   /////////////////////////////////////////////////////////////
   dtUtil::RefString& RefString::operator=(const std::string& value)
   {
      Intern(value);
      return *this;
   }

   /////////////////////////////////////////////////////////////
   dtUtil::RefString& RefString::operator=(const dtUtil::RefString& value)
   {
      if (this == &value)
         return *this;
#if USE_TABLE
      //If we are using the table, we'll get the same pointer anyway.
      mString = value.mString;
#else
      Intern(*value.mString);
#endif
      return *this;
   }

   /////////////////////////////////////////////////////////////
   void RefString::Intern(const std::string& value)
   {
#if USE_TABLE
      //One can only insert a string once, but it will still return the iterator.
      mString = &(*StringSet.insert(value).first);
      StringCount = StringSet.size();
#else
      if (mString != NULL)
      {
         delete mString;
         --StringCount;
      }
      mString = new std::string(value);
      ++StringCount;
#endif
   }

}
