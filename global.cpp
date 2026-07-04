#include "header_files/global.h"
#include <string>
#include <vector>

int current_index_unique{0};
int current_id_unique;
std::string category_filter{""};
std::string task_filter{""};
bool toggleComplete_unique{false};
bool updateTasks_unique{false};
bool exit_unique{false};
std::vector<std::vector<int>> winSize;
std::vector<std::vector<int>> winPos;
