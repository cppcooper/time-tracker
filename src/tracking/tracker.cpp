#include "tracking/tracker.h"
#include "time/formatting.h"
#include <filesystem>

#ifndef WINDOWS
  #include <ncurses.h>
#else
  #include <thread>
  #include <conio.h>
  #include <windows.h>
#endif

namespace fs = std::filesystem;
Tracker* instance = nullptr;

inline fs::path GetConfigPath() {
    std::string home_path;
#ifdef unix
    home_path = std::string(getenv("HOME")) + "/.config/";
#else //if defined(_WIN32)
    home_path = getenv("HOMEDRIVE");
    std::string path = getenv("HOMEPATH");
    home_path.append(path);
#endif
    return home_path + "time-tracker";
}

#ifdef WINDOWS
void clear_input(){
    while (_kbhit()){
        _getch();
    }
}

int16_t key_wait(){
    union Key
    {
        int16_t key;
        int8_t  key_bytes[2];
    };
    Key input;
    input.key = 0;
    for(int i = 0; i < 2; ++i){
        clear_input();
        while(!_kbhit()){}
        input.key_bytes[i] = _getch();
        switch(input.key_bytes[i]){
            case 0:
            case -32:
                break;
            default:
                return input.key;
        }
    }
    return -1;
}
#endif

Tracker::Tracker(){
    config = GetConfigPath();
    fs::create_directories(config.parent_path());
    std::fstream file(config, std::ios::in);
    std::string line;
    std::stringstream ssline;
    while(std::getline(file,line)) {
        ssline.str(line);
        if (line.find('.') == std::string::npos) {
            int dollars = 0;
            ssline >> dollars;
            hourly_rate = dollars / 100.f;
        }
    }
    instance = this;
    hours_file.load();
}

void Tracker::start() {
    ScreenController::initialize();
    printing = std::thread(&Tracker::print, this);
    while (!exiting) {
#ifndef WINDOWS
        short key = getch();
#else
        short key = key_wait();
#endif
        switch (key) {
            case ' ':
                !hours_file.clocked_in ? hours_file.clockin() : hours_file.clockout();
                break;
            case KEY_EXIT:
                shutdown();
                break;
            case ERR:
                napms(250);
                break;
        }
    }
    printing.join();
    ScreenController::deinitialize();
}

void Tracker::shutdown() {
    exiting = true;
}

void Tracker::print() {
    static time_point_sc now;
    while (!exiting) {
        now = chrono::system_clock::now();
        chrono::seconds session_elapsed = hours_file.clocked_in ? chrono::duration_cast<chrono::seconds>(now - hours_file.clock_in) : chrono::seconds::zero();
        chrono::seconds accumulated = hours_file.past + hours_file.today + session_elapsed;
        std::string session_str = duration_to_string(session_elapsed);
        std::string accumulated_str = duration_to_string(accumulated);
        chrono::duration<double, std::ratio<3600>> hours_acc = accumulated;
        double earnings = hours_acc.count() * hourly_rate;
        hours_acc = session_elapsed;
        double session_earnings = hours_acc.count() * hourly_rate;
        ScreenController::print({date_to_string(now), session_str, accumulated_str, earnings, session_earnings}, hours_file.clocked_in);
#ifndef WINDOWS
        napms(500);
#else
        std::this_thread::sleep_for(chrono::milliseconds(500));
#endif
    }
}
