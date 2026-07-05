#include "header_files/fileHandling.h"
#include "header_files/global.h"
#include "header_files/taskClass.h"
#include "header_files/theming.h"
#include "header_files/ui.h"
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <locale.h>
#include <memory>
#include <ncurses.h>
#include <string>
#include <string_view>
#include <vector>

// #define DEBUG

void initWinSizePos() {
  winSize = {
      {static_cast<int>(LINES * .70), COLS},
      {static_cast<int>(LINES * .30), COLS},
      {static_cast<int>(LINES * .08), static_cast<int>(COLS * .90)},
      {static_cast<int>(LINES * .10), static_cast<int>(COLS * .30)},
      {static_cast<int>(LINES * .80), static_cast<int>(COLS * .80)},
      {1, COLS},
      {LINES, COLS},
  };

  winPos = {
      {0, 0},
      {static_cast<int>(LINES * .70), 0},
      {static_cast<int>(LINES * .08), static_cast<int>(COLS * .05)},
      {static_cast<int>(LINES * .30), static_cast<int>(COLS * .35)},
      {static_cast<int>(LINES * .10), static_cast<int>(COLS * .10)},
      {LINES - 1, 0},
      {0, 0},
  };
};

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

bool isFirstTime(std::string &filename) {
  if (std::filesystem::exists(filename))
    return false;
  return true;
}

int main(int argc, char *argv[]) {
  std::string filename{""};
  bool firstTime{true};

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

  firstTime = isFirstTime(filename);
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

  setlocale(LC_ALL, "");
  initscr();

  if (has_colors()) {
    start_color();
    use_default_colors();
    if (can_change_color()) {
      importColors();
      createPairs();
    } else
      std::cout << "Your terminal cannot change colours, the colours will "
                   "not look like they are supposed to!\n";
  } else {
    std::cout << "Your terminal has no colors!\n";
  }

  noecho();
  raw();
  curs_set(0);

  initWinSizePos();

  displayStart(tasks, firstTime);

  saveToFile(filename, tasks);

  endwin();

  return 0;
}
