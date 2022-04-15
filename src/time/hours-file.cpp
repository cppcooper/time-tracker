#include "time/hours-file.h"
#include "time/formatting.h"

#include <date/date.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <chrono>
#include <vector>
#include <string>

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

namespace dt = date;
void HoursFile::load() {
    time_point_sc t = chrono::system_clock::now();
    std::string current_date = fmt::format(date_fmt, t);
    std::string timestamp = fmt::format(time_fmt, t);
    elapsed_past = chrono::minutes::zero();
    elapsed_today = chrono::minutes::zero();

    log.open(file, std::ios::in);
    std::string line;
    std::stringstream ssline;
    ssline.imbue(std::locale(ssline.getloc(), new delimiter_ctype(",")));
    while (std::getline(log, line)) {
        ssline.clear();
        ssline.str(line);
        if (!dt::from_stream(ssline, date_fmt, date)) {
            continue;
        }
        std::string line_date = fmt::format(date_fmt, date);
        time_point_sc time_in;
        time_point_sc time_out;
        chrono::minutes &elapsed = current_date == line_date ? elapsed_today : elapsed_past;
        bool odd = true;
        while (dt::from_stream(ssline, time_fmt, odd ? time_in : time_out)) {
            if (!odd) {
                elapsed += chrono::duration_cast<chrono::minutes>(time_out - time_in);
            }
            odd = !odd;
        }
    }

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
