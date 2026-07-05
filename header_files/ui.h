#ifndef UI_H
#define UI_H
#include "taskClass.h"
#include <memory>
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <vector>

void displayStart(std::vector<std::unique_ptr<Tasks>> &mainTasks,
                  bool firstTime);

std::string askMenu(WINDOW *win, PANEL *panel, std::string text);
std::string selectMenu(WINDOW *win, PANEL *panel,
                       std::vector<std::string> options, std::string text);

void helpMenu(WINDOW *win, PANEL *panel);
void setStatusMenu(WINDOW *win, PANEL *panel, std::string_view mode,
                   std::string filter = "");

void createSubVector(std::vector<Tasks *> &tasks,
                     std::vector<std::unique_ptr<Tasks>> &mainTasks);
std::vector<Tasks *> createDisplaySubvector(std::vector<Tasks *> &filteredTasks,
                                            WINDOW *displayWin, int start);
void displayTasks(WINDOW *win, std::vector<Tasks *> &tasks);
void displayTaskDetails(WINDOW *win, std::vector<Tasks *> &tasks);

void resizeWindows(WINDOW *wins[], PANEL *panels[]);

#endif // !UI_H
