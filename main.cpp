#include <cstring>
#include <iostream>
#include <ncurses.h>

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
  if (argc > 1)
    parseArguments(argv, argc);

  initscr();
  WINDOW *mainWin;
  mainWin = newwin(LINES, COLS, 0, 0);
  raw();
  curs_set(0);
  box(mainWin, 0, 0);

  wrefresh(mainWin);

  delwin(mainWin);
  endwin();

  return 0;
}
