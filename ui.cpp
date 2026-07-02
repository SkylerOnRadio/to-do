#include "header_files/ui.h"
#include "header_files/global.h"
#include "header_files/inputHandling.h"
#include "header_files/taskClass.h"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <string_view>
#include <vector>

#define CTRL(key) (key & 0x1F)

/* ============== SCREEN DISPLAYS ============== */
void displayTasks(WINDOW *win, std::vector<Tasks *> &tasks) {
  if (tasks.size() == 0) {
    werase(win);
    box(win, 0, 0);
    return;
  }

  int y{1};

  werase(win);
  box(win, 0, 0);
  for (int i = 0; i < tasks.size(); ++i) {
    int start = 1;
    if (current_index_unique == i) {
      current_id_unique = tasks.at(i)->id;
      wattron(win, A_REVERSE);
      mvwaddstr(win, y, start, tasks.at(i)->task.c_str());
      start += tasks.at(i)->task.size() + 10;
      mvwaddstr(win, y, start, tasks.at(i)->category.c_str());
      wattroff(win, A_REVERSE);
    } else {
      mvwaddstr(win, y, start, tasks.at(i)->task.c_str());
      start += tasks.at(i)->task.size() + 10;
      mvwaddstr(win, y, start, tasks.at(i)->category.c_str());
    }
    ++y;
  }
}

void displayTaskDetails(WINDOW *win, std::vector<Tasks *> &tasks) {
  if (tasks.size() == 0) {
    werase(win);
    box(win, 0, 0);
    return;
  }

  Tasks *task = tasks.at(current_index_unique);
  werase(win);
  box(win, 0, 0);
  mvwaddstr(win, 1, 1, task->task.c_str());
}

void setStatusMenu(WINDOW *win, PANEL *panel, std::string_view mode,
                   std::string filter) {
  int y = 0;
  werase(win);
  wattron(win, A_REVERSE);
  mvwhline(win, y, 0, ' ', getmaxx(win));
  int start = 3;
  char separator = '>';
  std::string showing;
  if (toggleComplete_unique)
    showing = "INCOMPLETE";
  else
    showing = "ALL";
  if (!task_filter.empty())
    showing += " | " + task_filter;
  if (!category_filter.empty())
    showing += " | " + category_filter;

  if (mode == "main") {
    std::string name = "LIST";
    std::string id = "TASK ID: " + std::to_string(current_id_unique);
    std::string text = "Press 'h' to open the help menu.";

    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, id.c_str());
    start += id.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, showing.c_str());
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else if (mode == "insert") {
    std::string name = "INSERT";
    std::string text = "Press 'C-q' to exit.";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else if (mode == "delete") {
    std::string name = "DELETE";
    std::string id = "TASK ID: " + std::to_string(current_id_unique);
    std::string text = "Press 'C-q' to close the menu.";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, id.c_str());
    start += id.size() + 1;
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else if (mode == "status") {
    std::string name = "CHANGE STATUS";
    std::string text = "Press 'q' to exit.";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else if (mode == "help") {
    std::string name = "HELP MENU";
    std::string text = "Do you really need me to tell you how to exit?";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else if (mode == "task-filter") {
    std::string name = "FILTER";
    std::string x = "TASKS";
    std::string text = "Press 'ENTER' to get results.";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, x.c_str());
    start += x.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, filter.c_str());
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else if (mode == "category-filter") {
    std::string name = "FILTER";
    std::string x = "CATEGORY";
    std::string text = "Press 'ENTER' to get results.";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, x.c_str());
    start += x.size() + 1;
    mvwaddch(win, y, start, separator);
    start += 2;
    mvwaddstr(win, y, start, filter.c_str());
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  } else {
    std::string name = "SHIT";
    std::string text = "How are you even reading this?";
    wattron(win, A_BOLD);
    mvwaddstr(win, y, start, name.c_str());
    start += name.size() + 1;
    mvwaddstr(win, y, getmaxx(win) - text.length() - 1, text.c_str());
    wattroff(win, A_BOLD);
  }

  wattroff(win, A_REVERSE);
  update_panels();
  doupdate();
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
      "r --> Remove all filters",
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

/* ============== TAKS FUNCTIONS ============== */

bool containsString(std::string_view toCheck, std::string_view contained) {
  if (toCheck.size() < contained.size())
    return false;

  return toCheck.find(contained) != std::string_view::npos;
}

void createSubVector(std::vector<Tasks *> &tasks,
                     std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  tasks.clear();
  for (int i = 0; i < mainTasks.size(); ++i) {
    bool insert{true};

    if (toggleComplete_unique)
      insert = mainTasks.at(i)->status == 2 ? false : true;

    if (!category_filter.empty())
      if (insert)
        insert = containsString(mainTasks.at(i)->category, category_filter);

    if (!task_filter.empty()) {
      if (insert) {
        insert = containsString(mainTasks.at(i)->task, task_filter);
      }
    }

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

  if (current_index_unique > res.size())
    current_index_unique = res.size() - 1;
  return res;
}

/* ============== MAIN FUNCTIONS ============== */

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
    displayTaskDetails(windows[TASKDETAIL], tasks);
    setStatusMenu(windows[STATUSBAR], panels[STATUSBAR], "main");

    update_panels();
    doupdate();

    input = wgetch(windows[0]);

    handleInput(input, windows, panels, filteredTasks,
                mainTasks_for_subvector_and_inputHandler_only, start, end,
                getmaxy(windows[TASKLIST]) - 3);

    update_panels();
    doupdate();
  }
}

void displayStart(std::vector<std::unique_ptr<Tasks>> &mainTasks) {
  // setting up the windows for the panels library
  WINDOW *wins[6];
  PANEL *panels[6];

  wins[0] = newwin(LINES * .70, COLS, 0, 0);
  wins[1] = newwin(LINES * .30, COLS, (LINES * .70), 0);
  wins[2] = newwin(LINES * .08, COLS * .90, LINES * .08, COLS * .05);
  wins[3] = newwin(LINES * .10, COLS * .30, LINES * .30, COLS * .35);
  wins[4] = newwin(LINES * .80, COLS * .80, LINES * .10, COLS * .10);
  wins[5] = newwin(1, COLS, LINES - 1, 0);

  for (auto win : wins)
    box(win, 0, 0);

  panels[0] = new_panel(wins[0]);
  panels[1] = new_panel(wins[1]);
  panels[2] = new_panel(wins[2]);
  panels[3] = new_panel(wins[3]);
  panels[4] = new_panel(wins[4]);
  panels[5] = new_panel(wins[5]);

  hide_panel(panels[ASKMENU]);
  hide_panel(panels[SELECTIONMENU]);
  hide_panel(panels[HELPMENU]);
  setStatusMenu(wins[STATUSBAR], panels[STATUSBAR], "main");

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
