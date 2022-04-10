#include "system/hours-file.h"
#include <sstream>
#include <vector>

struct delimiter_ctype : std::ctype<char> {
    static const mask* make_table(std::string delims) {
        // make a copy of the "C" locale table
        static std::vector<mask> v(classic_table(), classic_table() + table_size);
        for (mask m: v) {
            m &= ~space;
        }
        for (char d: delims) {
            v[d] |= space;
        }
        return &v[0];
    }
    explicit delimiter_ctype(std::string delims, ::size_t refs = 0) : ctype(make_table(delims), false, refs) {}
};

HoursFile::HoursFile() {
    file = fs::current_path().string() + "/time_log.csv";
}

#include <iostream>
void HoursFile::load_past_hours() {
    log.open(file, std::ios::in);
    std::string line;
    std::stringstream ssline;
    //std::cout << "opening " << file << " has been " << (log.good() ? "" : "un") << "successful" << std::endl;
    ssline.imbue(std::locale(ssline.getloc(), new delimiter_ctype(",")));
    while (std::getline(log, line)) {
        ssline.clear();
        ssline.str(line);
        _previous_seconds = 0.0;
        double seconds;
        while (ssline >> seconds) {
            //std::cout << "cell: " << seconds << std::endl;
            _previous_seconds += seconds;
        }
        _previous_hours = previous_seconds / 3600;
    }
    log.close();
}

void HoursFile::save(double elapsed) {
    log.open(file, std::ios::out | std::ios::app);
    log << elapsed << ",";
    log.close();
}

void HoursFile::clear() {
    log.open(file, std::ios::out | std::ios::app);
    log << "\n0.0,";
    log.close();
    _previous_seconds = 0.0;
    _previous_hours = 0.0;
}

void HoursFile::zero() {
    _previous_seconds = 0;
    _previous_hours = 0;
}
