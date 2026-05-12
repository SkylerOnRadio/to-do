#include "tasks.hpp"
#include <ctime>

Tasks::Tasks(std::string task, int id) {
  // time(nullptr) gets the current Linux TIme
  this->created_at = time(nullptr);
  this->modified_at = time(nullptr);
  this->task = task;
  this->id = id;
}

Tasks::Tasks(std::string task, bool completed, int id, time_t created_at,
             time_t modified_at) {
  this->task = task;
  this->created_at = created_at;
  this->modified_at = modified_at;
  this->completed = completed;
  this->id = id;
}
