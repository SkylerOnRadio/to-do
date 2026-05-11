#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Constants needed
std::string version = "v2.1.0";
std::string helpText =
    "\nThis is a tool to keep track of your to dos.\nNo Arguments: "
    "displays all the tasks left and their status.\n--version: "
    "displays the version of the tool.\n--add: you can add a "
    "task using this.\n--complete: asks for the task to "
    "complete.\n--delete: asks for the task to be deleted.\n\n";

// Creating a hardcoded filename
const char *homeDir = getenv("HOME");
std::string fileName = std::string(homeDir) + "/.tasks.csv";

// This is a type alias, i.e. creating a shortcut name for a complex data type.
// using is a keyword to tell the compliler That we are decalaring an alias.
// function<...> is a wrapper from functional library, that holds almost
// anything that acts like a function. void(const vector<string>&) defines that
// the alias has to be void, must take a vector of strings as parameter by
// reference.
using CommandFunc = std::function<void(const std::vector<std::string> &)>;

// Creating a class to store Tasks as liked list
class Tasks {

public:
  std::string task;
  bool completed{false};
  int id;
  time_t created_at;
  time_t modified_at;

  Tasks(std::string task, int id) {
    // time(nullptr) gets the current Linux TIme
    this->created_at = time(nullptr);
    this->modified_at = time(nullptr);
    this->task = task;
    this->id = id;
  }
  Tasks(std::string task, bool completed, int id, time_t created_at,
        time_t modified_at) {
    this->task = task;
    this->created_at = created_at;
    this->modified_at = modified_at;
    this->completed = completed;
    this->id = id;
  }
};

std::vector<Tasks> taskList;

// ----------------------------------------------------------------------------------------------------------------------------------------------------
// UTILITY FUNCTIONS
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
void addTask(std::string task, bool complete, int id, time_t created_at,
             time_t modified_at) {
  taskList.emplace_back(task, complete, id, created_at, modified_at);
}

void reindex() {
  int currentTask{1};

  for (Tasks &task : taskList) {
    task.id = currentTask++;
  }
}

// Fumction to overwrite files after deleting a task
void overwrite() {

  std::fstream file(fileName, file.out);
  for (const Tasks &task : taskList) {
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

void parseFromCSV() {
  char letter;
  int i{1};
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
        addTask(task, completed, id, created_at, modified_at);
        i++;
      } else
        // increment field if we are not at the last field
        feildNo++;
      continue;

    }
    // if the letter is " and the next character is also " then we dont need to
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
void startup() {
  std::fstream f{fileName, f.in};
  if (!f.is_open()) {
    std::ofstream create(fileName);
    create.close();
  }
  parseFromCSV();
  reindex();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// LINKED LIST FUNCTIONS

// Fucntion for the user to create task and then saves into the file
void createTask() {
  std::string text;
  std::cout << "Enter the task: ";
  getline(std::cin, text);
  int newTaskId = taskList.empty() ? 1 : taskList.back().id + 1;
  taskList.emplace_back(text, newTaskId);

  insertToFile(&taskList.at(taskList.back().id - 1));
}

// TODO: Make the display of tasks more visually appealing and also be concious
// of the terminal size
void displayTasks() {
  if (taskList.empty()) {
    std::cout << "There are no tasks!\n";
    return;
  }

  std::cout
      << "----------------------------------------COMPLETE------------------"
         "--"
         "--------------------\n";
  for (const Tasks &task : taskList) {
    if (task.completed)
      std::cout << "Id: " + std::to_string(task.id) + ",\tTask: " << task.task
                << "\tCompleted: True\n";
  }

  std::cout
      << "---------------------------------------INCOMPLETE-----------------"
         "--"
         "--------------------\n";
  for (const Tasks &task : taskList) {
    if (!task.completed)
      std::cout << "Id: " + std::to_string(task.id) + "\tTask: " << task.task
                << "\tCompleted: False\n";
  }
}

void toggleComplete() {
  displayTasks();
  std::cout << "\n";

  int taskNo = askTaskNumber("What task would you like to set as complete: ");

  for (Tasks &task : taskList) {
    if (task.id == taskNo) {
      task.completed = !task.completed;
      break;
    }
  }

  overwrite();
}

// Deletes task from the linked list
// TO DO delete the task from the file
void deleteTask() {
  displayTasks();
  std::cout << "\n";
  int taskNo = askTaskNumber("Which task would you like to delete: ");

  if (taskNo > taskList.back().id || taskNo < 1) {
    std::cout << "There is no such task\n";
    return;
  }
  int i = 0;
  for (const Tasks &task : taskList) {
    if (task.id == taskNo)
      break;
    i++;
  }

  taskList.erase(taskList.begin() + i);
  reindex();
  overwrite();
}

// TODO: Currrenly only the first argument passed is processed, need to make
// multiple arguments be processed do chainning of commands becomes possible. In
// addition to this the ability to not need to be asked the task to add, delete,
// complete but rather be able to pass them as an argument itself and then that
// is processed

// argc is the number of arguments passed
// argv is a pointer array where each element points to the first character of
// each argument
int main(int argc, char *argv[]) {
  startup();

  if (argc < 2) {
    displayTasks();
    overwrite();
    return 0;
  }
  // Making a command app
  // in <string, CommandFunc> the first argument is the data type of the key and
  // the second argument is the data type of the value, here the data type is a
  // custom class
  std::map<std::string, CommandFunc> commands;

  // Register a command
  // commands is the map we made, "--version" is the key, the value for this is
  // a lambda function which takes a vector of strings as its parameter,
  // this vector is where we will store the rest of the arguments later on
  // when we match the arguments passed when calling the file.
  commands["--version"] = [](const std::vector<std::string> &args) {
    std::cout << version << '\n';
  };
  commands["--add"] = [](const std::vector<std::string> &args) {
    createTask();
  };
  commands["--complete"] = [](const std::vector<std::string> &args) {
    toggleComplete();
  };
  commands["--delete"] = [](const std::vector<std::string> &args) {
    deleteTask();
  };
  commands["--help"] = [](const std::vector<std::string> &args) {
    std::cout << helpText;
  };

  // Extract the command and its specific arguments
  // we are taking the second word passed, that is the argument and then put the
  // rest of the arguments in a string vector
  std::string cmdName = argv[1];
  std::vector<std::string> cmdArgs;
  for (int i = 2; i < argc; ++i)
    cmdArgs.push_back(argv[i]);

  // Execution Logic
  // We find the argument that is passed, the function find if it doesn't ind
  // the value passed then will return the memory address right after the last
  // element, command.end() will return the memory address right after the last
  // element, so if the value is not found then the if block will be false and
  // won't run
  if (commands.find(cmdName) != commands.end()) {
    // commands[cmdName] takes the value of the value stored to it, in our case
    // a function, since it now takes the function as its value, we use () to
    // tell the function to run and pass the remaining arguments.
    // NOTE: calling a function by its name itself does not run it, we have
    // to use () to run the function
    commands[cmdName](cmdArgs);
  } else {
    std::cout << "Unknown Command: " << cmdName << "\n";
  }

  overwrite();
}
