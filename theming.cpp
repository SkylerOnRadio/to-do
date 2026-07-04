#include "header_files/theming.h"
#include <ncurses.h>

int scaleRGB(int val) { return (val * 1000) / 255; }

void importColors() {
  init_color(colours::BLUE, scaleRGB(13), scaleRGB(192), scaleRGB(219));
  init_color(colours::GREEN, scaleRGB(30), scaleRGB(214), scaleRGB(17));
  init_color(colours::SKY, scaleRGB(16), scaleRGB(124), scaleRGB(143));
  init_color(colours::WHITE, scaleRGB(255), scaleRGB(255), scaleRGB(255));
  init_color(colours::EMERALD, scaleRGB(60), scaleRGB(161), scaleRGB(6));
}

void createPairs() {
  init_pair(pairs::TASKS_LIST_PAIR, colours::BLUE, -1);
  init_pair(pairs::KEY_PAIR, colours::GREEN, -1);
  init_pair(pairs::VALUE_PAIR, colours::BLUE, -1);
  init_pair(pairs::BORDER_PAIR, colours::BLUE, -1);
  init_pair(pairs::STATUS_BAR, colours::SKY, colours::WHITE);
  init_pair(pairs::OTHER_MENU, colours::EMERALD, -1);
}
