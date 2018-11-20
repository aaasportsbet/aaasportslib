#include "aaasports.hpp"
#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>

namespace aaasportslib
{
using namespace eosio;
using namespace std;

class supervisorbase
{
public:
  supervisorbase(account_name n)
      : _self(n), _supervisors(n, n), _challenges(n, n) {}

  // @abi table
  struct supervisor
  {
    uint64_t id;
    account_name player; // player account name
    asset bet_quant;     // player bet how much to become a supervisor
    uint64_t round_id;   // which round the player be selected
    uint64_t challenge_count =
        0;             // how many times the player disagree with round public result
    asset award_quant; // the award for a supervisor
    uint8_t left_round =
        MAX_WITNESS_COUNT; // how many round left for a player can witness

    uint64_t primary_key() const { return id; }

    EOSLIB_SERIALIZE(supervisor, (id)(player)(bet_quant)(round_id)(
                                     challenge_count)(award_quant)(left_round))
  };

  typedef eosio::multi_index<N(supervisors), supervisor> supervisors;
  /// supervisors
  supervisors _supervisors;

  /// add a new supervisor
  void _addsupervisor(const account_name &player, const uint64_t &round_id,
                      const asset &quant)
  {
    for (auto &item : _supervisors)
    {
      if (item.player == player)
      {
        // if player already exist, just update left_round
        _supervisors.modify(_supervisors.find(item.id), _self, [&](auto &ns) {
          ns.bet_quant += quant;
          ns.round_id = round_id;
          ns.left_round += MAX_WITNESS_COUNT;
        });
      }
      else if (item.left_round == 0)
      {
        // if player's left_round == 0, earse it
        _supervisors.erase(_supervisors.find(item.id));
      }
    }

    // add a new supervisor
    _supervisors.emplace(_self, [&](auto &ns) {
      ns.id = _supervisors.available_primary_key();
      ns.player = player;
      ns.bet_quant = quant;
      ns.round_id = round_id;
      ns.challenge_count = 0;
      ns.left_round = MAX_WITNESS_COUNT;
    });
  }

  // check whether player is a supervisor
  bool _areyousupervisor(const account_name &player)
  {
    for (auto &item : _supervisors)
    {
      if (item.player == player && item.left_round > 0)
      {
        return true;
      }
    }

    return false;
  }

  /// @abi table
  struct challenge
  {
    uint64_t id;
    uint64_t round_id;
    account_name player;
    string memo;

    uint64_t primary_key() const { return id; }
    uint64_t byround() const { return round_id; }

    EOSLIB_SERIALIZE(challenge, (id)(player)(round_id)(memo))
  };
  typedef eosio::multi_index<N(challenges), challenge> challenges;
  /// challenges
  challenges _challenges;

  /// @abi action
  void challengebet(const account_name &player, const uint64_t &round_id,
                    const string &memo)
  {
    checkperm(player);

    eosio_assert(_areyousupervisor(player), "you must be a supervisor");
  }

private:
  static const uint8_t MAX_WITNESS_COUNT = 21;
  account_name _self;
};

} // namespace aaasportslib