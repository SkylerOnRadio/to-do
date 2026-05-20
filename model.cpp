#include "tasks.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Constants needed
std::string version = "v2.2.3";

// Creating a hardcoded filename
std::string fileName;

// ----------------------------------------------------------------------------------------------------------------------------------------------------
// UTILITY FUNCTIONS

void getFilePath() {
  const char *homeDir = getenv("HOME");
  if (!homeDir) {
    std::cerr << "The Home directory does not exist, either there is something "
                 "very wrong with your computer or you are using windows(which "
                 "is almost equivalent tbh), or u are using docker.";
    exit(EXIT_FAILURE);
  }
  fileName = std::string(homeDir) + "/.tasks.csv";
}

int askTaskNumber(std::string message) {
  int taskNo;
  std::cout << message;
  std::cin >> taskNo;
  std::cin.ignore();
  return taskNo;
}

// function that converts a normal string to a string version that is safe to be
// inserted in to CSV format
std::string parseForCSV(const std::string &text) {
  bool addQuotes = false;
  std::string parsedText = "";
  for (char c : text) {
    if (c == '"') {
      parsedText += '"';
      addQuotes = true;
    }

    else if (c == ',' || c == '\n') {
      addQuotes = true;
    }
    parsedText += c;
  }
  if (addQuotes)
    parsedText = '"' + parsedText + '"';
  return parsedText;
}

void insertToFile(Tasks *newTask) { // writing the task to file
  std::fstream file{fileName, file.out | file.app};

  // getting the localtime to enter to the file
  tm c_time = *localtime(&newTask->created_at);
  tm m_time = *localtime(&newTask->modified_at);
  char buf[20];
  // strftime() converts a time variable to a string provided
  strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &c_time);
  std::string createdTime = parseForCSV(buf);
  strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &m_time);
  std::string modifiedTime = parseForCSV(buf);

  // feilds declaration and initialization in order i want to insert
  std::string id = parseForCSV(std::to_string(newTask->id));
  std::string task = parseForCSV(newTask->task);
  std::string completed = newTask->completed ? "True" : "False";

  file << id << "," << task << "," << completed << "," << createdTime << ","
       << modifiedTime << "\n";
}

// Function to create all the tasks loaded from the file into the linked list
void addTask(std::vector<Tasks> *taskList, std::string task, bool complete,
             int id, time_t created_at, time_t modified_at) {
  taskList->emplace_back(task, complete, id, created_at, modified_at);
}

void reindex(std::vector<Tasks> *taskList) {
  int currentTask{1};

  for (Tasks &task : *taskList) {
    task.id = currentTask++;
  }
}

// Fumction to overwrite files after deleting a task
void overwrite(std::vector<Tasks> *taskList) {

  std::fstream file(fileName, file.out);
  for (const Tasks &task : *taskList) {
    tm c_time = *localtime(&task.created_at);
    tm m_time = *localtime(&task.modified_at);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &c_time);
    std::string createdTime = parseForCSV(buf);
    strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &m_time);
    std::string modifiedTime = parseForCSV(buf);

    // feilds declaration and initialization in order i want to insert
    std::string id = parseForCSV(std::to_string(task.id));
    std::string taskText = parseForCSV(task.task);
    std::string completed = task.completed ? "True" : "False";

    file << id << "," << taskText << "," << completed << "," << createdTime
         << "," << modifiedTime << "\n";
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
// STARTUP FUCNTIONS

// TODO: The parser currently has no way to handle empty feilds, need to work on
// that
void parseFromCSV(std::vector<Tasks> *taskList) {
  char letter;
  // quoteCount is to keep track of the quotes found in the text to know if the
  // , encountered is a end of a field or not, fieldNo is used to keep track of
  // the field we are on, since we only need 5 fields for now it can go to a max
  // of 5, it will be made clearer how it works when we reach to its use
  int quoteCount{0};
  int feildNo{1};
  std::string feild = "";

  // variables to store the tasks till they are saved to the linked list
  int id;
  std::string task;
  bool completed;
  time_t created_at;
  time_t modified_at;

  std::fstream file{fileName, file.in};
  if (!file.is_open())
    std::cerr << "File could not be opened, tasks were no initiated!\n";

  while (file.get(letter)) {

    // if we encounter a , or a \n, we check if the quotation count is even,
    // becuase if it is even then we are not inside a quotation block
    if ((letter == ',' || letter == '\n') && quoteCount % 2 == 0) {
      // we check the value of fieldNo to check which field we are parsing
      switch (feildNo) {
      case 1:
        id = stoi(feild);
        break;

      case 2:
        task = feild;
        break;

      case 3:
        completed = (feild == "False") ? false : true;
        break;

      case 4: {

        std::istringstream ss{feild};
        struct tm tmp_time{0};
        ss >> std::get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (ss.fail())
          std::cerr << "Could not parse time!\n";
        else
          created_at = mktime(&tmp_time);
        break;
      }

      case 5: {
        std::istringstream ss{feild};
        struct tm tmp_time{0};
        ss >> std::get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (ss.fail())
          std::cerr << "Could not parse time!\n";
        else
          modified_at = mktime(&tmp_time);
        break;
      }
      }

      // reset field and quoteCount so that the next field is parsed cleanly
      feild = "";
      quoteCount = 0;

      // if the fieldNo is a divisible by 5 then we have reached to the fifth
      // and last field and so the field has to be reset to the first field and
      // we add the task from all the fields that we parsed
      if (feildNo % 5 == 0 || letter == '\n') {
        feildNo = 1;
        time_t currentTime = time(nullptr);
        if (!completed || !((currentTime - modified_at) >= 86400))
          addTask(taskList, task, completed, id, created_at, modified_at);
      } else
        // increment field if we are not at the last field
        feildNo++;
      continue;

    } // if the letter is " and the next character is also " then we dont need
      // to
    // parse it, its a csv format thingy
    else if (letter == '"' && file.peek() == '"') {

      feild += '"';
      file.get();
      continue;
    }
    if (letter == '"') {
      quoteCount++;
      continue;
    }

    feild += letter;
  }
}

// Fucntion to run on startup, will create file if it doesn't exist and will
// call the function to load all tasks from file
void startup(std::vector<Tasks> *taskList) {
  getFilePath();
  std::fstream f{fileName, f.in};
  if (!f.is_open()) {
    std::ofstream create(fileName);
    create.close();
  }
  parseFromCSV(taskList);
  reindex(taskList);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// VECTOR MANIPULATION

// Fucntion for the user to create task and then saves into the file
void createTask(std::vector<Tasks> *taskList, std::string task) {
  int newTaskId = taskList->empty() ? 1 : taskList->back().id + 1;
  taskList->emplace_back(task, newTaskId);

  insertToFile(&taskList->back());
}

void toggleComplete(std::vector<Tasks> *taskList, int taskNo) {
  for (Tasks &task : *taskList) {
    if (task.id == taskNo) {
      task.completed = !task.completed;
      time_t currentTime = time(nullptr);
      task.modified_at = currentTime;
      break;
    }
  }

  overwrite(taskList);
}

// Deletes task from the linked list
// TO DO delete the task from the file
void deleteTask(std::vector<Tasks> *taskList, int taskNo) {

  taskList->erase(taskList->begin() + taskNo);
  reindex(taskList);
  overwrite(taskList);
}
