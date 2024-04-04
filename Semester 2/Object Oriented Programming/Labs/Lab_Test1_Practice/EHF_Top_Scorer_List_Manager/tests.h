#pragma once
#include <assert.h>

#include "service.h"

class Test {
 public:
  static void testAll() {
    Test::testAdd();
    Test::testPlayersFromGivenTeam();
  }
  static void testAdd() {
    Service service = Service();
    assert(service.getPlayers().size() == 5);
  }
  static void testPlayersFromGivenTeam() {
    Service service = Service();
    std::vector<Player> sortedPlayers =
        service.getPlayersByGivenTeamSortedByGoalsDescending("CSM_Bucharest");
    assert(sortedPlayers[0].getGoals() == 87);
    assert(sortedPlayers[1].getGoals() == 80);
  }
};
