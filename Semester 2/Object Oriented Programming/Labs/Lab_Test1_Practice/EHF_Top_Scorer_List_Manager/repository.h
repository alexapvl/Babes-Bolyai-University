#pragma once
#include <vector>

#include "domain.h"

class Repo {
 private:
  std::vector<Player> data;

 public:
  Repo();
  ~Repo();
  void addPlayer(Player p);
  bool doesExist(Player p);
  std::vector<Player> getPlayers();
  std::vector<Player> getPlayersForGivenTeam(std::string team);
};