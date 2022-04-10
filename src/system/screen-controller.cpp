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

void ScreenController::print(const Figures &figures, bool paused) {
    clear();
    printw("\nTime Tracker\n-----------");
    printw("\n -        Space   -- pause/resume tracking.");
    printw("\n -        Insert  -- load accumulated time.");
    printw("\n -        BkSpace -- don't show accumulated time.");
    printw("\n - Ctrl + BkSpace -- reset accumulated time.");
    printw("\n - Ctrl + C       -- stop tracking and exit.\n\n");
    if (paused) {
        printw("\n    - PAUSED -   ");
    } else {
        printw("\n");
    }
    if (figures.earnings < 0.f) {
        printw("\nAccumulated Time: %s", figures.accumulated_time.c_str());
    } else {
        printw("\nAccumulated Time: %s - $%.2f", figures.accumulated_time.c_str(), figures.earnings);
    }
    printw("\nSession Time:     %s\n", figures.session_time.c_str());
    refresh();
}
