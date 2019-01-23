#pragma once
#include <eosiolib/action.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
// #include <eosiolib/currency.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <string>
#include <vector>
#define DEBUG
#include "logger.hpp"

namespace aaasportslib {
using namespace eosio;
using namespace std;

// some extra field for table
struct extradata {
  string key;
  string value;

  EOSLIB_SERIALIZE(extradata, (key)(value));
};

/// player bet
struct bet {
  uint64_t id;
  uint64_t round_id;
  name player;
  int8_t bet_val;
  uint64_t share;
  uint64_t status; // win or not

  EOSLIB_SERIALIZE(bet, (id)(round_id)(player)(bet_val)(share)(status))
};

static const uint64_t default_public_duration =
    7200000000; // 2 * 60 * 60 * 1000 * 1000
static const uint8_t default_bet_fee_percent = 1;

/// check permission
static inline void checkperm(const name &acc, const name &perm = "active"_n) {
  eosio_assert(is_account(acc), "account does not exist");
  require_auth(permission_level(acc, perm));
}

/// check asset
static inline void checkasset(const asset &a) {
  eosio_assert(a.is_valid(), "invalid asset");
  eosio_assert(a.amount >= 0, "must bet positive quantity");
}

static inline bool startsWith(string s, string sub) { return s.find(sub) == 0; }

static inline bool endsWith(string s, string sub) {
  return s.rfind(sub) == (s.length() - sub.length());
}

template <typename T = uint64_t>
static inline T convert(std::string const &value) {
  logger_info("conver value: ", value);
  T result = 0;

  bool neg = false;
  char const *p = value.c_str();
  char const *q = p + value.size();

  // check neg
  if (*p == '-') {
    neg = true;
    ++p;
  } else if (*p == '+') {
    neg = false;
    ++p;
  }

  while (p < q) {
    result = (result << 1) + (result << 3) + *(p++) - '0';
  }

  if (neg) {
    result = -result;
  }

  return result;
}

static const char *charmap = "0123456789";
template <typename T = uint64_t>
static inline string to_string(const T &value) {
  string result;
  result.reserve(40); // max. 40 digits possible ( uint64_t has 20)
  T helper = value;

  do {
    result += charmap[helper % 10];
    helper /= 10;
  } while (helper);
  reverse(result.begin(), result.end());
  return result;
}

/// tranfer token args
struct transfer_args {
  name from;
  name to;
  asset quantity;
  string memo;
};

/// calc bet unit fee
static const asset get_bet_fee(const asset &unit,
                               const uint16_t bet_fee_percent) {
  auto fee = unit / 1000 * bet_fee_percent;
  checkasset(fee);

  return fee;
}

/// calc bet ast award
static const asset get_bet_ast_award(const asset &fee, const uint64_t share,
                                     const uint64_t bet_ast_unit) {
  const asset total_bet_fee = fee * share;
  auto ast =
      asset(total_bet_fee.amount * bet_ast_unit, symbol(symbol_code("AST"), 4));
  checkasset(ast);

  return ast;
}

} // namespace aaasportslib
