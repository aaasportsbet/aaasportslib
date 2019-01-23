#include "aaasports.hpp"
#include <eosiolib/eosio.hpp>
#include <eosiolib/name.hpp>

namespace aaasportslib
{
using namespace eosio;

#define EOSIO_ABI_EX(TYPE, MEMBERS)                                                                                          \
  extern "C"                                                                                                                 \
  {                                                                                                                          \
    void apply(uint64_t receiver, uint64_t code, uint64_t action)                                                            \
    {                                                                                                                        \
      if (name(action) == "onerror"_n)                                                                                       \
      {                                                                                                                      \
        eosio_assert(name(code) == "eosio"_n,                                                                                \
                     "onerror action's are only valid from "                                                                 \
                     "the \"eosio\" system account");                                                                        \
      }                                                                                                                      \
      auto self = receiver;                                                                                                  \
                                                                                                                             \
      if ((code == self && name(action) != "transfer"_n) || (name(code) == "eosio.token"_n && name(action) == "transfer"_n)) \
      {                                                                                                                      \
        switch (action)                                                                                                      \
        {                                                                                                                    \
          EOSIO_DISPATCH_HELPER(TYPE, MEMBERS)                                                                               \
        }                                                                                                                    \
      }                                                                                                                      \
    }                                                                                                                        \
  }

} // namespace aaasportslib
