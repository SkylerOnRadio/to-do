#ifndef TASKS_HPP
#define TASKS_HPP

#include <string>

class Tasks {
public:
  std::string task;
  bool completed{false};
  int id;
  std::string category;
  time_t created_at;
  time_t modified_at;

  Tasks(std::string task, int id, std::string category = "None");
  Tasks(std::string task, bool complete, int id, time_t created_at,
        time_t modified_at, std::string category = "None");
};

#endif
