#ifndef TASKCLASS_H
#define TASKCLASS_H

#include <ctime>
#include <string>
class Tasks {
public:
  int id;
  std::string task;
  std::string category;
  int status{0};
  bool renewing{false};
  time_t created_at;
  time_t modified_at;

  Tasks(int id, std::string task, std::string category = "None");
  Tasks(int id, std::string task, int status, bool renewing, time_t created_at,
        time_t modified_at, std::string category = "None");
  void editTask(std::string task, std::string category, int status,
                bool renewing);
};

#endif // !TASKCLASS_H
