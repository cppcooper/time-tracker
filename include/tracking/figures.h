#pragma once
#include <string>

struct Figures {
    std::string date;
    std::string session_time = 0;
    std::string accumulated_time = 0;
    double earnings = -1.f;
    double session_earnings = -1.f;
};