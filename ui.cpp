#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#define WIDTH 30
#define HEIGHT 10

class Tasks {
public:
  std::string task, created_at, modified_at, complete_at, complete, repeating;
  int id;
  Tasks(int id, std::string task, std::string created_at,
        std::string modified_at, std::string complete_at, std::string complete,
        std::string repeating) {
    this->id = id;
    this->task = task;
    this->created_at = created_at;
    this->modified_at = modified_at;
    this->complete_at = complete_at;
    this->complete = complete;
    this->repeating = repeating;
  }
};

void print_taskList(WINDOW *taskList_win, int highlight,
                    std::vector<Tasks> *taskList, int startIndex,
                    int endIndex) {
  int x, y;
  x = 2;
  y = 1;

  int maxx, maxy;
  getmaxyx(taskList_win, maxy, maxx);

  werase(taskList_win);
  box(taskList_win, 0, 0);

  if (!has_colors()) {
    std::cout << "Your terminal does not support colors.\n";
  }

  for (int i = startIndex; i <= endIndex; i++) {

    const Tasks &task = taskList->at(i);
    std::string text = task.task + '\t' + std::to_string(task.id);

    if (highlight == task.id) {
      wattron(taskList_win, A_REVERSE);
      mvwaddstr(taskList_win, y, x, text.c_str());
      wattroff(taskList_win, A_REVERSE);
    } else
      mvwaddstr(taskList_win, y, x, text.c_str());

    y++;
  }
  wrefresh(taskList_win);
}

int main() {
  WINDOW *taskList_win;
  int inputch;
  int highlight{1};

  std::vector<Tasks> taskList;
  for (int i = 1; i <= 50; ++i) {
    taskList.emplace_back(i, "This is a task", "9-5-2016", "9-5-2016",
                          "9-5-2016", "True", "Ues");
  }

  initscr();
  clear();
  noecho();
  cbreak();
  int startx = 0;
  int starty = 2;

  taskList_win = newwin((2 * LINES) / 3, (2 * COLS) / 3, starty, startx);
  int y, x;
  getmaxyx(taskList_win, y, x);
  int start{0};
  int end{y - 3};

  mvaddstr(0, 0, "This is the test task displayer.");
  refresh();

  print_taskList(taskList_win, highlight, &taskList, start, end);

  keypad(taskList_win, TRUE);

  while (1) {
    inputch = wgetch(taskList_win);
    switch (inputch) {
    case 'k':
      if (highlight == 1)
        break;
      else {
        highlight--;
        if (highlight - start < 3 && start != 0)
          start--;
      }
      break;

    case 'j':
      if (highlight == 50)
        break;
      else {
        highlight++;
        if (highlight - 1 >= start + y - 2)
          start++;
      }
      break;

    case 'q':
      endwin();
      return 0;
    }
    end = start + y - 3;
    print_taskList(taskList_win, highlight, &taskList, start, end);
  }
}
