#include <iostream>
#include <string>
#include <vector>

class Tasks {
public:
  std::string task, created_at, modified_at, complete_at, complete, repeating;
  int id;
  Tasks(int id, std::string task, std::string created_at,
        std::string modified_at, std::string complete_at, std::string complete,
        std::string repeating) {
    this->id = id;
    this->task = task;
    this->created_at = created_at;
    this->modified_at = modified_at;
    this->complete_at = complete_at;
    this->complete = complete;
    this->repeating = repeating;
  }
};

int main() {
  std::vector<Tasks> taskList;
  for (int i = 1; i <= 50; ++i) {
    taskList.emplace_back(i, "This is a task", "9-5-2016", "9-5-2016",
                          "9-5-2016", "True", "Ues");
  }
  for (Tasks task : taskList) {
    std::cout << task.id << std::endl;
    std::cout << task.task << std::endl;
    std::cout << task.complete_at << std::endl;
    std::cout << task.modified_at << std::endl;
    std::cout << task.created_at << std::endl;
    std::cout << task.complete << std::endl;
    std::cout << task.repeating << std::endl;
  }
}
