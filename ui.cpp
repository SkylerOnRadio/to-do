#include <array>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#define WIDTH 30
#define HEIGHT 10

// Class to store the tasks
class Tasks {
public:
  std::string task, created_at, modified_at, complete_at, complete, repeating;
  int id;
  Tasks(int id, std::string task, std::string created_at,
        std::string modified_at, std::string complete_at, std::string complete,
        std::string repeating) {
    this->id = id;
    this->task = task;
    this->created_at = created_at;
    this->modified_at = modified_at;
    this->complete_at = complete_at;
    this->complete = complete;
    this->repeating = repeating;
  }
};

void print_task_details(WINDOW *taskDetails_win, int highlight,
                        std::vector<Tasks> *taskList) {
  int x, y;
  x = 2;
  y = 1;
  const Tasks &task = taskList->at(highlight);

  werase(taskDetails_win);
  box(taskDetails_win, 0, 0);

  std::array<std::string, 6> displayTexts;
  displayTexts[0] = "Id: " + std::to_string(task.id);
  displayTexts[1] = "Task: " + task.task;
  displayTexts[2] = "Status: " + task.complete;
  displayTexts[3] = "Repeating: " + task.repeating;
  displayTexts[4] = "Created At: " + task.created_at;
  displayTexts[5] = "Complete By: " + task.complete_at;

  for (std::string text : displayTexts) {
    mvwaddstr(taskDetails_win, y, x, text.c_str());
    y++;
  }

  wrefresh(taskDetails_win);
};

// Function that prints all the tasks that are there
void print_taskList(WINDOW *taskList_win, int highlight,
                    std::vector<Tasks> *taskList, int startIndex,
                    int endIndex) {

  // Printing each task takes the x,y position to print at, x is 2 to be
  // moderately away from the border and y is 1 becuse the border starts at 0
  int x, y;
  x = 2;
  y = 1;

  // we erase the previous screen so that the remanants of the screen before
  // that does not stay in the new screen
  werase(taskList_win);
  // makes a border around the window
  box(taskList_win, 0, 0);

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
    std::string text = task.task + '\t' + std::to_string(task.id);

    // reverse the foreground and background color if the task id is the same as
    // the tesk we are currently on
    if (highlight == task.id) {
      wattron(taskList_win, A_REVERSE);
      mvwaddstr(taskList_win, y, x, text.c_str());
      wattroff(taskList_win, A_REVERSE);
    } else
      mvwaddstr(taskList_win, y, x, text.c_str());

    // increment y so that the next iteration prints on the next line
    y++;
  }
  // display the tasks finally, wouldn't wanna do all this and not displat it :p
  wrefresh(taskList_win);
}

int main() {
  // making a window for the tasks, we haven't defined the size though, inputch
  // is the character we use to keep track of what the user is pressing,
  // highlight is the task we are on so that we can highlight it
  WINDOW *taskList_win, *taskDetails_win;
  int inputch;
  int highlight{1};

  // vector to strore the tasks, for now using hardcoded data, once the basic
  // structure for the ui is done then the file reading will be done
  std::vector<Tasks> taskList;
  for (int i = 1; i <= 50; ++i) {
    taskList.emplace_back(
        i,
        "Task Number " + std::to_string(i), // Unique Description
        "2026-05-" + std::to_string(i),     // Unique Start Date
        "2026-05-11", "2026-05-11", "True",
        "Category_" + std::to_string(i % 5) // Cycles through 5 categories
    );
  }
  initscr(); // initialise the curses mode
  clear();   // i forgot what this did tbh
  noecho();  // makes it so that the characters that the user is pressing is not
             // shown by default, we don't want to show thw user what they are
             // pressing for no reason
  cbreak();  // don't have to press enter after every character input
  curs_set(0); // remove the cursor, its ugly when we see tssks

  // start x is 0 to start at the beginning and y is at 2 becuse we are printing
  // some lines before the windows, only for now though

  // initializing the window fot the windows to display the tasks, its 2/3rd of
  // the screen both verticcally and horizontally
  taskList_win = newwin((2 * LINES) / 3, COLS, 0, 0);
  taskDetails_win = newwin((LINES / 3), COLS, ((2 * LINES) / 3), 0);

  int y, x;
  getmaxyx(taskList_win, y, x);
  // variables to know which tasks to display, we manipulate them and send them
  // to the print task function to show only the tasks that we need
  int start{0};
  int end{y - 3};

  refresh();

  // print all the task
  print_taskList(taskList_win, highlight, &taskList, start, end);
  print_task_details(taskDetails_win, highlight - 1, &taskList);

  // makes the ketboard accept arrow and function button
  keypad(taskList_win, TRUE);

  // loop till the user pressing exit key
  while (1) {
    inputch = wgetch(taskList_win);
    switch (inputch) {

      // k is for going up, if the user is at the top we dont do anything, the
      // logic for going up is wonky and idk how I got it work, i'll change it
      // later to be more consistent
    case 'k':
      if (highlight == 1)
        break;
      else {
        // TODO: change the up logic, make it go up if you are at the 3rd task
        // from the top and then stop at task 1
        highlight--;
        if (highlight - start < 3 && start != 0)
          start--;
      }
      break;

      // j is for going downm, ig the user is at at the last task, then stop
      // scrolling, else if the user is at highlight greater than the starting
      // task nu + the max tasks that we can show then scroll
    case 'j':
      if (highlight == 50)
        break;
      else {
        highlight++;
        if (highlight - 1 >= start + y - 2)
          start++;
      }
      break;

    case 'q':
      endwin();
      return 0;
    }
    end = start + y - 3;
    print_taskList(taskList_win, highlight, &taskList, start, end);
    print_task_details(taskDetails_win, highlight - 1, &taskList);
  }
}
