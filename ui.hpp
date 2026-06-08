#ifndef UI_HPP
#define UI_HPP

#include "tasks.hpp"
#include <ncurses.h>
#include <vector>

void printHelpWin(WINDOW *helpWin);
void print_taskList(WINDOW *taskList_win, int highlight,
                    std::vector<Tasks> *taskList, int startIndex, int endIndex);
void print_task_details(WINDOW *taskDetails_win, int highlight,
                        std::vector<Tasks> *taskList);
void askMenu(std::string &target, std::string infoText);
bool askDelete(int highlight);

#endif // !DEBUG
