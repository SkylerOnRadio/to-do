#include "header_files/inputHandling.h"
#include "header_files/global.h"
#include "header_files/taskClass.h"
#include "header_files/tasksHandling.h"
#include "header_files/ui.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

#define CTRL(key) (key & 0x1F)

void handleInput(int input, WINDOW *askWin, PANEL *askPanel, WINDOW *menuWin,
                 PANEL *menuPanel, WINDOW *helpWin, PANEL *helpPanel,
                 std::vector<Tasks *> &filteredTasks,
                 std::vector<std::unique_ptr<Tasks>> &mainTasks, int &start,
                 int end, int maxy) {
  switch (input) {
  case 'j':
    if (current_index_unique >= maxy - 3) {
      if (start + current_index_unique >= filteredTasks.size() - 4)
        current_index_unique >= end ? current_index_unique
                                    : ++current_index_unique;
      else {
        ++start;
      }
    } else {
      // if the display tasks in less than the maxy then the above condition
      // will fail, so to handle that we only increment current_index_unique if
      // it is smaller than end
      if (current_index_unique < end)
        ++current_index_unique;
    }
    break;

  case 'k':
    if (current_index_unique <= 3) {
      if (start + current_index_unique <= 3)
        current_index_unique <= 0 ? current_index_unique
                                  : --current_index_unique;
      else {
        --start;
      }
    } else {
      --current_index_unique;
    }
    break;

  case 'q':
    exit_unique = true;
    break;

  case CTRL('c'):
    exit_unique = true;
    break;

  case 'i': {
    // get the values to insert
    std::string task = askMenu(askWin, askPanel, "Enter the task.");
    if (task == "")
      break;
    std::string category = askMenu(askWin, askPanel, "Enter the category.");
    if (category == "")
      category = "None";

    insertNewTask(task, category, mainTasks);
    updateTasks_unique = true;

    break;
  }

  case 'd': {
    std::string confirm = askMenu(askWin, askPanel, "Delete task?");
    if (confirm != "y")
      break;
    deleteTask(current_id_unique, mainTasks);
    updateTasks_unique = true;
    break;
  }

  case 'c': {
    std::vector<std::string> options = {"Incomplete", "Ongoing", "Complete"};

    std::string res =
        selectMenu(menuWin, menuPanel, options, "Change status to?");
    int status;
    if (res == "")
      break;
    else if (res == "Incomplete")
      status = 0;
    else if (res == "Ongoing")
      status = 1;
    else if (res == "Complete")
      status = 2;

    changeStatus(current_id_unique, status, mainTasks);
    updateTasks_unique = true;
    break;
  }

  case '.':
    toggleComplete_unique = !toggleComplete_unique;
    updateTasks_unique = true;
    break;

  case 'h':
    helpMenu(helpWin, helpPanel);
    break;
  }
}
