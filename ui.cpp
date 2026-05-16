#include "model.hpp"
#include "tasks.hpp"
#include <array>
#include <iostream>
#include <ncurses.h>

#define HIGHLIGHT_BACKGROUND 24
#define TEXT_BLUE 38
#define STATUS_GREEN 77
#define KEYS_COLOR 34
#define DISPLAY_TEXT 75

#define MAIN_WINDOW 24
#define HIGHLIGHT_PAIR 25
#define STATUS_DOT_PAIR 26
#define STATUS_HIGHLIGHT_PAIR 27
#define KEYS_PAIR 28
#define DISPLAY_TEXT_PAIR 29

#define ID_COLS 6
#define STATUS_COLS 6

// init_color(COLOR_BLACK, 31, 164, 250);

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

inline void printTask(WINDOW *taskList_win, std::string *id,
                      std::string *status, std::string *taskText,
                      int *printIdFrom, int *printStatusFrom,
                      int *printTaskFrom, int *y, int MAIN_COLOR,
                      int STATUS_COLOR) {
  mvwaddstr(taskList_win, *y, *printIdFrom, id->c_str());
  wattroff(taskList_win, COLOR_PAIR(MAIN_COLOR));
  wattron(taskList_win, COLOR_PAIR(STATUS_COLOR));
  mvwaddstr(taskList_win, *y, *printStatusFrom, status->c_str());
  wattroff(taskList_win, COLOR_PAIR(STATUS_COLOR));
  wattron(taskList_win, COLOR_PAIR(MAIN_COLOR));
  mvwaddstr(taskList_win, *y, *printTaskFrom, taskText->c_str());
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

  init_pair(MAIN_WINDOW, TEXT_BLUE, -1);
  init_pair(HIGHLIGHT_PAIR, HIGHLIGHT_BACKGROUND, TEXT_BLUE);
  init_pair(STATUS_DOT_PAIR, STATUS_GREEN, -1);
  init_pair(STATUS_HIGHLIGHT_PAIR, HIGHLIGHT_BACKGROUND, STATUS_GREEN);
  // we erase the previous screen so that the remanants of the screen before
  // that does not stay in the new screen
  werase(taskList_win);
  // makes a border around the window
  wattron(taskList_win, COLOR_PAIR(MAIN_WINDOW));
  box(taskList_win, 0, 0);
  wattroff(taskList_win, COLOR_PAIR(MAIN_WINDOW));
  mvwaddstr(taskList_win, 0, 5, "Task List");

  // if the terminal does not support don't  run the program
  if (!has_colors()) {
    std::cout << "Your terminal does not support colors.\n";
    exit(0);
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
    if (taskText.length() > COLS - 2 - ID_COLS - STATUS_COLS - 6)
      taskText = taskText.substr(0, COLS - 2 - ID_COLS - STATUS_COLS - 6 - 3)
                     .append("...");
    std::string status = task.completed ? "\u25CF" : "\u25CB";

    int printIdFrom = ((1 + ID_COLS) - id.length()) / 2;
    int printStatusFrom = ID_COLS + 1 + (STATUS_COLS - id.length()) / 2;
    int printTaskFrom =
        (1 + ID_COLS + STATUS_COLS) +
        ((COLS - 2 - ID_COLS - STATUS_COLS) - taskText.length()) / 2;

    // reverse the foreground and background color if the task id is the same as
    // the tesk we are currently on
    if (highlight == task.id) {
      wattron(taskList_win, COLOR_PAIR(HIGHLIGHT_PAIR));
      wattron(taskList_win, A_REVERSE);
      mvwhline(taskList_win, y, 1, ' ', maxx - 2);
      printTask(taskList_win, &id, &status, &taskText, &printIdFrom,
                &printStatusFrom, &printTaskFrom, &y, HIGHLIGHT_PAIR,
                STATUS_HIGHLIGHT_PAIR);
      wattroff(taskList_win, A_REVERSE);
      wattroff(taskList_win, COLOR_PAIR(HIGHLIGHT_PAIR));
    } else {

      wattron(taskList_win, COLOR_PAIR(MAIN_WINDOW));
      printTask(taskList_win, &id, &status, &taskText, &printIdFrom,
                &printStatusFrom, &printTaskFrom, &y, MAIN_WINDOW,
                STATUS_DOT_PAIR);
      wattroff(taskList_win, COLOR_PAIR(MAIN_WINDOW));
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

  init_pair(KEYS_PAIR, KEYS_COLOR, -1);
  init_pair(DISPLAY_TEXT_PAIR, DISPLAY_TEXT, -1);
  init_pair(MAIN_WINDOW, TEXT_BLUE, -1);

  const Tasks &task = taskList->at(highlight);

  werase(taskDetails_win);

  wattron(taskDetails_win, COLOR_PAIR(MAIN_WINDOW));
  box(taskDetails_win, 0, 0);
  wattroff(taskDetails_win, COLOR_PAIR(MAIN_WINDOW));

  mvwaddstr(taskDetails_win, 0, 5, "Task Details");
  std::array<std::string, 4> keys = {
      "Id: ", "Task: ", "Status: ", "Created At: "};
  //
  std::array<std::string, 4> displayTexts;
  displayTexts[0] = std::to_string(task.id);
  displayTexts[1] = task.task;
  displayTexts[2] = (task.completed ? "Completed" : "Incomplete");
  // displayTexts[3] = "Repeating: " + task.repeating;
  char buf[24];
  tm c_time = *localtime(&task.created_at);
  strftime(buf, sizeof(buf), "%e %B %Y, %l:%M %p", &c_time);
  displayTexts[3] = (buf);
  // displayTexts[5] = "Complete By: " + task.complete_at;
  //
  for (int i = 0; i < 4; ++i) {
    int max_lenght = COLS - 6 - keys[i].length();
    wattron(taskDetails_win, COLOR_PAIR(KEYS_PAIR));
    mvwaddstr(taskDetails_win, y, x, keys[i].c_str());
    wattroff(taskDetails_win, COLOR_PAIR(KEYS_PAIR));
    wattron(taskDetails_win, COLOR_PAIR(DISPLAY_TEXT_PAIR));
    if (displayTexts[i].length() > max_lenght) {
      std::vector<std::string> longTexts;
      while (displayTexts[i].length() > max_lenght) {
        longTexts.push_back(displayTexts[i].substr(0, max_lenght));
        displayTexts[i] =
            displayTexts[i].substr(max_lenght, displayTexts[i].length());
      }
      for (std::string text : longTexts) {
        mvwaddstr(taskDetails_win, y, x + keys[i].length(), text.c_str());
        y++;
      }
    } else
      mvwaddstr(taskDetails_win, y, x + keys[i].length(),
                displayTexts[i].c_str());
    mvwaddstr(taskDetails_win, y, x + keys[i].length(),
              displayTexts[i].c_str());
    wattroff(taskDetails_win, COLOR_PAIR(DISPLAY_TEXT_PAIR));
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
