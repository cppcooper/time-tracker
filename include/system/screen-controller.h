#pragma once
#include "tracking/figures.h"
#if defined(__linux) || defined(__linux__) || defined(linux)
 #define LINUX
#elif defined(__APPLE__)
 #define MACOS
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
 #define WINDOWS
#endif

class ScreenController {
public:
    static void initialize();
    static void deinitialize();
    static void print(const Figures &figures, bool clocked_in);
};