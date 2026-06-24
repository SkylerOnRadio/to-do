#include "header_files/taskClass.h"
#include "header_files/tasksHandling.h"
#include <ctime>
#include <memory>
#include <string>
#include <vector>

int bsearch(std::vector<std::unique_ptr<Tasks>> &tasks, int target) {
  int low = 0;
  int high = tasks.size();
  while (high >= low) {
    int mid = (high + low) / 2;
    if (tasks.at(mid)->id == target)
      return mid;
    if (tasks.at(mid)->id > target)
      high = mid - 1;
    if (tasks.at(mid)->id < target)
      low = mid + 1;
  }
  return -1;
}

int insertNewTask(std::string task, std::string category,
                  std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  int id = tasksMain.empty() ? 1 : tasksMain.back()->id + 1;

  tasksMain.push_back(std::make_unique<Tasks>(Tasks(id, task, category)));

  return 0;
}

int insertTask(int id, std::string task, std::string category, int status,
               bool renewing, time_t created_at, time_t modified_at,
               std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  if (status > 2 || status < 0)
    return -1;
  if (task == "")
    return -1;

  tasksMain.push_back(std::make_unique<Tasks>(
      Tasks(id, task, status, renewing, created_at, modified_at, category)));

  return 0;
}

int deleteTask(int id, std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  if (id > tasksMain.back()->id)
    return -1;

  int index = bsearch(tasksMain, id);
  if (index == -1)
    return -1;

  tasksMain.erase(tasksMain.begin() + index);
  return 0;
}

int editTask(int id_const, std::string task, std::string category, int status,
             bool renewing, std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  if (id_const > tasksMain.back()->id)
    return -1;

  int index = bsearch(tasksMain, id_const);
  if (index == -1)
    return -1;

  tasksMain.at(index)->editTask(task, category, status, renewing);
  return 0;
}

int changeStatus(int id, int status,
                 std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  if (id > tasksMain.back()->id)
    return -1;

  int index = bsearch(tasksMain, id);
  if (index == -1)
    return -1;

  tasksMain.at(index)->status = status;
  return 0;
}
