#pragma once
#include <string>
#include <fc/reflect/reflect.hpp>
#include <iosfwd>

namespace eosio { namespace chain {
   using std::string;

   static constexpr uint64_t char_to_symbol( char c ) {
      if( c == '/' )
         return 63;
      if( c >= 'A' && c <= 'Z' )
         return (c - 'A') + 37;
      if( c >= 'a' && c <= 'z' )
         return (c - 'a') + 11;
      if( c >= '0' && c <= '9' )
         return (c - '0') + 1;
      return 0;
   }

   // Each char of the string is encoded into 6-bit chunk and left-shifted
   // to its 6-bit slot starting with the highest slot for the first char.
   // The 11st char, if str is long enough, is encoded into 4-bit chunk
   // and placed in the lowest 4 bits. 64 = 10 * 5 + 4
   static constexpr uint64_t string_to_name( const char* str )
   {
      uint64_t name = 0;
      int i = 0;
      for ( ; str[i] && i < 10; ++i) {
          // NOTE: char_to_symbol() returns char type, and without this explicit
          // expansion to uint64 type, the compilation fails at the point of usage
          // of string_to_name(), where the usage requires constant (compile time) expression.
           name |= (char_to_symbol(str[i]) & 0x3f) << (64 - 6 * (i + 1));
       }

      // The for-loop encoded up to 60 high bits into uint64 'name' variable,
      // if (strlen(str) > 10) then encode str[10] into the low (remaining)
      // 4 bits of 'name'
      if (i == 10)
          name |= char_to_symbol(str[10]) & 0x0F;
      return name;
   }

#define N(X) eosio::chain::string_to_name(#X)

   struct name {
      uint64_t value = 0;
      bool empty()const { return 0 == value; }
      bool good()const  { return !empty();   }

      name( const char* str )   { set(str);           } 
      name( const string& str ) { set( str.c_str() ); }

      void set( const char* str );

      template<typename T>
      name( T v ):value(v){}
      name(){}

      explicit operator string()const;

      string to_string() const { return string(*this); }

      name& operator=( uint64_t v ) {
         value = v;
         return *this;
      }

      name& operator=( const string& n ) {
         value = name(n).value;
         return *this;
      }
      name& operator=( const char* n ) {
         value = name(n).value;
         return *this;
      }

      friend std::ostream& operator << ( std::ostream& out, const name& n ) {
         return out << string(n);
      }

      friend bool operator < ( const name& a, const name& b ) { return a.value < b.value; }
      friend bool operator <= ( const name& a, const name& b ) { return a.value <= b.value; }
      friend bool operator > ( const name& a, const name& b ) { return a.value > b.value; }
      friend bool operator >=( const name& a, const name& b ) { return a.value >= b.value; }
      friend bool operator == ( const name& a, const name& b ) { return a.value == b.value; }

      friend bool operator == ( const name& a, uint64_t b ) { return a.value == b; }
      friend bool operator != ( const name& a, uint64_t b ) { return a.value != b; }

      friend bool operator != ( const name& a, const name& b ) { return a.value != b.value; }

      operator bool()const            { return value; }
      operator uint64_t()const        { return value; }
      operator unsigned __int128()const       { return value; }
   };


   inline std::vector<name> sort_names( std::vector<name>&& names ) {
      fc::deduplicate(names);
      return names;
   }

} } // eosio::chain

namespace std {
   template<> struct hash<eosio::chain::name> : private hash<uint64_t> {
      typedef eosio::chain::name argument_type;
      typedef typename hash<uint64_t>::result_type result_type;
      result_type operator()(const argument_type& name) const noexcept
      {
         return hash<uint64_t>::operator()(name.value);
      }
   };
};

namespace fc {
  class variant;
  void to_variant(const eosio::chain::name& c, fc::variant& v);
  void from_variant(const fc::variant& v, eosio::chain::name& check);
} // fc


FC_REFLECT( eosio::chain::name, (value) )
