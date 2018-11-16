#include "aaasports.hpp"
#include <eosiolib/eosio.hpp>

namespace aaasportslib
{
using namespace eosio;

#define EOSIO_ABI_EX(TYPE, MEMBERS)                                                                       \
  extern "C"                                                                                              \
  {                                                                                                       \
    void apply(uint64_t receiver, uint64_t code, uint64_t action)                                         \
    {                                                                                                     \
      if (action == N(onerror))                                                                           \
      {                                                                                                   \
        /* onerror is only valid if it is for the "eosio" code account and                                \
         * authorized by "eosio"'s                                                                        \
         * "active permission */                                                                          \
        eosio_assert(code == N(eosio), "onerror action's are only valid from "                            \
                                       "the \"eosio\" system account");                                   \
      }                                                                                                   \
      auto self = receiver;                                                                               \
                                                                                                          \
      if ((code == self && action != N(transfer)) || (code == N(eosio.token) && action == N(transfer)) || \
          action == N(onerror))                                                                           \
      {                                                                                                   \
        TYPE thiscontract(self);                                                                          \
        switch (action)                                                                                   \
        {                                                                                                 \
          EOSIO_API(TYPE, MEMBERS)                                                                        \
        }                                                                                                 \
        /* does not allow destructor of thiscontract to run: eosio_exit(0); */                            \
      }                                                                                                   \
    }                                                                                                     \
  }

} // namespace aaasportslib
