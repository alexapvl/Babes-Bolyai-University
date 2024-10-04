#pragma once
#include "lista.h"

void substitute_in_list(PNod nod, TElem value, TElem substitution) {
  if(nod == nullptr) {
    return;
  }
  if(nod->e == value) {
    nod->e = substitution;
  }

  return substitute_in_list(nod->urm, value, substitution);
}
