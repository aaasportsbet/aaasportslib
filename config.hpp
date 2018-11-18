#include "aaasports.hpp"
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>

namespace aaasportslib {
using namespace eosio;

class configbase {
public:
  configbase(account_name n) : _self(n), _configs(n, n) {}

  // @abi table configs i64
  struct config {
    uint64_t id;
    // issuer permission
    permission_name issuerperm = N(active);
    // token out permission
    permission_name tokenoutperm = N(active);
    // how long a game can last, nba: 3hours, soccer: 2hours
    uint64_t game_duration;
    // how long a game result publish to
    // the contract before lottery,
    // default 2hours
    uint64_t public_duration = 7200000000;

    uint64_t primary_key() const { return id; }

    EOSLIB_SERIALIZE(
        config, (id)(issuerperm)(tokenoutperm)(game_duration)(public_duration))
  };

  typedef eosio::multi_index<N("config"), config> configs;

  /// config
  configs _configs;

  /// set config, should only be used in test
  /// @abi action
  void setconfig(const permission_name issuerperm,
                 const permission_name tokenoutperm,
                 const uint64_t game_duration, const uint64_t public_duration) {
    checkperm(_self);

    // find config
    auto cfg = _configs.find(0);

    auto lambda = [&](auto &nc) {
      nc.id = 0;
      nc.issuerperm = issuerperm;
      nc.tokenoutperm = tokenoutperm;
      nc.game_duration = game_duration;
      nc.public_duration = public_duration;
    };
    if (cfg == _configs.end()) {
      _configs.emplace(_self, lambda);
    } else {
      _configs.modify(cfg, _self, lambda);
    }
  }

  const config &getconfigOrDefault(const config &defaultcfg) {
    // find config
    auto cfg = _configs.find(0);
    if (cfg == _configs.end()) {
      return defaultcfg;
    }

    return *cfg;
  }

private:
  account_name _self;
  // inline account_name get_self() const { return _self; }
};

} // namespace aaasportslib