#include "system/screen-controller.h"

void ScreenController::initialize() {
    initscr();
    noecho();
    cbreak();
    //halfdelay(1);
    scrollok(stdscr, FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
}

void ScreenController::deinitialize() {
    endwin();
}

void ScreenController::print(const Figures &figures, bool clocked_in) {
    clear();
    printw("\nTime Tracker");
    printw("\n -        Space   -- clock(in/out).");
    printw("\n - Ctrl + C       -- stop tracking and exit.");
    if (clocked_in) {
        printw("\n    [CLOCKED IN]");
    } else {
        printw("\n");
    }
    printw("\n [%s]", figures.date.c_str());
    if (figures.earnings <= 0.f) {
        printw("\nAccumulated Time: %s", figures.accumulated_time.c_str());
        printw("\nSession Time:     %s\n", figures.session_time.c_str());
    } else {
        printw("\nAccumulated Time: %s - $%.2f", figures.accumulated_time.c_str(), figures.earnings);
        printw("\nSession Time:     %s - $%.2f\n", figures.session_time.c_str(), figures.session_earnings);
    }
    refresh();
}
