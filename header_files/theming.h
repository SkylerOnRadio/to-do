#ifndef THEMING_H
#define THEMING_H

#include <cstdint>
#include <ncurses.h>

namespace colours {

constexpr int8_t BLUE = 32;
constexpr int8_t GREEN = 33;
constexpr int8_t SKY = 34;
constexpr int8_t WHITE = 35;
constexpr int8_t EMERALD = 36;
}; // namespace colours

namespace pairs {

constexpr int8_t TASKS_LIST_PAIR = 32;
constexpr int8_t KEY_PAIR = 33;
constexpr int8_t VALUE_PAIR = 34;
constexpr int8_t BORDER_PAIR = 35;
constexpr int8_t STATUS_BAR = 36;
constexpr int8_t OTHER_MENU = 37;
} // namespace pairs

void importColors();
void createPairs();

#endif // !THEMING_H
