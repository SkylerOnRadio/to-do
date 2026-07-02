#ifndef THEMING_H
#define THEMING_H

#include <cstdint>
#include <ncurses.h>

namespace colours {

constexpr int8_t BLUE = 32;
constexpr int8_t GREEN = 33;
}; // namespace colours

namespace pairs {

constexpr int8_t TASKS_LIST_PAIR = 32;
}

void importColors();
void createPairs();

#endif // !THEMING_H
