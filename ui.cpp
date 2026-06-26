#include "header_files/ui.h"
#include "header_files/taskClass.h"
#include <memory>
#include <ncurses.h>
#include <vector>

void displayStart(WINDOW *win, std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  std::vector<Tasks *> filteredTasks;
  for (int i = 0; i < mainTasks.size(); ++i) {
    filteredTasks.push_back(mainTasks.at(i).get());
  }
  int input;
}
