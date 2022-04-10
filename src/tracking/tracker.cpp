#include "tracking/tracker.h"
#include "tracking/timeformat.h"
#include <filesystem>

namespace fs = std::filesystem;

inline fs::path GetConfigPath() {
    std::string home_path;
#ifdef unix
    home_path = std::string(getenv("HOME")) + "/.config/";
#else //if defined(_WIN32)
    home_path = getenv("HOMEDRIVE");
    std::string path = getenv("HOMEPATH");
    home_path.append(path);
#endif
    return home_path + "time-tracking-wage";
}

int initial_time = 0;
Tracker* instance = nullptr;

Tracker::Tracker(int init_time){
    config = GetConfigPath();
    fs::create_directories(config.parent_path());
    std::fstream file(config, std::ios::in);
    std::string line;
    std::stringstream ssline;
    while(std::getline(file,line)) {
        ssline.str(line);
        if (line.substr(0, line.find(',')).find('.') == std::string::npos) {
            int dollars = 0;
            ssline >> dollars;
            hourly_rate = dollars / 100.f;
        }
    }
    initial_time = init_time;
    instance = this;
    hours_file.load_past_hours();
}

void Tracker::close(){
    exiting.store(true);
    paused = false;
}

void Tracker::track_time() {
    ScreenController::initialize();
    clock.start(initial_time);
    printing = std::thread(&Tracker::print, this);
    while (!exiting) {
        short key = key_wait();
        switch (key) {
            case ' ':
                if (paused) {
                    clock.resume();
                    paused = false;
                } else {
                    clock.pause();
                    paused = true;
                }
                break;
            case 127:
                hours_file.clear();
                break;
            case 8:
                hours_file.zero();
                break;
            case 21216:
                hours_file.load_past_hours();
                break;
        }
    }
    printing.join();
    clock.stop();
    print();
    hours_file.save(clock.elapsed_seconds());
    ScreenController::deinitialize();
}

void Tracker::print() {
    while (!exiting) {
        std::string session_time = clock.elapsed_timestamp();
        std::string accumulated_time = format_duration(std::chrono::duration_cast<std::chrono::seconds>(
                duration<double>(hours_file.previous_seconds + clock.elapsed_seconds())));
        double earnings = (hours_file.previous_hours + clock.elapsed_hours()) * hourly_rate;
        ScreenController::print({session_time, accumulated_time, earnings}, paused);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
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
