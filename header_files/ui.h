#ifndef UI_H
#define UI_H
#include "taskClass.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

void displayStart(std::vector<std::unique_ptr<Tasks>> &mainTasks);

std::string askMenu(WINDOW *win, PANEL *panel, std::string text);
std::string selectMenu(WINDOW *win, PANEL *panel,
                       std::vector<std::string> options, std::string text);

#endif // !UI_H
