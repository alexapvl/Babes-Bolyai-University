#pragma once
#include <assert.h>

#include "service.h"

class Test {
 public:
  static void testAll() {
    Test::testAdd();
    Test::testHeal();
  }
  static void testAdd() {
    Service s = Service();
    assert(s.getPatients().size() == 5);
    s.addPatient("Alexandru_Apavaloaiei", 19, false, 7);
    assert(s.getPatients().size() == 6);
  }
  static void testHeal() {
    Service s = Service();
    assert(s.getPatients()[1].getInfected() == true);
    s.healPatient("Lidia_Aspen");
    assert(s.getPatients()[1].getInfected() == false);
  }
};