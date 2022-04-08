cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project ("time-tracker")

# target names
set(TARGET time-tracker)

# dependencies
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false src/*)

# include directories
include_directories(include)

# targets
add_executable(${TARGET} ${SOURCES})