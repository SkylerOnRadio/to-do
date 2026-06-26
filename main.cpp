#include "header_files/fileHandling.h"
#include "header_files/taskClass.h"
#include "header_files/ui.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

void parseArguments(char *arguments[], int argCount) {
  for (int i = 1; i < argCount; ++i) {
    if (strcmp(arguments[i], "-h") == 0 ||
        strcmp(arguments[i], "--help") == 0) {
      std::cout
          << "To-do manager to keep track of your to-dos and help "
             "you organise them.\n"
          << "Current flags are: \n"
          << "--updateFile  : updates the task.csv file that the previous "
             "version uses to make it compatible with the new version.\n";
      exit(0);
    } else {
      std::cout
          << "Invalid flag. Use -h or --help to find flags and their uses.\n";
    }
  }
}

int main(int argc, char *argv[]) {
  // checking if arguments are passed, if yes let the argument parser handle
  // them
  if (argc > 1)
    parseArguments(argv, argc);

  // checking if HOME directory exits
  const char *homeDir = getenv("HOME");
  if (!homeDir) {
    std::cerr
        << "There is no HOME directory in this machine, either there is "
           "something very very wrong with your machine or you are not using "
           "Linux, or something I didn't think of(Let me know in that case).\n";
    exit(EXIT_FAILURE);
  }
  std::string filename = std::string(homeDir) + "/.tasks.csv";

  std::vector<std::unique_ptr<Tasks>> tasks;

  loadFile(filename, tasks);

  initscr();
  WINDOW *mainWin;
  mainWin = newwin(LINES, COLS, 0, 0);
  raw();
  curs_set(0);
  box(mainWin, 0, 0);

  wrefresh(mainWin);
  displayStart(mainWin, tasks);

  delwin(mainWin);
  endwin();

  return 0;
}
