#include "header_files/theming.h"
#include <ncurses.h>

int scaleRGB(int val) { return (val * 1000) / 255; }

void importColors() {
  init_color(colours::BLUE, scaleRGB(13), scaleRGB(192), scaleRGB(219));
  init_color(colours::GREEN, scaleRGB(8), scaleRGB(7), scaleRGB(48));
}

void createPairs() { init_pair(pairs::TASKS_LIST_PAIR, colours::BLUE, -1); }
