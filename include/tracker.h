#pragma once
#include "stdafx.h"
#include "stopwatch.h"

class tracker{
private:
    std::thread printing;
    std::fstream log;
    StopWatch clock;
    bool paused = false;
    bool exiting = false;
    double previous_seconds = 0;
    double previous_hours = 0;
    double hourly_rate = -1.f;

protected:
    void load();
    void save();
    void clear();

public:
    tracker(int);
    BOOL close(DWORD);
    void track_time();
    void print();
};