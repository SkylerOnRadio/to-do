#ifndef GLOBAL_H
#define GLOBAL_H

#include "taskClass.h"
#include <string>
#include <vector>
extern int start_index_unique;
extern int current_index_unique;
extern int last_index_unique;
extern int current_id_unique;
extern std::string category_filter;
extern bool updateTasks_unique;
extern bool toggleComplete_unique;
extern Tasks *startTask_unique;
extern Tasks *lastTask_unique;
extern bool exit_unique;

#endif // !GLOBAL_H
