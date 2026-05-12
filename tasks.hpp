#ifndef TASKS_HPP
#define TASKS_HPP

#include <string>

class Tasks {
public:
  std::string task;
  bool completed{false};
  int id;
  time_t created_at;
  time_t modified_at;

  Tasks(std::string task, int id);
  Tasks(std::string task, bool complete, int id, time_t created_at,
        time_t modified_at);
};

#endif
