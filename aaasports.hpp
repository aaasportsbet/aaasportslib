#pragma once
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <string>
#include <vector>

namespace aaasportslib
{
using namespace eosio;
using namespace std;

constexpr static const account_name eosiotoken = N("eosio.token");
/// token out of account
constexpr static const permission_name tokenoutp = N("token.out");

/// check issuer permission
static inline void checkissuerperm(const account_name &issuer,
                                   const account_name &perm)
{
  // only has nbaissuer permission account can put this action
  eosio_assert(is_account(issuer), "issuer account does not exist");
  require_auth(permission_level(issuer, perm));
}

/// check player perm
static inline void checkplayerperm(const account_name &player)
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

/// tranfer token args
struct transfer_args
{
  account_name from;
  account_name to;
  asset quantity;
  string memo;
};

/// token transfer
void token_transfer(const permission_name perm, const account_name &from,
                    const account_name &to, const asset &quant, const string memo)
{
  currency::inline_transfer(from, to, extended_asset(quant, eosiotoken), memo, perm);
}

} // namespace aaasportslib
