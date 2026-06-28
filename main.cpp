#include "header_files/fileHandling.h"
#include "header_files/taskClass.h"
#include "header_files/ui.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <string_view>
#include <vector>

#define DEBUG

void parseArguments(char *arguments[], int argCount,
                    std::string_view filename) {
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
    } else if (strcmp(arguments[i], "--updateFile") == 0) {
      updatePreviousVersionFile(static_cast<std::string>(filename));
      exit(EXIT_SUCCESS);
    } else {
      std::cout
          << "Invalid flag. Use -h or --help to find flags and their uses.\n";
    }
  }
}

int main(int argc, char *argv[]) {
  std::string filename{""};
#ifndef DEBUG
  // checking if HOME directory exits
  const char *homeDir = getenv("HOME");
  if (!homeDir) {
    std::cerr
        << "There is no HOME directory in this machine, either there is "
           "something very very wrong with your machine or you are not using "
           "Linux, or something I didn't think of(Let me know in that case).\n";
    exit(EXIT_FAILURE);
  }
  filename = std::string(homeDir) + "/.tasks.csv";
#endif

#ifdef DEBUG
  filename = "test.csv";
#endif

  // checking if arguments are passed, if yes let the argument parser handle
  // them
  if (argc > 1)
    parseArguments(argv, argc, filename);

  std::vector<std::unique_ptr<Tasks>> tasks;

  loadFile(filename, tasks);

  initscr();
  noecho();
  raw();
  curs_set(0);

  displayStart(tasks);

  saveToFile(filename, tasks);

  endwin();

  return 0;
}
