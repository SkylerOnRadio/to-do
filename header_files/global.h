#ifndef GLOBAL_H
#define GLOBAL_H

#include "taskClass.h"
#include <string>
#include <vector>
extern int start_index_unique;
extern int current_index_unique;
extern std::vector<std::string> activeFilters_unique;
extern bool updateTasks_unique;
extern Tasks *startTask_unique;
extern Tasks *lastTask_unique;
extern bool exit_unique;

#endif // !GLOBAL_H
