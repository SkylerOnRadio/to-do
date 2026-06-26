#ifndef UI_H
#define UI_H
#include "taskClass.h"
#include <memory>
#include <ncurses.h>
#include <vector>

void displayStart(WINDOW *win, std::vector<std::unique_ptr<Tasks>> &mainTasks);

#endif // !UI_H
