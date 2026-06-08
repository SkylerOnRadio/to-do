#ifndef MODEL_HPP
#define MODEL_HPP

#include "tasks.hpp"
#include <ctime>
#include <string>
#include <vector>

std::string parseForCSV(const std::string &text);
void insertToFile(Tasks *newTask);
void addTask(std::vector<Tasks> *taskList, std::string task, bool complete,
             int id, time_t created_at, time_t modified_at,
             std::string category = "None");
void reindex(std::vector<Tasks> *taskList);
void overwrite(std::vector<Tasks> *taskList);
void parseFromCSV(std::vector<Tasks> *taskList);
void startup(std::vector<Tasks> *taskList);

void createTask(std::vector<Tasks> *taskList, std::string task,
                std::string category = "None");
void toggleComplete(std::vector<Tasks> *taskList, int taskNo);
void deleteTask(std::vector<Tasks> *taskList, int taskNo);

#endif // !DEBUG
