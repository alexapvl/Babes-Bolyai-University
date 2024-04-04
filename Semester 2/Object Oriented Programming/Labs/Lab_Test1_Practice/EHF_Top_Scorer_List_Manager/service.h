#pragma once
#include "repository.h"

class Service {
 private:
  Repo repo;

 public:
  Service();
  ~Service();
  void addPlayer(std::string name, std::string nationality, std::string team,
                 int goals);
  std::vector<Player> getPlayers();
  std::vector<Player> getPlayersByGivenTeamSortedByGoalsDescending(
      std::string team);
};