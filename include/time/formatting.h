#pragma once
#include <tuple>
#include <string>
#include <iomanip>

namespace chrono = std::chrono;

static constexpr const char* date_fmt = "%Y-%M-%D";
static constexpr const char* time_fmt = "%H:%M:%S";

template <typename Container, typename Fun>
inline void tuple_for_each(const Container& c, Fun fun)
{
    for (auto& e : c)
        fun(std::get<0>(e), std::get<1>(e), std::get<2>(e));
}
 
inline std::string format_duration(chrono::seconds time)
{
    using T = std::tuple<chrono::seconds, int, const char *>;
    
    constexpr T formats[] = {
        T{chrono::hours(1), 2, ""},
        T{chrono::minutes(1), 2, ":"},
        T{chrono::seconds(1), 2, ":"}
    };
    
    std::ostringstream o;
    tuple_for_each(formats, [&time, &o](auto denominator, auto width, auto separator) {
        o << separator << std::setw(width) << std::setfill('0') << (time / denominator);
        time = time % denominator;
    });
    return o.str();
}