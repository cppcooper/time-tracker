#pragma once
#include <chrono>
#include <string>

using namespace std::chrono;

class PunchClock
{
private:
    bool m_paused = false;
    system_clock my_clock;
    time_point<system_clock> date;
    time_point<system_clock> time;
    time_point<system_clock> m_start;
    time_point<system_clock> m_end;
    duration<double> m_seconds;

public:
    PunchClock(){
        start();
    }
    void clock_in();
    void clock_out();
    void start(int initial_time = 0);
    void stop();
    void pause();
    void resume();

    /*
    1. date
    2. clock-in
    3. clock-out
    4. repeat 2,3
    require manual clock-in/out
    create new line if line date doesn't match current date
    */
    
    double elapsed_seconds();
    double elapsed_hours();
    std::string elapsed_timestamp();
};