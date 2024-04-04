#pragma once
#include <assert.h>

#include "service.h"

class Test {
 public:
  static void testAll() {
    Test::testMark();
    Test::testRemove();
  }
  static void testRemove() {
    Service service = Service();
    service.removeSchool("Avram_Iancu", Address(46.77, 23.60));
    assert(service.getSchools().size() == 4);
  }
  static void testMark() {
    Service service = Service();
    std::vector<School> result = service.getSchoolsByDate(Date(24, 4, 2022));
    assert(service.getSchools()[0].getVisited() == true);
    assert(service.getSchools()[1].getVisited() == true);
    assert(result.size() == 2);
  }
};