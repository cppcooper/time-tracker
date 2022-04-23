cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project ("time-tracker")

if(APPLE)
    message("If it works, it works.. but apple builds are not currently supported.")
endif()

if(UNIX)
    find_package(Curses)
    if(NOT CURSES_HAVE_NCURSES_H)
        message(FATAL_ERROR "Ncurses not found, please install.")
    endif()
endif()

# target name
set(TARGET ${PROJECT_NAME})

# dependencies
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false src/*)
add_subdirectory(extern/fmt EXCLUDE_FROM_ALL)
add_subdirectory(extern/hhdate EXCLUDE_FROM_ALL)

# include directories
include_directories(include)
include_directories(extern/fmt/include)
include_directories(extern/hhdate/include)

# targets
if(UNIX)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX /usr/bin)
    endif()
endif()
add_executable(${TARGET} ${SOURCES})
if(UNIX)
    target_link_libraries(${TARGET} ncurses pthread fmt)
else()
    target_link_libraries(${TARGET} pthread fmt)
endif()
install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})