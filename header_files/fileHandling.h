#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "taskClass.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

int loadFile(std::string filename,
             std::vector<std::unique_ptr<Tasks>> &tasksMain);

int saveToFile(std::string filename,
               std::vector<std::unique_ptr<Tasks>> &tasksMain);
int updatePreviousVersionFile(std::string filename);

#endif // !FILEHANDLING_H
