#pragma once
#include "time/formatting.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <atomic>

namespace fs = std::filesystem;

class HoursFile {
private:
    fs::path file;
    std::fstream log;
    time_point_sc date;
    time_point_sc time_in;
    chrono::minutes elapsed_past = chrono::minutes::zero();
    chrono::minutes elapsed_today = chrono::minutes::zero();
    std::atomic<bool> _clocked_in = false;
protected:
    void save(const time_point_sc &clock);
public:
    HoursFile();
    void load(); // set date
    void clockin();
    void clockout();
    void new_day();
    //void new_file();

    const chrono::minutes &past = elapsed_past;
    const chrono::minutes &today = elapsed_today;
    const time_point_sc &clock_in = time_in;
    const std::atomic<bool> &clocked_in = _clocked_in;
};
