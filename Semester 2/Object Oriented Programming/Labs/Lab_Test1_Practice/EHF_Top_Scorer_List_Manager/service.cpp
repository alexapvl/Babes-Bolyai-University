#include "service.h"

Service::Service() {
  this->repo = Repo();
  Player p1 = Player("Nora_Mork", "NOR", "Larvik", 83);
  Player p2 = Player("Isabelle_Gullden", "SWE", "CSM_Bucharest", 80);
  Player p3 = Player("Cristina_Neagu", "ROU", "Buducnost", 63);
  Player p4 = Player("Allison_Pineau", "FRA", "HCM_Baia_Mare", 82);
  Player p5 = Player("Ilina_Ekaterina", "RUS", "CSM_Bucharest", 87);

  repo.addPlayer(p1);
  repo.addPlayer(p2);
  repo.addPlayer(p3);
  repo.addPlayer(p4);
  repo.addPlayer(p5);
}
Service::~Service() {}

void Service::addPlayer(std::string name, std::string nationality,
                        std::string team, int goals) {
  /*
  Adds a player
  If the player already exists, then a message will be displayed in the UI
  */
  Player player = Player(name, nationality, team, goals);
  if (this->repo.doesExist(player))
    throw std::runtime_error("Player already exists");
  this->repo.addPlayer(player);
}

std::vector<Player> Service::getPlayers() { return this->repo.getPlayers(); }

std::vector<Player> Service::getPlayersByGivenTeamSortedByGoalsDescending(
    std::string team) {
  /*
  Gets the players form a given team into a separate vector, then sorts the
  vector in descending order based on the number of goals scored
  */
  std::vector<Player> playersSorted = std::vector<Player>();
  for (int i = 0; i < this->getPlayers().size(); ++i) {
    if (this->getPlayers()[i].getTeam() == team)
      playersSorted.push_back(this->getPlayers()[i]);
  }
  std::sort(playersSorted.begin(), playersSorted.end(),
            [](Player p1, Player p2) { return p1.getGoals() > p2.getGoals(); });
  return playersSorted;
}