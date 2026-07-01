#include "header_files/ui.h"
#include "header_files/global.h"
#include "header_files/inputHandling.h"
#include "header_files/taskClass.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

#define CTRL(key) (key & 0x1F)
enum windowNames { TASKLIST, TASKDETAIL, ASKMENU, SELECTIONMENU, HELPMENU };

void createSubVector(std::vector<Tasks *> &tasks,
                     std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  bool insert{true};
  tasks.clear();
  for (int i = 0; i < mainTasks.size(); ++i) {
    if (toggleComplete_unique)
      insert = mainTasks.at(i)->status == 2 ? false : true;

    if (!category_filter.empty())
      if (insert)
        insert = mainTasks.at(i)->category == category_filter ? true : false;

    if (insert)
      tasks.push_back(mainTasks.at(i).get());
  }
}

std::vector<Tasks *> createDisplaySubvector(std::vector<Tasks *> &filteredTasks,
                                            WINDOW *displayWin, int start) {
  int maxy, maxx;
  getmaxyx(displayWin, maxy, maxx);
  int maxTasks = maxy - 2;
  int end =
      std::min(start + static_cast<size_t>(maxTasks), filteredTasks.size());

  std::vector<Tasks *> res{filteredTasks.begin() + start,
                           filteredTasks.begin() + end};

  return res;
}

void displayTasks(WINDOW *win, std::vector<Tasks *> &tasks) {
  if (tasks.size() == 0)
    return;

  int y{1};
  int x{1};

  werase(win);
  box(win, 0, 0);
  for (int i = 0; i < tasks.size(); ++i) {
    if (current_index_unique == i) {
      current_id_unique = tasks.at(i)->id;
      wattron(win, A_REVERSE);
      mvwaddstr(win, y, x, tasks.at(i)->task.c_str());
      wattroff(win, A_REVERSE);
    } else
      mvwaddstr(win, y, x, tasks.at(i)->task.c_str());
    ++y;
  }
}

void displayTaskDetails(WINDOW *win, std::vector<Tasks *> &tasks) {
  if (tasks.size() == 0)
    return;

  Tasks *task = tasks.at(current_index_unique);
  werase(win);
  box(win, 0, 0);
  mvwaddstr(win, 1, 1, task->task.c_str());
}

void display(WINDOW *windows[], PANEL *panels[],
             std::vector<Tasks *> &filteredTasks,
             std::vector<std::unique_ptr<Tasks>>
                 &mainTasks_for_subvector_and_inputHandler_only) {
  int input;
  int start{0};
  int end{0};
  current_index_unique = 0;

  while (!exit_unique) {

    // create the tasks to display
    if (updateTasks_unique)
      createSubVector(filteredTasks,
                      mainTasks_for_subvector_and_inputHandler_only);
    updateTasks_unique = false;

    std::vector<Tasks *> tasks =
        createDisplaySubvector(filteredTasks, windows[TASKLIST], start);
    end = tasks.size() - 1;

    // display the tasks
    displayTasks(windows[TASKLIST], tasks);
    update_panels();
    doupdate();

    displayTaskDetails(windows[TASKDETAIL], tasks);
    update_panels();
    doupdate();

    input = wgetch(windows[0]);

    handleInput(input, windows[ASKMENU], panels[ASKMENU],
                windows[SELECTIONMENU], panels[SELECTIONMENU],
                windows[HELPMENU], panels[HELPMENU], filteredTasks,
                mainTasks_for_subvector_and_inputHandler_only, start, end,
                getmaxy(windows[TASKLIST]) - 3);

    update_panels();
    doupdate();
  }
}

void displayStart(std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  // setting up the windows for the panels library
  WINDOW *wins[5];
  PANEL *panels[5];

  wins[0] = newwin(LINES * .70, COLS, 0, 0);
  wins[1] = newwin(LINES * .30, COLS, (LINES * .70), 0);
  wins[2] = newwin(LINES * .08, COLS * .90, LINES * .08, COLS * .05);
  wins[3] = newwin(LINES * .10, COLS * .30, LINES * .30, COLS * .35);
  wins[4] = newwin(LINES * .80, COLS * .80, LINES * .10, COLS * .10);

  for (auto win : wins)
    box(win, 0, 0);

  panels[0] = new_panel(wins[0]);
  panels[1] = new_panel(wins[1]);
  panels[2] = new_panel(wins[2]);
  panels[3] = new_panel(wins[3]);
  panels[4] = new_panel(wins[4]);

  hide_panel(panels[ASKMENU]);
  hide_panel(panels[SELECTIONMENU]);
  hide_panel(panels[HELPMENU]);

  update_panels();
  doupdate();

  std::vector<Tasks *> filteredTasks;
  for (int i = 0; i < mainTasks.size(); ++i) {
    filteredTasks.push_back(mainTasks.at(i).get());
  }

  display(wins, panels, filteredTasks, mainTasks);

  for (WINDOW *win : wins)
    delwin(win);
}

std::string askMenu(WINDOW *win, PANEL *panel, std::string text) {
  std::string userInput{""};
  int ch;

  werase(win);
  curs_set(1);

  box(win, 0, 0);
  mvwaddstr(win, 0, COLS * 0.1, text.c_str());

  wmove(win, 1, 1);
  show_panel(panel);

  update_panels();
  doupdate();

  while (1) {
    ch = wgetch(win);

    if (ch > 31 && ch < 127) {
      userInput.push_back(ch);
    } else if (ch == CTRL('c') || ch == CTRL('q')) {
      userInput = "";
      break;
    } else if (ch == 127 || ch == KEY_BACKSPACE) {
      if (!userInput.empty())
        userInput.pop_back();
    } else if (ch == '\n')
      break;

    werase(win);
    box(win, 0, 0);
    mvwaddstr(win, 0, COLS * 0.1, text.c_str());
    mvwaddstr(win, 1, 1, userInput.c_str());
    update_panels();
    doupdate();
  }

  curs_set(0);
  werase(win);
  hide_panel(panel);
  update_panels();
  doupdate();

  return userInput;
}

std::string selectMenu(WINDOW *win, PANEL *panel,
                       std::vector<std::string> options, std::string text) {
  int ch;

  werase(win);

  box(win, 0, 0);
  int maxy, maxx;
  getmaxyx(win, maxy, maxx);
  mvwaddstr(win, 0, maxx * 0.1, text.c_str());
  mvwaddstr(win, maxy / 2, (maxx - options.begin()->size()) / 2,
            options.begin()->c_str());

  show_panel(panel);

  update_panels();
  doupdate();

  int index{0};
  while (1) {
    ch = wgetch(win);

    if (ch == 'j') {
      if (index == options.size() - 1)
        index = 0;
      else
        ++index;
    } else if (ch == 'k') {
      if (index == 0)
        index = options.size() - 1;
      else
        --index;
    } else if (ch == CTRL('c') || ch == 'q') {
      index = -1;
      break;
    } else if (ch == '\n')
      break;

    werase(win);
    box(win, 0, 0);
    mvwaddstr(win, 0, maxx * 0.1, text.c_str());
    mvwaddstr(win, maxy / 2, (maxx - options.at(index).size()) / 2,
              options.at(index).c_str());
    update_panels();
    doupdate();
  }

  curs_set(0);
  werase(win);
  hide_panel(panel);
  update_panels();
  doupdate();

  if (index == -1)
    return "";
  return options.at(index);
}

void helpMenu(WINDOW *win, PANEL *panel) {
  int ch;

  werase(win);

  box(win, 0, 0);
  int maxy, maxx;
  getmaxyx(win, maxy, maxx);
  show_panel(panel);

  std::string text = "Keymaps for the program is: ";
  mvwaddstr(win, 1, (maxx - text.size()) / 2, text.c_str());
  std::vector<std::string> keymaps = {
      "q --> Quit the current screen (for screens that do not take user "
      "input)",
      "C-q --> Quit the current screen (for screens that do take user input)",
      "j --> Go to the task above the highlighted task",
      "k --> Go to the task below the highlighted task",
      "i --> Insert a new task",
      "d --> Delete the highlighted task",
      "c --> Change the status of the highlighted task",
      "s --> Search through the tasks",
      "f --> Search through the categories",
      "e --> Edit the highlighted task",
      ". --> Edit the highlighted task",
  };

  int y = (maxy - (2 + keymaps.size() + 2)) / 2;
  for (std::string keymap : keymaps) {
    if (keymap.size() > maxx - 10) {
      mvwaddstr(win, y++, 2, keymap.substr(0, maxx - 4).c_str());
      keymap = keymap.substr(maxx - 4, keymap.size());
      while (keymap.size() > maxx) {
        std::string substr = keymap.substr(0, maxx - 4);
        keymap = keymap.substr(maxx - 4, keymap.size());
        mvwaddstr(win, y, 10, substr.c_str());
        ++y;
      }
      mvwaddstr(win, y, 10, keymap.c_str());
    } else
      mvwaddstr(win, y, 2, keymap.c_str());
    ++y;
  }

  update_panels();
  doupdate();

  while (1) {
    int ch = wgetch(win);
    if (ch == CTRL('c') || ch == 'q')
      break;
  }

  curs_set(0);
  werase(win);
  hide_panel(panel);
  update_panels();
  doupdate();
}
