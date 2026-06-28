#ifndef INPUTHANDLING_H
#define INPUTHANDLING_H

#include "./taskClass.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <vector>

void handleInput(int input, WINDOW *win, PANEL *panel,
                 std::vector<std::unique_ptr<Tasks>> &mainTasks);

#endif // !INPUTHANDLING_H
