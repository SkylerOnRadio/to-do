#include "header_files/ui.h"
#include "header_files/global.h"
#include "header_files/inputHandling.h"
#include "header_files/taskClass.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

enum windowNames { TASKLIST, TASKDETAIL };

void createSubVector(std::vector<Tasks *> &tasks) {}

void displayTasks(WINDOW *win, std::vector<Tasks *> &tasks) {
  int maxy, maxx;
  getmaxyx(win, maxy, maxx);
  int maxTasks = maxy - 2;
  int y{1};
  int x{1};

  for (int i = start_index_unique; (i < tasks.size() && i <= maxTasks); ++i) {
    if (current_index_unique == i) {
      wattron(win, A_REVERSE);
      mvwaddstr(win, y, x, tasks.at(i)->task.c_str());
      wattroff(win, A_REVERSE);
    } else
      mvwaddstr(win, y, x, tasks.at(i)->task.c_str());
    ++y;
  }
}

void displayTaskDetails(WINDOW *win, std::vector<Tasks *> &tasks) {
  Tasks *task = tasks.at(current_index_unique);
  mvwaddstr(win, 1, 1, task->task.c_str());
}

void display(WINDOW *windows[], std::vector<Tasks *> &tasks) {
  int input;

  while (!exit_unique) {

    // create the tasks to display
    if (activeFilters_unique.empty())
      createSubVector(tasks);

    // display the tasks
    displayTasks(windows[TASKLIST], tasks);
    update_panels();
    doupdate();

    displayTaskDetails(windows[TASKDETAIL], tasks);
    update_panels();
    doupdate();

    input = wgetch(windows[0]);

    handleInput(input);
  }
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
  for (int i = 0; i < mainTasks.size(); ++i) {
    filteredTasks.push_back(mainTasks.at(i).get());
  }

  display(wins, filteredTasks);

  for (WINDOW *win : wins)
    delwin(win);
}
