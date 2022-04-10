cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project ("time-tracker")

find_package(Curses)
if(NOT CURSES_HAVE_NCURSES_H)
    message(FATAL_ERROR "Ncurses not found, please install.")
endif()

# target names
set(TARGET time-tracker)

# dependencies
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false src/*)

# include directories
include_directories(include)

# targets
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr/bin)
endif()
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ncurses)
install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})