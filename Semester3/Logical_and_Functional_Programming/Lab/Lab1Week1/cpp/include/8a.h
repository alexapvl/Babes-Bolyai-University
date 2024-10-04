#pragma once

int lowest_common_multiple(TElem a, TElem b, TElem i = 1) {
  if (i % a == 0 && i % b == 0) {
    return i;
  }
  return lowest_common_multiple(a, b, i + 1);
}

int lcm_list(PNod nod) {
  if(nod == nullptr) {
    return 1;
  }

  return lowest_common_multiple(nod->e, lcm_list(nod->urm), 1);
}
