#include <iostream>
using namespace std;

// Creating a class to store Tasks as liked list
class Tasks {

public:
  string task;
  bool completed = false;
  Tasks *next;

  Tasks(string task) {
    this->task = task;
    this->next = nullptr;
  }
};

int latestTask = 0;
Tasks *head = nullptr;

// checking if head exists or not was getting repitative so did this
bool isHead() {
  if (head == nullptr)
    return false;
  return true;
}

void createTask() {
  string text;
  cout << "Enter the task: ";
  getline(cin, text);
  Tasks *newTask = new Tasks(text);
  latestTask++;

  if (!isHead()) {
    head = newTask;
    return;
  }
  Tasks *tmp = head;
  while (tmp->next != nullptr)
    tmp = tmp->next;
  tmp->next = newTask;
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
      cout << "Task: " << tmp->task << "\tCompleted: True\n";
    tmp = tmp->next;
  }

  // set up a temporary variable to store head then iterates through nodes and
  // prints the ones that are incomplete
  tmp = head;
  cout << "---------------------------------------INCOMPLETE-------------------"
          "--------------------\n";
  while (tmp != nullptr) {
    if (!tmp->completed)
      cout << "Task: " << tmp->task << "\tCompleted: False\n";
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
  cout << "Task " << taskNo << " was set to complete.\n";
}

void deleteTask(int taskNo) {
  if (!isHead()) {
    cout << "There are no tasks!\n";
    return;
  }
  if (taskNo > latestTask) {
    cout << "There is no such task\n";
    return;
  }
  Tasks *tmp = head;
  for (int i = 1; i < taskNo - 1; i++)
    tmp = tmp->next;

  Tasks *toBeDeleted = tmp->next;
  tmp->next = toBeDeleted->next;
  delete toBeDeleted;
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
