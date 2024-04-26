#include "domain.h"
using namespace std;

double Option::computeProfit() {
  if (optionType == "CALL") {
    return sellPrice - strikePrice;
  } else { // PUT
    return strikePrice - buyPrice;
  }
}