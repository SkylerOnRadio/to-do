#ifndef INPUTHANDLING_H
#define INPUTHANDLING_H

#include "./taskClass.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <vector>

void handleInput(int input, WINDOW *askWin, PANEL *askPanel, WINDOW *menuWin,
                 PANEL *menuPanel, std::vector<Tasks *> &displayTasks,
                 std::vector<std::unique_ptr<Tasks>> &mainTasks, int start,
                 int end, int maxy);

#endif // !INPUTHANDLING_H
