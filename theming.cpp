#include "header_files/theming.h"
#include <ncurses.h>

int scaleRGB(int val) { return (val * 1000) / 255; }

void importColors() {
  init_color(colours::BLUE, scaleRGB(13), scaleRGB(192), scaleRGB(219));
  init_color(colours::GREEN, scaleRGB(30), scaleRGB(214), scaleRGB(17));
  init_color(colours::STATUS, scaleRGB(54), scaleRGB(49), scaleRGB(107));
  init_color(colours::WHITE, scaleRGB(255), scaleRGB(255), scaleRGB(255));
  init_color(colours::EMERALD, scaleRGB(60), scaleRGB(161), scaleRGB(6));
  init_color(colours::RED, scaleRGB(247), scaleRGB(12), scaleRGB(24));
  init_color(colours::YELLOW, scaleRGB(247), scaleRGB(255), scaleRGB(18));
  init_color(colours::GRAY, scaleRGB(84), scaleRGB(89), scaleRGB(82));
  init_color(colours::GRAY_BLUE, scaleRGB(111), scaleRGB(165), scaleRGB(199));
  init_color(colours::DARK_BLUE, scaleRGB(0), scaleRGB(201), scaleRGB(191));
  init_color(colours::LIGHT_GREY, scaleRGB(47), scaleRGB(49), scaleRGB(48));
  init_color(colours::HIGHLIGHT, scaleRGB(0), scaleRGB(155), scaleRGB(186));
  init_color(colours::LIGHT_BLUE, scaleRGB(179), scaleRGB(242), scaleRGB(255));
}

void createPairs() {
  init_pair(pairs::TASKS_LIST_PAIR, colours::BLUE, -1);
  init_pair(pairs::KEY_PAIR, colours::GREEN, -1);
  init_pair(pairs::VALUE_PAIR, colours::BLUE, -1);
  init_pair(pairs::BORDER_PAIR, colours::BLUE, -1);
  init_pair(pairs::STATUS_BAR, colours::STATUS, colours::WHITE);
  init_pair(pairs::OTHER_MENU, colours::EMERALD, -1);
  init_pair(pairs::INCOMPLETE_PAIR, colours::DARK_BLUE, -1);
  init_pair(pairs::ONGOING_PAIR, colours::GRAY_BLUE, -1);
  init_pair(pairs::COMPLETE_PAIR, colours::GRAY, -1);
  init_pair(pairs::HIGHLIGHT_PAIR, colours::HIGHLIGHT, colours::WHITE);
}
