#include "../include/lista.h"
#include "../include/8a.h"
#include "../include/8b.h"
#include <iostream>

using namespace std;

int main() {

   Lista l;
   l = creare();
   cout << lcm_list(l._prim) << endl;
   cout << "###############\n";
   l = creare();
   TElem a, b;
   cout << "Value: ";
   cin >> a;
   cout << "Substitution: ";
   cin >> b;
   substitute_in_list(l._prim, a, b);
   tipar(l);
   
   return 0;
}
