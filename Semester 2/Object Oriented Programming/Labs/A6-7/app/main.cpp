// Local movie database
#include "../include/ui/ui.h"

int main(int argc, char* argv[]) {
  std::cout << "Choose the file type for the user:\n";
  std::cout << "1. CSV\n";
  std::cout << "2. HTML\n";
  std::cout << ">>> ";
  int choice;
  UI::readInt(choice);
  UI ui = UI(choice);
  ui.runApp();
  return 0;
}