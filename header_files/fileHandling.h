#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "taskClass.h"
#include <memory>
#include <string_view>
#include <vector>

int loadFile(std::string filename,
             std::vector<std::unique_ptr<Tasks>> &tasksMain);

#endif // !FILEHANDLING_H
