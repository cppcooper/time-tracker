#include "time/hours-file.h"
#include <iostream>

void HoursFile::save(const time_point_sc &clock) {
    log.open(file, std::ios::out | std::ios::app);
    log << "," << time_to_string(clock);
    log.close();
}

HoursFile::HoursFile() {
    file = fs::current_path().string() + "/time_log.csv";
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
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
        //_clocked_in = false;
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
                        time_in = start; //+ chrono::hours(offset_epoch->tm_hour) + chrono::minutes(offset_epoch->tm_min);
                    } else if (has_clockin) {
                        elapsed_today += chrono::duration_cast<chrono::minutes>(end - start);
                    }
                    has_clockin = !has_clockin;
                }
                _clocked_in = has_clockin;
            }
            if (!is_today) {
                _clocked_in = false;
            }
        }
    }
    log.close();
}
#pragma clang diagnostic pop

void HoursFile::clockin() {
    assert(!clocked_in);
    _clocked_in = true;
    time_point_sc now = std::chrono::system_clock::now();
    time_in = now;
    new_day();
    save(now);
}

void HoursFile::clockout() {
    assert(clocked_in);
    _clocked_in = false;
    time_point_sc now = std::chrono::system_clock::now();
    elapsed_today += chrono::duration_cast<chrono::minutes>(now - time_in);
    new_day();
    save(now);
}

void HoursFile::new_day() {
    time_point_sc now = std::chrono::system_clock::now();
    std::string date_now = date_to_string(now);
    if (date_now != date_to_string(date)) {
        log.open(file, std::ios::out | std::ios::app);
        if (clocked_in) {
            log << ",23:59:59";
        }
        log << "\n" << date_now;
        if (clocked_in) {
            log << ",00:00:00";
        }
        log.close();
    }
}
