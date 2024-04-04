#include "domain.h"

Player::Player(std::string name, std::string nationality, std::string team,
               int goals)
    : name(name), nationality(nationality), team(team), goals(goals) {}
Player::~Player() {}

void Player::setName(std::string name) { this->name = name; }
void Player::setNationality(std::string nationality) {
  this->nationality = nationality;
}
void Player::setTeam(std::string team) { this->team = team; }
void Player::setGoals(int goals) { this->goals = goals; }

std::string Player::getName() { return this->name; }
std::string Player::getNationality() { return this->nationality; }
std::string Player::getTeam() { return this->team; }
int Player::getGoals() { return this->goals; }
