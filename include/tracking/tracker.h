#pragma once
#include "system/screen-controller.h"
#include "time/hours-file.h"
#include <thread>
#include <atomic>
#include <filesystem>

namespace fs = std::filesystem;

class Tracker{
private:
    fs::path config;
    std::thread printing;
    HoursFile hours_file;
    std::atomic<bool> exiting = false;
    double hourly_rate = -1.f;

public:
    Tracker();
    void start();
    void shutdown();
    void print();
};