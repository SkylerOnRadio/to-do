#include "header_files/taskClass.h"
#include <ctime>
#include <string>

int Tasks::feilds = 7;

Tasks::Tasks(int id, std::string task, std::string category) {
  this->id = id;
  this->task = task;
  this->category = category;
  this->status = 0;
  this->renewing = false;
  this->created_at = time(nullptr);
  this->modified_at = time(nullptr);
}

Tasks::Tasks(int id, std::string task, int status, bool renewing,
             time_t created_at, time_t modified_at, std::string category) {
  this->id = id;
  this->task = task;
  this->category = category;
  this->status = status;
  this->renewing = renewing;
  this->created_at = created_at;
  this->modified_at = modified_at;
}

void Tasks::editTask(std::string task, std::string category, int status,
                     bool renewing, time_t modified_at) {
  this->task = task;
  this->category = category;
  this->status = status;
  this->renewing = renewing;
  this->modified_at = modified_at;
}
