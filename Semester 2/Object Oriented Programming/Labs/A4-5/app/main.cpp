// Local movie database
#include "../include/ui/ui.h"
#include "../tests/tests.h"

int main(int argc, char* argv[]) {
  if (argc == 2 && strcmp(argv[1], "--test") == 0) {
    testAll();
    std::cout << "All tests passed!" << std::endl;
    return 0;
  }
  UI ui = UI();
  ui.runApp();
  return 0;
}