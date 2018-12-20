#include <thread>
#include <fstream>
#include <windows.h>
#include "stopwatch.h"

class tracker{
private:
    std::thread printing;
    std::fstream log;
    StopWatch clock;
    bool paused = false;
    bool exiting = false;
    double previous_time = 0;

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