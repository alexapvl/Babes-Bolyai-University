#include <assert.h>

#include "service.h"

class Test {
 public:
  static void testAll() {
    Test::testRemove();
    Test::testUnpaid();
  }
  static void testRemove() {
    Repo repo = Repo();
    Bill bill1 = Bill("a", "Digi", Date(21, 4, 2016), 75, false);
    Bill bill2 = Bill("b", "Digi", Date(16, 3, 2016), 75, true);
    repo.addBill(bill1);
    assert(repo.getAllBills().size() == 1);
    repo.addBill(bill2);
    assert(repo.getAllBills().size() == 2);
  }
  static void testUnpaid() {
    Service service = Service();
    assert(service.totalUnpaid() == 225);
  }
};