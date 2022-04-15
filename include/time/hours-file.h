#pragma once
#include <fstream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
namespace chrono = std::chrono;
using time_point_sc = chrono::time_point<chrono::system_clock>;

class HoursFile {
private:
    fs::path file;
    std::fstream log;
    time_point_sc date;
    chrono::minutes elapsed_past;
    chrono::minutes elapsed_today;
    double _previous_seconds = 0;
    double _previous_hours = 0;
public:
    HoursFile();
    void load_past_hours();
    void load();
    void save(double elapsed);
    void clear();
    void zero();
    std::string latest_date();

    const double &previous_seconds = _previous_seconds;
    const double &previous_hours = _previous_hours;
};
