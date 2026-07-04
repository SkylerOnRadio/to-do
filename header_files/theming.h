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
constexpr int8_t RED = 37;
constexpr int8_t YELLOW = 38;
constexpr int8_t GRAY = 39;
constexpr int8_t GRAY_BLUE = 40;
constexpr int8_t DARK_BLUE = 41;
constexpr int8_t LIGHT_GREY = 42;
constexpr int8_t HIGHLIGHT = 43;
constexpr int8_t LIGHT_BLUE = 44;
}; // namespace colours

namespace pairs {

constexpr int8_t TASKS_LIST_PAIR = 32;
constexpr int8_t KEY_PAIR = 33;
constexpr int8_t VALUE_PAIR = 34;
constexpr int8_t BORDER_PAIR = 35;
constexpr int8_t STATUS_BAR = 36;
constexpr int8_t OTHER_MENU = 37;
constexpr int8_t INCOMPLETE_PAIR = 39;
constexpr int8_t ONGOING_PAIR = 40;
constexpr int8_t COMPLETE_PAIR = 41;
constexpr int8_t HIGHLIGHT_PAIR = 42;
} // namespace pairs

void importColors();
void createPairs();

#endif // !THEMING_H
