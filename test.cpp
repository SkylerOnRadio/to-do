#include <algorithm>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Creating a hardcoded filename
string fileName = "tasks.txt";
int latestTask{0};

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

// checking if head exists or not was getting repitative so did this
bool isHead() {
  if (head == nullptr)
    return false;
  return true;
}

// Function to create all the tasks loaded from the file into the linked list
void addTask(string task, bool complete, int id, time_t created_at,
             time_t modified_at) {
  latestTask++;
  Tasks *newTask = new Tasks(task, complete, id, created_at, modified_at);

  if (!isHead()) {
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
    string Block = string("[\n") + "Id: " + to_string(tmp->id) + ",\n" +
                   "Task: \"" + tmp->task +
                   "\",\nCompleted: " + (tmp->completed ? "true" : "false") +
                   ",\n],\n";
    file << Block;
    tmp = tmp->next;
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
    string Block = string("[\n") + "Id: " + to_string(tmp->id) + ",\n" +
                   "Task: \"" + tmp->task +
                   "\",\nCompleted: " + (tmp->completed ? "true" : "false") +
                   ",\n],\n";
    file << Block;
    tmp = tmp->next;
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
// STARTUP FUCNTIONS

// Gets all the tasks previously stored in the file
void getTasksFromFile() {
  fstream file{fileName, file.in};
  if (!file.is_open()) {
    cerr << "File could not be opened, tasks were not initialized!\n";
  }

  // bracCount variable makes sure that distinct tasks are handled distinctly
  // the file structure is such that each task info is sorrounded by [], hence
  // by keeping track of the bracket count we can know when a distinct bracket
  // is being read taskBlock is to store the entire data of the task that is
  // then processed later taskVector is where each word that actually comprises
  // of the task is stored
  //  TO DO merge the task separation and processing into one process
  int bracCount{0};
  string taskBlock = "";
  vector<string> taskVector;

  do {
    // reads through the file and increments bracCount when [ is encountered,
    // decrements bracCount when ] is found if ' ' or , is found then we store
    // the word into the vector and reset taskBlock and if the letter is not any
    // of them then the letter is concatinated with task

    char letter = file.get();
    if (letter == '[')
      bracCount++;
    else if (letter == ']')
      bracCount--;
    else if (letter == ' ' || letter == ',') {
      taskBlock.erase(remove_if(taskBlock.begin(), taskBlock.end(),
                                [](unsigned char x) { return isspace(x); }),
                      taskBlock.end());
      taskVector.push_back(taskBlock);
      taskBlock = "";
    }

    if (letter != ',' && letter != '[' && letter != ']' && letter != '"' &&
        letter != '\n')
      taskBlock = taskBlock + letter;

    // once bracCount is 0, we loop through the vectored list of strings and try
    // to find the keywords, IF Task: is found then all the following words will
    // be put into tasks IF Completed: is found then the completed variable of
    // task is set to true
    if (bracCount == 0 && letter == ']') {
      string task = "";
      bool completed;
      int id;
      time_t created_at, modified_at;

      bool taskFlag{false}, completedFlag{false}, idFlag{false};
      bool modifiedFlag{false}, createdFlag{false};
      for (string word : taskVector) {
        if (word == "Completed:") {
          completedFlag = true;
          taskFlag = false;
          idFlag = false;
          modifiedFlag = false;
          createdFlag = false;
          continue;
        } else if (word == "Task:") {
          taskFlag = true;
          completedFlag = false;
          idFlag = false;
          modifiedFlag = false;
          createdFlag = false;
          continue;
        } else if (word == "Id:") {
          taskFlag = false;
          completedFlag = false;
          idFlag = true;
          modifiedFlag = false;
          createdFlag = false;
          continue;
        } else if (word == "Created_At:") {
          taskFlag = false;
          completedFlag = false;
          idFlag = false;
          modifiedFlag = false;
          createdFlag = true;
          continue;
        } else if (word == "Modified_At:") {
          taskFlag = false;
          completedFlag = false;
          idFlag = false;
          modifiedFlag = true;
          createdFlag = false;
          continue;
        }

        if (taskFlag)
          task = task + " " + word;
        else if (completedFlag) {
          completed = word == "false" ? false : true;
        } else if (idFlag) {
          id = stoi(word);
        } else if (createdFlag) {
          istringstream ss(word);
          struct tm tmp_tm = {0};
          ss >> get_time(&tmp_tm, "%d-%m-%Y %H-%M-%S");
          if (ss.fail())
            cerr << "Failed to parse time.\n";
          else
            created_at = mktime(&tmp_tm);
        } else if (modifiedFlag) {
          struct tm tmp_tm = {0};
          istringstream ss(word);
          ss >> get_time(&tmp_tm, "%d-%m-%Y %H-%M-%S");
          if (ss.fail())
            cerr << "Failed to parse time\n";
          else
            modified_at = mktime(&tmp_tm);
        }
      }

      taskVector.clear();
      addTask(task, completed, id, created_at, modified_at);
      task = "";
    }
  } while (!file.eof());
}

// Fucntion to run on startup, will create file if it doesn't exist and will
// call the function to load all tasks from file
void startup() {
  fstream f{fileName, f.in};
  if (!f.is_open())
    cerr << "File Error!\n";
  getTasksFromFile();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// LINKED LIST FUNCTIONS

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

  // writing the task to file
  fstream file{fileName, file.out | file.app};
  tm *c_time = localtime(&newTask->created_at);
  tm *m_time = localtime(&newTask->modified_at);
  // file.seekg(0, ios::end);
  // string Block = string("[\n") + "Id: " + to_string(latestTask) + ",\n" +
  //                "Task: \"" + newTask->task +
  //                "\",\nCompleted: " + (newTask->completed ? "true" : "false")
  //                +
  //                ",\nCreated_At: " + strftime(localtime(tmp->created_at)) +
  //                ",\nModified_At: " + strftime(localtime(tmp->modified_at)) +
  //                ",\n],\n";
  // file << Block << endl;
  file.seekg(0, ios::end);
  file << string("[\n") << "Id: " << to_string(latestTask) << ",\n"
       << "Task: \"" << newTask->task
       << "\",\nCompleted: " << (newTask->completed ? "true" : "false")
       << ",\nCreated_At: " << put_time(c_time, "%d-%m-%Y %H-%M-%S")
       << ",\nModified_At: " << put_time(m_time, "%d-%m-%Y %H-%M-%S")
       << ",\n],\n"
       << endl;
}

void displayTasks() {
  if (!isHead()) {
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
      cout << "Id: " + to_string(tmp->id) + ",\nTask: " << tmp->task
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

void setComplete(int taskNo) {
  if (!isHead()) {
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
void deleteTask(int taskNo) {

  if (!isHead()) {
    cout << "There are no tasks!\n";
    return;
  }
  if (taskNo > latestTask || taskNo < 1) {
    cout << "There is no such task\n";
    return;
  }
  Tasks *tmp = head;
  for (int i = 1; i < taskNo - 1; i++)
    tmp = tmp->next;

  Tasks *toBeDeleted = tmp->next;
  tmp->next = toBeDeleted->next;
  delete toBeDeleted;
  overwrite();
  latestTask--;
}

int askTaskNumber(string message) {
  int taskNo;
  cout << message;
  cin >> taskNo;
  cin.ignore();
  return taskNo;
}

int main() {
  startup();
  int input = 1;
  while (input != 0) {
    cout << "------------------------------------------------------------------"
            "--------------------------------------------"
         << endl;
    cout << "You have " << latestTask << " tasks.\n";
    cout << "Enter 0 to exit.\nEnter what you would like to do:\n1. Insert "
            "Task\n2. List all Tasks\n3. Complete a task\n4. Delete a task\n";
    cin >> input;
    cin.ignore();
    switch (input) {
    case 0:
      break;

    case 1:
      createTask();
      break;

    case 2:
      displayTasks();
      break;

    case 3: {
      int taskNo =
          askTaskNumber("What task would you like to set as complete: ");
      setComplete(taskNo);
      break;
    }

    case 4: {
      int taskNo = askTaskNumber("Which task would you like to delete: ");
      deleteTask(taskNo);
      break;
    }

    default:
      std::cout << "The input you made was invalid, please try again!";
      break;
    }
  }
}
