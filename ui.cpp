#include "tasks.hpp"
#include <array>
#include <iostream>
#include <ncurses.h>
#include <vector>

// defining the colors that are used in the program
#define HIGHLIGHT_BACKGROUND 24
#define TEXT_BLUE 38
#define STATUS_GREEN 77
#define KEYS_COLOR 34
#define DISPLAY_TEXT 75
#define ASK_WINDOW_COLOR 72

// defining the pair number for the pair of text and background colors
#define MAIN_WINDOW 24
#define HIGHLIGHT_PAIR 25
#define STATUS_DOT_PAIR 26
#define STATUS_HIGHLIGHT_PAIR 27
#define KEYS_PAIR 28
#define DISPLAY_TEXT_PAIR 29
#define ASK_WINDOW_PAIR 30

// These are the space that the id and status will take in the x axis
#define ID_COLS 6
#define STATUS_COLS 6
#define CATEGORY_COLS 15

// Funtion to print the help menu at the bottom of the screen
void printHelpWin(WINDOW *helpWin) {
  int maxy, maxx;
  getmaxyx(helpWin, maxy, maxx);

  const int keybindNo = 6;
  // each keybind is given the same amount of space
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
    // print the keybind and print it after the offset, the offset is increased
    // after every write to account for the previous keybind
    mvwaddstr(helpWin, y, x + (((maxx / keybindNo) - text.length()) / 2),
              text.c_str());
    x += offset;
  }
  wattroff(helpWin, A_REVERSE);

  wrefresh(helpWin);
};

// an inline function to print the task for the Task List window
inline void printTask(WINDOW *taskList_win, std::string *id,
                      std::string *status, std::string *taskText,
                      int *printIdFrom, int *printStatusFrom,
                      int *printTaskFrom, int *y, int MAIN_COLOR,
                      int STATUS_COLOR, std::string *category,
                      int *printCategoryFrom) {
  mvwaddstr(taskList_win, *y, *printIdFrom, id->c_str());

  wattroff(taskList_win, COLOR_PAIR(MAIN_COLOR));
  wattron(taskList_win, COLOR_PAIR(STATUS_COLOR));

  mvwaddstr(taskList_win, *y, *printStatusFrom, status->c_str());

  wattroff(taskList_win, COLOR_PAIR(STATUS_COLOR));
  wattron(taskList_win, COLOR_PAIR(MAIN_COLOR));

  mvwaddstr(taskList_win, *y, *printCategoryFrom, category->c_str());
  mvwaddstr(taskList_win, *y, *printTaskFrom, taskText->c_str());
}

// Function that prints all the tasks that are there
void print_taskList(WINDOW *taskList_win, int highlight,
                    std::vector<Tasks> *taskList, int startIndex,
                    int endIndex) {

  int maxy, maxx;
  getmaxyx(taskList_win, maxy, maxx);
  // set y to one so that printing can start from the y = 1 since y =0 is taken
  // by the border line
  int y{1};

  // initializing the pair with the colors needed
  init_pair(MAIN_WINDOW, TEXT_BLUE, -1);
  init_pair(HIGHLIGHT_PAIR, HIGHLIGHT_BACKGROUND, TEXT_BLUE);
  init_pair(STATUS_DOT_PAIR, STATUS_GREEN, -1);
  init_pair(STATUS_HIGHLIGHT_PAIR, HIGHLIGHT_BACKGROUND, STATUS_GREEN);

  // we erase the previous screen so that the remanants of the screen before
  // that does not stay in the new screen
  werase(taskList_win);
  // makes a border around the window, with the color of the main window
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

    const Tasks &task = taskList->at(i);
    std::string id = std::to_string(task.id);
    std::string taskText = task.task;
    std::string category = task.category;
    // if the task is longer than the space reserved for it then we add ... to
    // the last part and make it the size of the window
    if (taskText.length() >
        COLS - 2 - CATEGORY_COLS - ID_COLS - STATUS_COLS - 6)
      taskText = taskText
                     .substr(0, COLS - 2 - ID_COLS - STATUS_COLS -
                                    CATEGORY_COLS - 6 - 3)
                     .append("...");
    if (category.length() >
        COLS - 2 - ID_COLS - STATUS_COLS - taskText.length())
      category = category.substr(0, COLS - 2 - ID_COLS - STATUS_COLS - 6 - 3)
                     .append("...");
    // the \u things are unicode to point the displayer to what character to be
    // printed
    std::string status = task.completed ? "\u25CF" : "\u25CB";

    // we want to print the status and id in the middle of the 6 space, and the
    // task is printed centered in the remaining space
    int printIdFrom = ((1 + ID_COLS) - id.length()) / 2;
    int printStatusFrom = ID_COLS + 1 + (STATUS_COLS - 1) / 2;
    int printCategoryFrom = (1 + ID_COLS + STATUS_COLS + CATEGORY_COLS) / 2;
    int printTaskFrom = (1 + ID_COLS + STATUS_COLS + CATEGORY_COLS) +
                        ((COLS - 2 - ID_COLS - STATUS_COLS - CATEGORY_COLS) -
                         taskText.length()) /
                            2;

    // reverse the foreground and background color if the task id is the same as
    // the tesk we are currently on
    if (highlight == i) {
      wattron(taskList_win, COLOR_PAIR(HIGHLIGHT_PAIR));
      wattron(taskList_win, A_REVERSE);
      // mvwhline prints in the whole line, then we print over that with the
      // text we want, this makes the whole line be reversed in color and we
      // don't need to figure out where to print the empty spaces for the
      // highlighting and wher to print the actual text
      mvwhline(taskList_win, y, 1, ' ', maxx - 2);
      printTask(taskList_win, &id, &status, &taskText, &printIdFrom,
                &printStatusFrom, &printTaskFrom, &y, HIGHLIGHT_PAIR,
                STATUS_HIGHLIGHT_PAIR, &category, &printCategoryFrom);
      wattroff(taskList_win, A_REVERSE);
      wattroff(taskList_win, COLOR_PAIR(HIGHLIGHT_PAIR));
    } else {

      wattron(taskList_win, COLOR_PAIR(MAIN_WINDOW));
      printTask(taskList_win, &id, &status, &taskText, &printIdFrom,
                &printStatusFrom, &printTaskFrom, &y, MAIN_WINDOW,
                STATUS_DOT_PAIR, &category, &printCategoryFrom);
      wattroff(taskList_win, COLOR_PAIR(MAIN_WINDOW));
    }

    // increment y so that the next iteration prints on the next line
    y++;
  }
  // display the tasks finally, wouldn't wanna do all this and not displat it :p
  wrefresh(taskList_win);
}

// print the details of the task, it doesn't include all the tasks but only what
// the user will actually need
void print_task_details(WINDOW *taskDetails_win, int highlight,
                        std::vector<Tasks> *taskList) {
  int x, y;
  x = 2;
  y = 1;

  // initialize the colors pairs for the keys and for the text
  init_pair(KEYS_PAIR, KEYS_COLOR, -1);
  init_pair(DISPLAY_TEXT_PAIR, DISPLAY_TEXT, -1);
  init_pair(MAIN_WINDOW, TEXT_BLUE, -1);

  // make a read only task from which we can extract the data
  const Tasks &task = taskList->at(highlight);

  werase(taskDetails_win);

  wattron(taskDetails_win, COLOR_PAIR(MAIN_WINDOW));
  box(taskDetails_win, 0, 0);
  wattroff(taskDetails_win, COLOR_PAIR(MAIN_WINDOW));

  mvwaddstr(taskDetails_win, 0, 5, "Task Details");
  // defining an array of the keys, the text to be displayed next to them is
  // also stored in an array in the same order so that we can access them with
  // just one iterator
  std::array<std::string, 5> keys = {
      "Id: ", "Task: ", "Status: ", "Created At: ", "Category: "};
  //
  std::array<std::string, 5> displayTexts;
  displayTexts[0] = std::to_string(task.id);
  displayTexts[1] = task.task;
  displayTexts[2] = (task.completed ? "Completed" : "Incomplete");
  // strft is used to convert the time structure to string
  char buf[24];
  tm c_time = *localtime(&task.created_at);
  strftime(buf, sizeof(buf), "%e %B %Y, %l:%M %p", &c_time);
  displayTexts[3] = (buf);
  displayTexts[4] = task.category;

  for (int i = 0; i < keys.size(); ++i) {
    int max_lenght = COLS - 6 - keys[i].length();

    // display the key
    wattron(taskDetails_win, COLOR_PAIR(KEYS_PAIR));
    mvwaddstr(taskDetails_win, y, x, keys[i].c_str());
    wattroff(taskDetails_win, COLOR_PAIR(KEYS_PAIR));

    wattron(taskDetails_win, COLOR_PAIR(DISPLAY_TEXT_PAIR));
    // since the task can be langer than the screen so this is to make sure only
    // as much as we can print is printed, we check if the text is longer than
    // the space we have reserved for the text, if the text is longer then we
    // split it from 0 to the max space we have for displaying the text and
    // store that in the vector and delelte that segment from the task, then we
    // check the remaining text for the same thing, we repeath this till the
    // length of the text is less or equal to the space we have reserved, then
    // we push the remaining text in the vector and display the vector elemenets
    // line by line
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

// TODO: Make the askTask window such that if the user enters more text than the
// size for it then the text wraps around, also make the window handle arrow,
// function kets like etc.

// window to ask task from the user, to insert a new task
void askMenu(std::string &target, std::string infoText) {
  WINDOW *askWindow;
  int y{1}, x{2};
  raw(); // disable terminal intercepting the keystrokes before the application

  // the space for the window was mostly eyeballed so this could be improved
  askWindow = newwin(3, COLS * .8, LINES / 10, COLS - (COLS * .9));

  init_pair(ASK_WINDOW_PAIR, ASK_WINDOW_COLOR, -1);

  werase(askWindow);
  wattron(askWindow, COLOR_PAIR(ASK_WINDOW_PAIR));
  box(askWindow, 0, 0);
  mvwaddstr(askWindow, 0, 5, infoText.c_str());
  wattroff(askWindow, COLOR_PAIR(ASK_WINDOW_PAIR));
  wrefresh(askWindow);

  curs_set(1); // set the cursor to be visible
  wmove(askWindow, y, x);
  keypad(askWindow,
         false); // don't let the user use function keys and arrow keys, this is
                 // done because I didn't think about the user using arrows to
                 // move around and how to deal if the user presses funtion keys

  // unless the user hits enter keep reading the inputs of the user
  int ch = wgetch(askWindow);
  while (ch != '\n') {
    // 127 and 8 are the possible ASCII number for the backspace key, so we
    // remove the last letter if they press it
    if (ch == 127 || ch == 8) {
      if (!target.empty())
        target.pop_back();
    } else if (ch == 17) { // 17 is the ASCII for 'n' so we set the string to
                           // empty for it
      target = "";
      break;
    } else
      target.push_back(ch);

    // display the character that the user entered
    werase(askWindow);
    wattron(askWindow, COLOR_PAIR(ASK_WINDOW_PAIR));
    box(askWindow, 0, 0);
    mvwaddstr(askWindow, 0, 5, infoText.c_str());
    mvwaddstr(askWindow, y, x, target.c_str());
    wmove(askWindow, y, x + target.size());
    wattroff(askWindow, COLOR_PAIR(ASK_WINDOW_PAIR));
    wrefresh(askWindow);

    ch = wgetch(askWindow);
  }

  wrefresh(askWindow);
  delwin(askWindow);
  curs_set(0);
  noraw();
  cbreak();
}

// window to ask for confimation of deleleting a task
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

  init_pair(ASK_WINDOW_PAIR, ASK_WINDOW_COLOR, -1);

  werase(askDeleteWin);
  wattron(askDeleteWin, COLOR_PAIR(ASK_WINDOW_PAIR));
  box(askDeleteWin, 0, 0);

  // keep the yes and no options equidistant
  mvwaddstr(askDeleteWin, 0, 5, infoText.c_str());
  mvwaddstr(askDeleteWin, 1, ((maxx / 2) - confirmText.length()) / 2,
            confirmText.c_str());
  mvwaddstr(askDeleteWin, 1, maxx / 2 + (((maxx / 2) - denyText.length()) / 2),
            denyText.c_str());
  wattroff(askDeleteWin, COLOR_PAIR(ASK_WINDOW_PAIR));
  wrefresh(askDeleteWin);

  ch = wgetch(askDeleteWin);

  while (1) {
    if (ch == 'n' || ch == 'q')
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
