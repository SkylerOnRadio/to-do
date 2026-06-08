#include "tasks.hpp"
#include <ctime>
#include <string>

Tasks::Tasks(std::string task, int id, std::string category) {
  // time(nullptr) gets the current Linux TIme
  this->created_at = time(nullptr);
  this->modified_at = time(nullptr);
  this->task = task;
  this->id = id;
  this->category = category;
}

Tasks::Tasks(std::string task, bool completed, int id, time_t created_at,
             time_t modified_at, std::string category) {
  this->task = task;
  this->created_at = created_at;
  this->modified_at = modified_at;
  this->completed = completed;
  this->id = id;
  this->category = category;
}
