#include "tracking/tracker.h"
#include "time/formatting.h"
#include <filesystem>

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

int16_t Tracker::key_wait() {
    union Key {
        int16_t key = 0;
        int8_t key_bytes[2];
    };
    while(!exiting){
        Key input;
        input.key_bytes[0] = getch();
        switch(input.key_bytes[0]){
            case ERR:
                napms(500);
                continue;
            case 0:
            case -32:
                input.key_bytes[1] = getch();
            default:
                break;
        }
        return input.key;
    }
    return -1;
}

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
        short key = key_wait();
        switch (key) {
            case ' ':
                hours_file.checkin();
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
        chrono::nanoseconds session_elapsed = hours_file.clocked_in ? now - hours_file.clock_in : chrono::nanoseconds::zero();
        chrono::nanoseconds accumulated = hours_file.past + hours_file.today + session_elapsed;
        std::string session_str = duration_to_string(session_elapsed);
        std::string accumulated_str = duration_to_string(accumulated);
        chrono::duration<double, std::ratio<3600>> hours_acc = accumulated;
        double earnings = hours_acc.count() * hourly_rate;
        hours_acc = session_elapsed;
        double session_earnings = hours_acc.count() * hourly_rate;
        ScreenController::print({date_to_string(now), session_str, accumulated_str, earnings, session_earnings}, hours_file.clocked_in);
        std::this_thread::sleep_for(chrono::milliseconds(500));
    }
}
