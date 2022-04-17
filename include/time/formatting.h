#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <date/date.h>

namespace hhdate = date;
namespace chrono = std::chrono;
using time_point_sc = chrono::time_point<chrono::system_clock>;

static constexpr const char* date_format = "%F";
static constexpr const char* time_format = "%F %T";
static constexpr const char* fmt_date = "{:%F}";
static constexpr const char* fmt_time = "{:%T}";

inline std::string duration_as_clock(chrono::nanoseconds time) {
    std::ostringstream os;
    auto hours = chrono::hours(1);
    auto minutes = chrono::minutes(1);
    auto seconds = chrono::seconds(1);
    os << std::setw(2) << std::setfill('0');
    os << (time / hours) << ":";
    time = time % hours;
    os << std::setw(2) << std::setfill('0');
    os << (time / minutes) << ":";
    time = time % minutes;
    os << std::setw(2) << std::setfill('0');
    os << (time / seconds);
    time = time % seconds;
    return os.str();
}

inline std::string date_to_string(const time_point_sc time){
    return fmt::format(fmt_date, time);
}

inline std::string time_to_string(const time_point_sc time){
    return fmt::format(fmt_time, time);
}

inline std::string duration_to_string(const chrono::seconds time){
    return duration_as_clock(time);
}

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