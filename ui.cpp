#include "header_files/ui.h"
#include "header_files/global.h"
#include "header_files/taskClass.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

void createSubVector(std::vector<Tasks *> &tasks) {}

void displayTasks(WINDOW *win, std::vector<Tasks *> &tasks) {
  int maxy, maxx;
  getmaxyx(win, maxy, maxx);
  int maxTasks = maxy - 2;
  int y{1};
  int x{1};

  for (int i = current_index_unique; i <= maxTasks; ++i) {
    std::string test = std::to_string(i);
    mvwaddstr(win, y, x, test.c_str());
    // mvwaddstr(win, y, x, tasks.at(i)->task.c_str());
    ++y;
  }
}

void display(WINDOW *win, std::vector<Tasks *> &tasks) {
  if (activeFilters_unique.empty())
    createSubVector(tasks);

  displayTasks(win, tasks);
}

void displayStart(std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  // setting up the windows for the panels library
  WINDOW *wins[2];
  PANEL *panels[2];

  wins[0] = newwin(LINES * .70, COLS, 0, 0);
  wins[1] = newwin(LINES * .30, COLS, (LINES * .70), 0);

  for (auto win : wins)
    box(win, 0, 0);

  panels[0] = new_panel(wins[0]);
  panels[1] = new_panel(wins[1]);

  update_panels();
  doupdate();

  std::vector<Tasks *> filteredTasks;
  int input;
  for (int i = 0; i < mainTasks.size(); ++i) {
    filteredTasks.push_back(mainTasks.at(i).get());
  }

  display(wins[0], filteredTasks);

  getch();
}
