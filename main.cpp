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
using namespace std;

// Constants needed
string version = "v2.0.0";
string helpText =
    "\nThis is a tool to keep track of your to dos.\nNo Arguments: "
    "displays all the tasks left and their status.\n--version: "
    "displays the version of the tool.\n--add: you can add a "
    "task using this.\n--complete: asks for the task to "
    "complete.\n--delete: asks for the task to be deleted.\n\n";

// Creating a hardcoded filename
const char *homeDir = getenv("HOME");
string fileName = string(homeDir) + "/.tasks.csv";
int latestTask{0};

// This is a type alias, i.e. creating a shortcut name for a complex data type.
// using is a keyword to tell the compliler That we are decalaring an alias.
// function<...> is a wrapper from functional library, that holds almost
// anything that acts like a function. void(const vector<string>&) defines that
// the alias has to be void, must take a vector of strings as parameter by
// reference.
using CommandFunc = function<void(const vector<string> &)>;

// Creating a class to store Tasks as liked list
class Tasks {

public:
  string task;
  bool completed{false};
  int id;
  Tasks *next;
  time_t created_at;
  time_t modified_at;

  Tasks(string task, int id) {
    // time(nullptr) gets the current Linux TIme
    this->created_at = time(nullptr);
    this->modified_at = time(nullptr);
    this->task = task;
    this->id = id;
    this->next = nullptr;
  }
  Tasks(string task, bool completed, int id, time_t created_at,
        time_t modified_at) {
    this->task = task;
    this->created_at = created_at;
    this->modified_at = modified_at;
    this->completed = completed;
    this->id = id;
    this->next = nullptr;
  }
};

Tasks *head{nullptr};

// ----------------------------------------------------------------------------------------------------------------------------------------------------
// UTILITY FUNCTIONS

void cleanup() {
  Tasks *tmp = head;
  while (tmp) {
    Tasks *next = tmp->next;
    delete tmp;
    tmp = next;
  }
}

int askTaskNumber(string message) {
  int taskNo;
  cout << message;
  cin >> taskNo;
  cin.ignore();
  return taskNo;
}

string parseForCSV(const string &text) {
  bool addQuotes = false;
  string parsedText = "";
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
  fstream file{fileName, file.out | file.app};

  // getting the localtime to enter to the file
  tm c_time = *localtime(&newTask->created_at);
  tm m_time = *localtime(&newTask->modified_at);
  char buf[20];
  strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &c_time);
  string createdTime = parseForCSV(buf);
  strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &m_time);
  string modifiedTime = parseForCSV(buf);

  // feilds declaration and initialization in order i want to insert
  string id = parseForCSV(to_string(newTask->id));
  string task = parseForCSV(newTask->task);
  string completed = newTask->completed ? "True" : "False";

  file << id << "," << task << "," << completed << "," << createdTime << ","
       << modifiedTime << "\n";
}

// Function to create all the tasks loaded from the file into the linked list
void addTask(string task, bool complete, int id, time_t created_at,
             time_t modified_at) {
  latestTask++;
  Tasks *newTask = new Tasks(task, complete, id, created_at, modified_at);

  if (head == nullptr) {
    head = newTask;
  } else {
    Tasks *tmp = head;
    while (tmp->next != nullptr)
      tmp = tmp->next;
    tmp->next = newTask;
  }
}

// Fumction to overwrite files after deleting a task
void overwrite() {
  Tasks *tmp = head;
  if (head == nullptr) {
    cerr << "There are no tasks that exist.\n";
    return;
  }
  fstream file(fileName, file.out);
  for (int i = 1; tmp != nullptr; i++) {
    tmp->id = i;
    tm c_time = *localtime(&tmp->created_at);
    tm m_time = *localtime(&tmp->modified_at);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &c_time);
    string createdTime = parseForCSV(buf);
    strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &m_time);
    string modifiedTime = parseForCSV(buf);

    // feilds declaration and initialization in order i want to insert
    string id = parseForCSV(to_string(tmp->id));
    string task = parseForCSV(tmp->task);
    string completed = tmp->completed ? "True" : "False";

    file << id << "," << task << "," << completed << "," << createdTime << ","
         << modifiedTime << "\n";
  }
}

// Overloading the previous function as to not have re defination of id variable
// of tasks unneccessarily
void overwrite(string mode) {
  Tasks *tmp = head;
  if (head == nullptr) {
    cerr << "There are no tasks that exist.\n";
    return;
  }
  fstream file(fileName, file.out);
  while (tmp != nullptr) {
    tm c_time = *localtime(&tmp->created_at);
    tm m_time = *localtime(&tmp->modified_at);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &c_time);
    string createdTime = parseForCSV(buf);
    strftime(buf, sizeof(buf), "%Y-%m-%d:%H-%M-%S", &m_time);
    string modifiedTime = parseForCSV(buf);

    // feilds declaration and initialization in order i want to insert
    string id = parseForCSV(to_string(tmp->id));
    string task = parseForCSV(tmp->task);
    string completed = tmp->completed ? "True" : "False";

    file << id << "," << task << "," << completed << "," << createdTime << ","
         << modifiedTime << "\n";

    tmp = tmp->next;
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
// STARTUP FUCNTIONS

void parseFromCSV() {
  char letter;
  int quoteCount{0};
  int feildNo{1};
  string feild = "";

  // variables to store the tasks till they are saved to the linked list
  int id;
  string task;
  bool completed;
  time_t created_at;
  time_t modified_at;

  fstream file{fileName, file.in};
  if (!file.is_open())
    cerr << "File could not be opened, tasks were no initiated!\n";

  while (file.get(letter)) {

    if ((letter == ',' || letter == '\n') && quoteCount % 2 == 0) {
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
        istringstream ss{feild};
        struct tm tmp_time{0};
        ss >> get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (ss.fail())
          cerr << "Could not parse time!\n";
        else
          created_at = mktime(&tmp_time);
        break;
      }

      case 5: {
        istringstream ss{feild};
        struct tm tmp_time{0};
        ss >> get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (ss.fail())
          cerr << "Could not parse time!\n";
        else
          modified_at = mktime(&tmp_time);
        break;
      }
      }

      feild = "";
      quoteCount = 0;

      if (feildNo % 5 == 0 || letter == '\n') {
        feildNo = 1;
        addTask(task, completed, id, created_at, modified_at);
      } else
        feildNo++;
      continue;

    } else if (letter == '"' && file.peek() == '"') {
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
  fstream f{fileName, f.in};
  if (!f.is_open()) {
    ofstream create(fileName);
    create.close();
  }
  parseFromCSV();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// LINKED LIST FUNCTIONS

// TODO: Use non hardcoded value and use CSV format

// Fucntion for the user to create task and then saves into the file
void createTask() {
  string text;
  cout << "Enter the task: ";
  getline(cin, text);
  latestTask++;
  Tasks *newTask = new Tasks(text, latestTask);

  Tasks *tmp = head;
  if (head == nullptr)
    head = newTask;
  else {
    while (tmp->next != nullptr)
      tmp = tmp->next;
    tmp->next = newTask;
  }

  insertToFile(newTask);
}

// TODO: Make the display of tasks more visually appealing and also be concious
// of the terminal size
void displayTasks() {
  if (head == nullptr) {
    cout << "There are no tasks!\n";
    return;
  }

  // set up a temporary variable to store head then iterates through nodes and
  // prints the ones that are complete
  Tasks *tmp = head;
  cout << "----------------------------------------COMPLETE--------------------"
          "--------------------\n";
  while (tmp != nullptr) {
    if (tmp->completed)
      cout << "Id: " + to_string(tmp->id) + ",\tTask: " << tmp->task
           << "\tCompleted: True\n";
    tmp = tmp->next;
  }

  // set up a temporary variable to store head then iterates through nodes and
  // prints the ones that are incomplete
  tmp = head;
  cout << "---------------------------------------INCOMPLETE-------------------"
          "--------------------\n";
  while (tmp != nullptr) {
    if (!tmp->completed)
      cout << "Id: " + to_string(tmp->id) + "\tTask: " << tmp->task
           << "\tCompleted: False\n";
    tmp = tmp->next;
  }
}

void setComplete() {
  displayTasks();
  cout << "\n";

  int taskNo = askTaskNumber("What task would you like to set as complete: ");
  if (head == nullptr) {
    cout << "There are no tasks!\n";
    return;
  }
  if (taskNo > latestTask) {
    cout << "There is no such task.\n";
  }
  Tasks *tmp = head;
  for (int i = 1; i < taskNo; i++)
    tmp = tmp->next;

  tmp->completed = true;
  tmp->modified_at = time(nullptr);
  cout << "Task " << taskNo << " was set to complete.\n";
  overwrite("edit");
}

// Deletes task from the linked list
// TO DO delete the task from the file
void deleteTask() {
  displayTasks();
  cout << "\n";
  int taskNo = askTaskNumber("Which task would you like to delete: ");

  if (head == nullptr) {
    cout << "There are no tasks!\n";
    return;
  }
  if (taskNo > latestTask || taskNo < 1) {
    cout << "There is no such task\n";
    return;
  }

  if (taskNo == 1) {
    Tasks *toBeDeleted = head;
    head = head->next;
    delete toBeDeleted;
  } else {

    Tasks *tmp = head;
    for (int i = 1; i < taskNo - 1; i++)
      tmp = tmp->next;

    Tasks *toBeDeleted = tmp->next;
    tmp->next = toBeDeleted->next;
    delete toBeDeleted;
  }
  overwrite();
  latestTask--;
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
    return 0;
  }
  // Making a command app
  // in <string, CommandFunc> the first argument is the data type of the key and
  // the second argument is the data type of the value, here the data type is a
  // custom class
  map<string, CommandFunc> commands;

  // Register a command
  // commands is the map we made, "--version" is the key, the value for this is
  // a lambda function which takes a vector of strings as its parameter,
  // this vector is where we will store the rest of the arguments later on
  // when we match the arguments passed when calling the file.
  commands["--version"] = [](const vector<string> &args) {
    cout << version << '\n';
  };
  commands["--add"] = [](const vector<string> &args) { createTask(); };
  commands["--complete"] = [](const vector<string> &args) { setComplete(); };
  commands["--delete"] = [](const vector<string> &args) { deleteTask(); };
  commands["--help"] = [](const vector<string> &args) { cout << helpText; };

  // Extract the command and its specific arguments
  // we are taking the second word passed, that is the argument and then put the
  // rest of the arguments in a string vector
  string cmdName = argv[1];
  vector<string> cmdArgs;
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
    cout << "Unknown Command: " << cmdName << "\n";
  }

  overwrite("edit");
  cleanup();
}
