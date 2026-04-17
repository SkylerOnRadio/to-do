#include <cstddef>
#include <iostream>
using namespace std;

class td {
private:
  string task;
  bool completed = false;

public:
  void askTask() {
    string text;
    cout << "Enter the task: ";
    getline(cin, text);
    task = text;
  }
  void getTask() {
    std::cout << "Task: " << task
              << "\tCompleted: " << (completed ? "Yes" : "No") << endl;
  }
  string valOfTask() { return task; }
  void setValOfTask(string text) { task = text; }
  void completeTask() { completed = true; }
  bool isComplete() { return completed; }
};

td tasks[50];
int latestTask = 0;

int askTaskNumber(string message) {
  int taskNo;
  cout << message;
  cin >> taskNo;
  cin.ignore();
  return taskNo;
}

void deleteTask(int taskNo) {
  for (int i = taskNo; i < latestTask - 1; i++) {
    string nextTask = tasks[i + 1].valOfTask();
    tasks[i].setValOfTask(nextTask);
  }
  tasks[latestTask].setValOfTask(nullptr_t);
  cout << "Task " << taskNo << " has been deleted.\n";
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
      tasks[latestTask].askTask();
      latestTask++;
      break;

    case 2:
      cout << "-------------------------------------------------INCOMPLETE-----"
              "----------------------------------------------"
           << endl;
      for (int j = 0; j < latestTask; j++) {
        if (tasks[j].isComplete())
          tasks[j].getTask();
      }
      cout << "--------------------------------------------------COMPLETE------"
              "----------------------------------------------"
           << endl;
      for (int j = 0; j < latestTask; j++) {
        if (!tasks[j].isComplete())
          tasks[j].getTask();
      }
      break;

    case 3: {
      for (int i = 0; i < latestTask; i++) {
        tasks[i].getTask();
      }
      int taskNo =
          askTaskNumber("What task would you like to set as complete: ");
      tasks[taskNo - 1].completeTask();
      cout << "Task " << taskNo << " was set to complete.\n";
      break;
    }

    case 4: {
      int taskNo = askTaskNumber("Which task would you like to delete: ");
      deleteTask(taskNo - 1);
      break;
    }

    default:
      std::cout << "The input you made was invalid, please try again!";
      break;
    }
  }
}
