#pragma once
#include "stdafx.h"

using namespace std::chrono;

class StopWatch
{
private:
    bool m_paused = false;
    system_clock my_clock;
    time_point<system_clock> m_start;
    time_point<system_clock> m_end;
    duration<double> m_seconds;

public:
    StopWatch(){
        start();
    }
    void start(int initial_time = 0);
    void stop();
    void pause();
    void resume();
    
    double elapsed_seconds();
    std::string elapsed_timestamp();
};