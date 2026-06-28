#include "header_files/inputHandling.h"
#include "header_files/global.h"
#include "header_files/tasksHandling.h"
#include "header_files/ui.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

#define CTRL(key) (key & 0x1F)

void handleInput(int input, WINDOW *win, PANEL *panel,
                 std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  switch (input) {
  case 'q':
    exit_unique = true;
    break;

  case CTRL('c'):
    exit_unique = true;
    break;

  case 'i': {
    // get the values to insert
    std::string task = askMenu(win, panel, "Enter the task.");
    if (task == "")
      break;
    std::string category = askMenu(win, panel, "Enter the category.");
    if (category == "")
      category = "None";

    insertNewTask(task, category, mainTasks);

    break;
  }
  }
}
