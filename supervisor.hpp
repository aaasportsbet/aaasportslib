#include "aaasports.hpp"
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>

namespace aaasportslib
{
using namespace eosio;

class supervisorbase
{
public:
  supervisorbase(account_name n) : _self(n) {}

private:
  account_name _self;
};

} // namespace aaasportslib