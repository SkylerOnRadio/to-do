#include "header_files/fileHandling.h"
#include "header_files/taskClass.h"
#include <fstream>
#include <string>

int loadFile(std::string filename,
             std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  std::ifstream file{filename};
  if (!file.is_open())
    return -1;

  std::string line;
  while (std::getline(file, line)) {
  }
}
