#pragma once
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class HoursFile {
private:
    fs::path file;
    std::fstream log;
    double _previous_seconds = 0;
    double _previous_hours = 0;
public:
    HoursFile();
    void load_past_hours();
    void save(double elapsed);
    void clear();
    void zero();
    const double &previous_seconds = _previous_seconds;
    const double &previous_hours = _previous_hours;
};
