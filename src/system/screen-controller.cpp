#include "system/screen-controller.h"
#include <thread>
#if defined(__linux) || defined(__linux__) || defined(linux)
# define LINUX
#elif defined(__APPLE__)
# define MACOS
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
# define WINDOWS
#endif


void ScreenController::initialize() {
#ifdef LINUX
    initscr();
    noecho();
    cbreak();
    //halfdelay(1);
    scrollok(stdscr, FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
#endif
}

void ScreenController::deinitialize() {
#ifdef LINUX
    endwin();
#endif
}

void print_ncurses(const Figures &figures, bool clocked_in) {
    clear();
    printw("\nTime Tracker\n- - - - - -");
    printw("\n -        Space   -- clock(in/out).");
    printw("\n - Ctrl + C       -- exit.");
    if (clocked_in) {
        printw("\n                  [CLOCKED IN]");
    } else {
        printw("\n");
    }
    printw("\n                  [%s]", figures.date.c_str());
    if (figures.earnings <= 0.f) {
        printw("\nAccumulated Time: %s", figures.accumulated_time.c_str());
        printw("\nSession Time:     %s\n", figures.session_time.c_str());
    } else {
        printw("\nAccumulated Time: %s - $%.2f", figures.accumulated_time.c_str(), figures.earnings);
        printw("\nSession Time:     %s - $%.2f\n", figures.session_time.c_str(), figures.session_earnings);
    }
    refresh();
}

void print_windows(const Figures &figures, bool clocked_in) {
    for(int i = 0; i < 7; ++i) {
        printf("\x1b[A\r                               \r");
    }
    printf("\nTime Tracker\n- - - - - -");
    printf("\n -        Space   -- clock(in/out).");
    printf("\n - Ctrl + C       -- exit.");
    if (clocked_in) {
        printf("\n                  [CLOCKED IN]");
    } else {
        printf("\n");
    }
    printf("\n                  [%s]", figures.date.c_str());
    if (figures.earnings <= 0.f) {
        printf("\nAccumulated Time: %s", figures.accumulated_time.c_str());
        printf("\nSession Time:     %s\n", figures.session_time.c_str());
    } else {
        printf("\nAccumulated Time: %s - $%.2f", figures.accumulated_time.c_str(), figures.earnings);
        printf("\nSession Time:     %s - $%.2f\n", figures.session_time.c_str(), figures.session_earnings);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ScreenController::print(const Figures &figures, bool clocked_in) {
#ifdef LINUX
    print_ncurses(figures, clocked_in);
#elifdef WINDOWS
    print_windows(figures, clocked_in);
#endif
}
