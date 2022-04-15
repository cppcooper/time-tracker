#pragma once
#include <fstream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
using namespace std::chrono;

class HoursFile {
private:
    fs::path file;
    std::fstream log;
    time_point<system_clock> date;
    double _previous_seconds = 0;
    double _previous_hours = 0;
public:
    HoursFile();
    void load_past_hours();
    void load_day_hours();
    void save(double elapsed);
    void clear();
    void zero();
    std::string latest_date();

    const double &previous_seconds = _previous_seconds;
    const double &previous_hours = _previous_hours;
};
