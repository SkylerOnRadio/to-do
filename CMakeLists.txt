cmake_minimum_required(VERSION 4.3)

#project name
project(todo)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)

# add the executable
add_executable(${PROJECT_NAME} main.cpp model.cpp tasks.cpp ui.cpp)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
