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

void handleInput(int input, WINDOW *wins[], PANEL *panels[],
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
    setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "insert");
    // get the values to insert
    std::string task =
        askMenu(wins[ASKMENU], panels[ASKMENU], "Enter the task.");
    if (task == "")
      break;
    std::string category =
        askMenu(wins[ASKMENU], panels[ASKMENU], "Enter the category.");
    if (category == "")
      category = "None";

    insertNewTask(task, category, mainTasks);
    updateTasks_unique = true;

    break;
  }

  case 'd': {
    setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "delete");
    std::string confirm =
        askMenu(wins[ASKMENU], panels[ASKMENU], "Delete task?");
    if (confirm != "y")
      break;
    deleteTask(current_id_unique, mainTasks);
    updateTasks_unique = true;
    break;
  }

  case 'c': {
    std::vector<std::string> options = {"Incomplete", "Ongoing", "Complete"};

    setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "status");
    std::string res = selectMenu(wins[SELECTIONMENU], panels[SELECTIONMENU],
                                 options, "Change status to?");
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
    setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "help");
    helpMenu(wins[HELPMENU], panels[HELPMENU]);
    break;

  case 's': {
    setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "task-filter");
    std::string filterText = {""};
    while (1) {
      int ch = getch();
      if (ch > 31 && ch < 127) {
        filterText += ch;
      } else if (ch == CTRL('q') || ch == CTRL('c')) {
        filterText = "";
        break;
      } else if ((ch == KEY_BACKSPACE || ch == 127) && !filterText.empty()) {
        filterText.pop_back();
      } else if (ch == '\n') {
        task_filter = filterText;
        break;
      }

      setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "task-filter",
                    filterText);
    }
    updateTasks_unique = true;
    break;
  }

  case 'f': {
    setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "category-filter");
    std::string filterText = {""};
    while (1) {
      int ch = getch();
      if (ch > 31 && ch < 127) {
        filterText += ch;
      } else if (ch == CTRL('q') || ch == CTRL('c')) {
        filterText = "";
        break;
      } else if ((ch == KEY_BACKSPACE || ch == 127) && !filterText.empty()) {
        filterText.pop_back();
      } else if (ch == '\n') {
        category_filter = filterText;
        break;
      }

      setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "category-filter",
                    filterText);
    }
    updateTasks_unique = true;
    break;
  }

  case 'r':
    task_filter = "";
    category_filter = "";
    toggleComplete_unique = false;
    updateTasks_unique = true;
    break;
  }
}
