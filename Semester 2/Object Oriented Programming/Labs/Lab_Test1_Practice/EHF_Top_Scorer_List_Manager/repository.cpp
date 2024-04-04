#include "repository.h"

Repo::Repo() { this->data = std::vector<Player>(); }
Repo::~Repo() {}

void Repo::addPlayer(Player p) { this->data.push_back(p); }

bool Repo::doesExist(Player p) {
  for (int i = 0; i < this->getPlayers().size(); ++i) {
    if (this->data[i] == p) return true;
  }
  return false;
}

std::vector<Player> Repo::getPlayers() { return this->data; }

std::vector<Player> Repo::getPlayersForGivenTeam(std::string team) {
  std::vector<Player> players = std::vector<Player>();
  for (int i = 0; i < this->getPlayers().size(); ++i) {
    if (this->data[i].getTeam() == team) players.push_back(this->data[i]);
  }
  return players;
}