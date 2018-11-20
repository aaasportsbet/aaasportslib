#pragma once
#include <eosiolib/action.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <string>
#include <vector>
#define DEBUG
#include "logger.hpp"

namespace aaasportslib
{
using namespace eosio;
using namespace std;

static const uint64_t default_public_duration =
    7200000000; // 2 * 60 * 60 * 1000 * 1000
static const uint8_t default_bet_fee_percent = 1;
constexpr static const account_name eosiotoken = N(eosio.token);
/// token out of account
constexpr static const permission_name default_tokenoutp = N(token.out);

/// check permission
static inline void checkperm(const account_name &acc,
                             const permission_name &perm = N(active))
{
  eosio_assert(is_account(acc), "account does not exist");
  require_auth(permission_level(acc, perm));
}

/// check asset
static inline void checkasset(const asset &a)
{
  eosio_assert(a.is_valid(), "invalid asset");
  eosio_assert(a.amount > 0, "must bet positive quantity");
}

template <typename T = uint64_t>
static inline T convert(std::string const &value)
{
  logger_info("conver value: ", value);
  T result = 0;

  bool neg = false;
  char const *p = value.c_str();
  char const *q = p + value.size();

  // check neg
  if (*p == '-')
  {
    neg = true;
    ++p;
  }
  else if (*p == '+')
  {
    neg = false;
    ++p;
  }

  while (p < q)
  {
    result = (result << 1) + (result << 3) + *(p++) - '0';
  }

  if (neg)
  {
    result = -result;
  }

  return result;
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
                    const account_name &to, const asset &quant,
                    const string memo)
{
  currency::inline_transfer(from, to, extended_asset(quant, eosiotoken), memo,
                            perm);
}

/// calc bet unit fee
static const asset &getbetfee(const asset &unit)
{
  auto fee = unit / 100 * default_bet_fee_percent;
  checkasset(fee);

  return fee;
}

} // namespace aaasportslib
