#ifndef TAKSHANDLING_H
#define TAKSHANDLING_H

#include "taskClass.h"
#include <ctime>
#include <memory>
#include <string>
#include <vector>

int insertNewTask(std::string task, std::string category,
                  std::vector<std::unique_ptr<Tasks>> &tasksMain);
int insertTask(int id, std::string task, std::string category, int status,
               bool renewing, time_t created_at, time_t modified_at,
               std::vector<std::unique_ptr<Tasks>> &tasksMain);

int deleteTask(int id, std::vector<std::unique_ptr<Tasks>> &tasksMain);

int editTask(int id_const, std::string task, std::string category, int status,
             bool renewing, std::vector<std::unique_ptr<Tasks>> &tasksMain);

int changeStatus(int id, int staus,
                 std::vector<std::unique_ptr<Tasks>> &tasksMain);

#endif // !TAKSHANDLING_H
