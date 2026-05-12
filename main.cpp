#include "model.hpp"
#include "tasks.hpp"
#include "ui.hpp"
#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 10

int main() {
  std::vector<Tasks> taskList;

  // making a window for the tasks, we haven't defined the size though, inputch
  // is the character we use to keep track of what the user is pressing,
  // highlight is the task we are on so that we can highlight it
  WINDOW *taskList_win, *taskDetails_win, *helpWin;
  int inputch;
  int highlight{1};

  startup(&taskList);

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
  taskDetails_win = newwin((LINES / 3) - 1, COLS, ((2 * LINES) / 3), 0);
  helpWin = newwin(1, COLS, LINES - 1, 0);
  printHelpWin(helpWin);

  int y, x;
  getmaxyx(taskList_win, y, x);
  // variables to know which tasks to display, we manipulate them and send them
  // to the print task function to show only the tasks that we need
  int start{0};
  int end = taskList.size() > (y - 3) ? (y - 3) : (taskList.size() - 1);

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
      if (highlight == taskList.size())
        break;
      else {
        highlight++;
        if (highlight - 1 >= start + y - 2)
          start++;
      }
      break;

    case 'c':
      toggleComplete(&taskList, highlight);
      overwrite(&taskList);
      break;

    case 'i': {
      std::string task = askTask();
      if (task.empty())
        break;

      createTask(&taskList, task);
      break;
    }

    case 'd': {
      if (!askDelete(highlight))
        break;
      deleteTask(&taskList, highlight);
      if (highlight > taskList.size())
        highlight = taskList.size();
      break;
    }

    case 'q':
      delwin(taskDetails_win);
      delwin(taskList_win);
      delwin(helpWin);
      endwin();
      overwrite(&taskList);
      return 0;
    }
    end =
        (start + y - 3) > taskList.size() ? taskList.size() - 1 : start + y - 3;
    print_taskList(taskList_win, highlight, &taskList, start, end);
    print_task_details(taskDetails_win, highlight - 1, &taskList);
  }
}
