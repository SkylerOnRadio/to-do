#ifndef GLOBAL_H
#define GLOBAL_H

#include "taskClass.h"
#include <string>
#include <vector>

extern int current_index_unique;
extern int current_id_unique;
extern std::string category_filter;
extern bool updateTasks_unique;
extern bool toggleComplete_unique;
extern bool exit_unique;

enum windowNames {
  TASKLIST,
  TASKDETAIL,
  ASKMENU,
  SELECTIONMENU,
  HELPMENU,
  STATUSBAR
};

#endif // !GLOBAL_H
