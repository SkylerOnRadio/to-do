#include "header_files/inputHandling.h"
#include "header_files/global.h"

#define CTRL(key) (key & 0x1F)

void handleInput(int input) {
  switch (input) {
  case 'q':
    exit_unique = true;
    break;

  case CTRL('c'):
    exit_unique = true;
    break;
  }
}
