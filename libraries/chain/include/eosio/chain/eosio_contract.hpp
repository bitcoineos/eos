/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosio/chain/types.hpp>
#include <eosio/chain/contract_types.hpp>

namespace eosio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_BE_newaccount(apply_context&);
   void apply_BE_updateauth(apply_context&);
   void apply_BE_deleteauth(apply_context&);
   void apply_BE_linkauth(apply_context&);
   void apply_BE_unlinkauth(apply_context&);

   /*
   void apply_BE_postrecovery(apply_context&);
   void apply_BE_passrecovery(apply_context&);
   void apply_BE_vetorecovery(apply_context&);
   */

   void apply_BE_setcode(apply_context&);
   void apply_BE_setabi(apply_context&);

   void apply_BE_abortdelay(apply_context&);
   ///@}  end action handlers

} } /// namespace eosio::chain
