#pragma once
#include <ncurses.h>
#include "tracking/figures.h"

class ScreenController {
public:
    static void initialize();
    static void deinitialize();
    static void print(const Figures &figures, bool clocked_in);
};