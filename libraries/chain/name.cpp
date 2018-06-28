#include <eosio/chain/name.hpp>
#include <fc/variant.hpp>
#include <boost/algorithm/string.hpp>
#include <fc/exception/exception.hpp>
#include <eosio/chain/exceptions.hpp>

namespace eosio { namespace chain { 

   void name::set( const char* str ) {
      const auto len = strnlen(str, 12);
      EOS_ASSERT(len <= 11, name_type_exception, "Name is longer than 11 characters (${name}) ", ("name", string(str)));
      value = string_to_name(str);
      EOS_ASSERT(to_string() == string(str), name_type_exception,
                 "Name not properly normalized (name: ${name}, normalized: ${normalized}) ",
                 ("name", string(str))("normalized", to_string()));
   }

   // keep in sync with name::to_string() in contract definition for name
   name::operator string()const {
     static const char* charmap = ".0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ/";

      string str(11,'.');

      uint64_t tmp = value;
      for( uint32_t i = 0; i <= 10; ++i ) {
         char c = charmap[tmp & (i == 0 ? 0x0f : 0x3f)];
         str[10-i] = c;
         tmp >>= (i == 0 ? 4 : 6);
      }

      boost::algorithm::trim_right_if( str, []( char c ){ return c == '.'; } );
      return str;
   }

} } /// eosio::chain

namespace fc {
  void to_variant(const eosio::chain::name& c, fc::variant& v) { v = std::string(c); }
  void from_variant(const fc::variant& v, eosio::chain::name& check) { check = v.get_string(); }
} // fc
