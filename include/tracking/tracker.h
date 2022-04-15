#pragma once
#include "punch-clock.h"
#include "system/screen-controller.h"
#include "system/hours-file.h"
#include <thread>
#include <atomic>
#include <filesystem>

namespace fs = std::filesystem;

class Tracker{
private:
    fs::path config;
    std::thread printing;
    PunchClock clock;
    HoursFile hours_file;
    std::atomic<bool> paused = false;
    std::atomic<bool> exiting = false;
    double hourly_rate = -1.f;

protected:
    int16_t key_wait();

public:
    Tracker(int);
    void close();
    void track_time();
    void print();
};