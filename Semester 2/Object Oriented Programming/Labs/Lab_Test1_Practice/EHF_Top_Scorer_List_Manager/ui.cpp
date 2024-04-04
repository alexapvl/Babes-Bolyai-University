#include "ui.h"

UI::UI() { this->service = Service(); }
UI::~UI() {}

void UI::printMenu() {
  std::cout << "1. Add player\n";
  std::cout << "2. Show all players\n";
  std::cout << "3. Show players from a given team in descending order based on "
               "number of goals scored\n";
  std::cout << "4. Exit\n";
}

void UI::start() {
  int choice;
  while (true) {
    this->printMenu();
    std::cin >> choice;
    try {
      if (choice == 1) {
        std::string name, nationality, team;
        int goals;
        std::cout << "Name: ";
        std::cin >> name;
        std::cout << "Nationality: ";
        std::cin >> nationality;
        std::cout << "Team: ";
        std::cin >> team;
        std::cout << "Number of goals: ";
        std::cin >> goals;
        this->service.addPlayer(name, nationality, team, goals);
        std::cout << "Player added successfully\n";
      } else if (choice == 2) {
        std::vector<Player> players = this->service.getPlayers();
        for (int i = 0; i < players.size(); ++i) {
          std::cout << players[i].toStr() << "\n";
        }
      } else if (choice == 3) {
        std::string team;
        std::cout << "Team: ";
        std::cin >> team;
        std::vector<Player> sortedPlayers =
            this->service.getPlayersByGivenTeamSortedByGoalsDescending(team);
        for (int i = 0; i < sortedPlayers.size(); ++i) {
          std::cout << sortedPlayers[i].toStr() << "\n";
        }
      } else if (choice == 4) {
        break;
      } else {
        throw std::runtime_error("Invalid option");
      }
    } catch (std::runtime_error& e) {
      std::cout << e.what() << "\n";
    }
  }
}