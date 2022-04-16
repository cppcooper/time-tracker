#include "time/hours-file.h"
#include <iostream>

HoursFile::HoursFile() {
    file = fs::current_path().string() + "/time_log.csv";
}

void HoursFile::load() {
    static const time_t ct(0);
    static auto offset_epoch = std::localtime(&ct);
    time_point_sc now = chrono::system_clock::now();
    std::string current_date = date_to_string(now);

    std::string line;
    std::stringstream ssline;
    ssline.imbue(std::locale(ssline.getloc(), new delimiter_ctype(",")));

    log.open(file, std::ios::in);
    while (std::getline(log, line)) {
        ssline.clear();
        ssline.str(line);

        // try to extract a date timestamp
        if (ssline >> hhdate::parse(date_format, date)) {
            date += chrono::hours(offset_epoch->tm_hour) + chrono::minutes(offset_epoch->tm_min);
            std::string line_date = date_to_string(date);

            bool has_clockin = false;
            bool is_today = current_date == line_date;
            elapsed_past += elapsed_today;
            elapsed_today = chrono::minutes::zero();
            time_point_sc start, end;
            std::string word;

            // get clock timestamps
            while (ssline >> word) {
                std::stringstream ssword(line_date + word);
                if (ssword >> hhdate::parse(time_format, has_clockin ? end : start)) {
                    if (is_today && !has_clockin) {
                        time_in = start;
                    } else if (has_clockin) {
                        elapsed_today += chrono::duration_cast<chrono::minutes>(end - start);
                    }
                    has_clockin = !has_clockin;
                }
                _clocked_in = has_clockin;
            }
        }
    }
    log.close();
}

void HoursFile::checkin() {
    auto t = std::chrono::system_clock::now();
    if(!_clocked_in) {
        time_in = t;
    }
    std::string current_date = fmt::format(fmt_date, t);
    std::string timestamp = fmt::format(fmt_time, t);
    if (_clocked_in) {
        // update elapsed today
        elapsed_today += chrono::duration_cast<chrono::minutes>(t - time_in);
    }
    _clocked_in = !_clocked_in;
    save(t);
}

void HoursFile::save(const time_point_sc &clock) {
    std::string timestamp = fmt::format(fmt_time, clock);
    auto now = chrono::system_clock::now();
    auto date_now = date_to_string(now);
    log.open(file, std::ios::out | std::ios::app);
    if(date_to_string(date) != date_now){
        date = now;
        log << "\n" << date_now;
    }
    log << "," << timestamp;
    log.close();
}
