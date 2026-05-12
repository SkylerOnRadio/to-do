#include "model.hpp"
#include "tasks.hpp"
#include <array>
#include <iostream>
#include <ncurses.h>

void printHelpWin(WINDOW *helpWin) {
  int maxy, maxx;
  getmaxyx(helpWin, maxy, maxx);

  const int keybindNo = 6;
  int offset = maxx / keybindNo;
  int x{0}, y{0};

  std::array<std::string, keybindNo> keybinds;
  keybinds[0] = "k: up";
  keybinds[1] = "j: down";
  keybinds[2] = "q: quit";
  keybinds[3] = "i: insert";
  keybinds[4] = "c: toggle complete";
  keybinds[5] = "d: delete";

  wattron(helpWin, A_REVERSE);
  mvwhline(helpWin, y, 0, ' ', maxx);
  for (std::string text : keybinds) {
    mvwaddstr(helpWin, y, x + (((maxx / keybindNo) - text.length()) / 2),
              text.c_str());
    x += offset;
  }
  wattroff(helpWin, A_REVERSE);

  wrefresh(helpWin);
};

inline void menuPrint(int colsPartion, int additionalPadding, std::string text,
                      WINDOW *taskList_win, int y) {
  int length = text.length();

  int printFrom = (colsPartion - length) / 2;

  mvwaddstr(taskList_win, y, additionalPadding + printFrom, text.c_str());
}

// Function that prints all the tasks that are there
void print_taskList(WINDOW *taskList_win, int highlight,
                    std::vector<Tasks> *taskList, int startIndex,
                    int endIndex) {

  // Printing each task takes the x,y position to print at, x is 2 to be
  // moderately away from the border and y is 1 becuse the border starts at 0
  int maxy, maxx;
  getmaxyx(taskList_win, maxy, maxx);
  int colsPartition, y;
  colsPartition = (maxx - 2) / 3;
  y = 1;

  // we erase the previous screen so that the remanants of the screen before
  // that does not stay in the new screen
  werase(taskList_win);
  // makes a border around the window
  box(taskList_win, 0, 0);
  mvwaddstr(taskList_win, 0, 5, "Task List");

  // if the terminal does not support don't  run the program
  if (!has_colors()) {
    std::cout << "Your terminal does not support colors.\n";
  }

  // loop through the vector, I didn't use a for each loop(check it out if you
  // don't know what it is, it loops kinda like python) because i want to only
  // show a certain amount of tasks that the screen can display
  for (int i = startIndex; i <= endIndex; i++) {

    // const means read only, and & is a reference so that the variable is
    // assigned the same memory location as the original task we are pointing to
    // using at(), saves memory
    const Tasks &task = taskList->at(i);
    std::string id = std::to_string(task.id);
    std::string taskText = task.task;
    std::string status = task.completed ? "Completed" : "Incomplete";

    // reverse the foreground and background color if the task id is the same as
    // the tesk we are currently on
    if (highlight == task.id) {
      wattron(taskList_win, A_REVERSE);
      mvwhline(taskList_win, y, 1, ' ', maxx - 2);
      menuPrint(colsPartition, 0, id, taskList_win, y);
      menuPrint(colsPartition, colsPartition, taskText, taskList_win, y);
      menuPrint(colsPartition, 2 * colsPartition, status, taskList_win, y);
      wattroff(taskList_win, A_REVERSE);
    } else {
      menuPrint(colsPartition, 0, id, taskList_win, y);
      menuPrint(colsPartition, colsPartition, taskText, taskList_win, y);
      menuPrint(colsPartition, 2 * colsPartition, status, taskList_win, y);
    }

    // increment y so that the next iteration prints on the next line
    y++;
  }
  // display the tasks finally, wouldn't wanna do all this and not displat it :p
  wrefresh(taskList_win);
}

void print_task_details(WINDOW *taskDetails_win, int highlight,
                        std::vector<Tasks> *taskList) {
  int x, y;
  x = 2;
  y = 1;
  const Tasks &task = taskList->at(highlight);

  werase(taskDetails_win);
  box(taskDetails_win, 0, 0);
  mvwaddstr(taskDetails_win, 0, 5, "Task Details");
  //
  std::array<std::string, 4> displayTexts;
  displayTexts[0] = "Id: " + std::to_string(task.id);
  displayTexts[1] = "Task: " + task.task;
  displayTexts[2] = "Status: ";
  displayTexts[2].append(task.completed ? "Completed" : "Incomplete");
  // displayTexts[3] = "Repeating: " + task.repeating;
  char buf[20];
  tm c_time = *localtime(&task.created_at);
  strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &c_time);
  displayTexts[3] = "Created At: ";
  displayTexts[3].append(buf);
  // displayTexts[5] = "Complete By: " + task.complete_at;
  //
  for (std::string text : displayTexts) {
    mvwaddstr(taskDetails_win, y, x, text.c_str());
    y++;
  }

  wrefresh(taskDetails_win);
};

std::string askTask() {
  WINDOW *askWindow;
  std::string task = "";
  std::string infoText = "Enter the task to be inserted";
  int y{1}, x{2};
  raw();

  askWindow = newwin(3, COLS * .8, LINES / 10, COLS - (COLS * .9));

  werase(askWindow);
  box(askWindow, 0, 0);
  mvwaddstr(askWindow, 0, 5, infoText.c_str());
  wrefresh(askWindow);

  int ch = wgetch(askWindow);
  curs_set(1);
  wmove(askWindow, y, x);
  keypad(askWindow, false);

  while (ch != '\n') {
    if (ch == 127 || ch == 8) {
      if (!task.empty())
        task.pop_back();
    } else if (ch == 17) {
      task = "";
      break;
    } else
      task.push_back(ch);

    werase(askWindow);
    box(askWindow, 0, 0);
    mvwaddstr(askWindow, 0, 5, infoText.c_str());
    mvwaddstr(askWindow, y, x, task.c_str());
    wmove(askWindow, y, x + task.size());
    wrefresh(askWindow);

    ch = wgetch(askWindow);
  }

  wrefresh(askWindow);
  delwin(askWindow);
  curs_set(0);
  noraw();
  cbreak();
  return task;
}

bool askDelete(int highlight) {
  WINDOW *askDeleteWin;
  bool confirm{false};
  int ch;
  std::string infoText = "Delete task " + std::to_string(highlight) + "?";
  std::string confirmText = "Yes: y";
  std::string denyText = "No: n";
  int maxy, maxx;

  askDeleteWin = newwin(3, COLS * 0.2, LINES / 10, (COLS - (COLS * .2)) / 2);
  getmaxyx(askDeleteWin, maxy, maxx);

  werase(askDeleteWin);
  box(askDeleteWin, 0, 0);
  mvwaddstr(askDeleteWin, 0, 5, infoText.c_str());
  mvwaddstr(askDeleteWin, 1, ((maxx / 2) - confirmText.length()) / 2,
            confirmText.c_str());
  mvwaddstr(askDeleteWin, 1, maxx / 2 + (((maxx / 2) - denyText.length()) / 2),
            denyText.c_str());
  wrefresh(askDeleteWin);

  ch = wgetch(askDeleteWin);

  while (1) {
    if (ch == 'n' || ch == 'c' || ch == 'q')
      break;
    else if (ch == 'y') {
      confirm = true;
      break;
    }
    ch = wgetch(askDeleteWin);
  }

  delwin(askDeleteWin);
  return confirm;
}
