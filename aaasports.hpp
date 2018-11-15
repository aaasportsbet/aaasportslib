#pragma once
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <string>
#include <vector>

namespace aaasportslib
{
using namespace eosio;
using namespace std;

/// check issuer permission
static inline void checkissuerperm(const account_name &issuer,
                                   const account_name &perm)
{
  // only has nbaissuer permission account can put this action
  eosio_assert(is_account(issuer), "issuer account does not exist");
  require_auth(permission_level(issuer, perm));
}

/// check player perm
static inline void checkplayerperm(const name &player)
{
  eosio_assert(is_account(player), "player account does not exist");
  require_auth(player);
}

/// check asset
static inline void checkasset(const asset &a)
{
  eosio_assert(a.is_valid(), "invalid asset");
  eosio_assert(a.amount > 0, "must bet positive quantity");
}

/// tranfer token args
struct transfer_args
{
  name from;
  name to;
  asset quantity;
  string memo;
};

constexpr static const account_name eosiotoken = N("eosio.token");

// string split
static inline vector<string> split(string strtem, char a)
{
  vector<string> strvec;

  string::size_type pos1, pos2;
  pos2 = strtem.find(a);
  pos1 = 0;
  while (string::npos != pos2)
  {
    strvec.push_back(strtem.substr(pos1, pos2 - pos1));

    pos1 = pos2 + 1;
    pos2 = strtem.find(a, pos1);
  }
  strvec.push_back(strtem.substr(pos1));
  return strvec;
}

/// token transfer
void token_transfer(const permission_level &perm, const account_name &from,
                    const name &to, const asset &quant, const string memo)
{
  action(perm, eosiotoken, N("transfer"), transfer_args{from, to, quant, memo})
      .send();
}

} // namespace aaasportslib
