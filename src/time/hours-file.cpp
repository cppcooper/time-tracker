#include "time/hours-file.h"
#include <iostream>
#include <regex>

chrono::minutes get_utc() {
    time_t rawtime = time(NULL);
    struct tm *ptm = gmtime(&rawtime);
    time_t gmt = mktime(ptm);
    ptm = localtime(&rawtime);
    time_t offset = rawtime - gmt + (ptm->tm_isdst ? 3600 : 0);
    return chrono::minutes(offset/60);
}

// if you prefer the namespaced version
std::string trim(std::string s) {
    std::regex e("^\\s+|\\s+$"); // remove leading and trailing spaces
    return std::regex_replace(s, e, "");
}

void trim_file(const fs::path &path) {
    std::fstream file(path);
    if (file) {
        file.seekg(0, std::ios::end);
        uint32_t file_size = file.tellg();
        char* buffer = new char[file_size];
        file.seekg(0);
        file.read(buffer, file_size);
        std::string contents = trim(buffer);
        file.seekp(0);
        file.write(contents.c_str(), contents.size());
        file.close();
        delete[] buffer;
    }
}

void HoursFile::append(const time_point_sc &clock) {
    log << "," << time_to_string(clock);
}

HoursFile::HoursFile() {
    file = fs::current_path().string() + "/time_log.csv";
    date = chrono::system_clock::now();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
void HoursFile::load() {
    trim_file(file);
    static const chrono::minutes utc_offset = get_utc();

    std::string line;
    std::stringstream ssline;
    ssline.imbue(std::locale(ssline.getloc(), new delimiter_ctype(",")));

    log.open(file, std::ios::in);
    // read each line of the time_log.csv of the working directory
    while (std::getline(log, line)) {
        // load the line string into a stream
        ssline.clear();
        ssline.str(line);
        //_clocked_in = false;

        time_point_sc line_date;
        // try to extract a date timestamp
        if (ssline >> hhdate::parse(date_format, line_date)) {
            // if we're clocked in, and we just parsed a new date... the file is malformed
            if (_clocked_in) {
                throw std::ios_base::failure(
                        "A problem was encountered in time_log.csv wherein the date changed while clocked in."
                        "A clock-out timestamp is needed before a new date is started.");
            }
            /* from `date` we subtract the utc offset because the offset was already applied
             * twice (technically; once when we saved, again when we parsed from file just now)
             * it would be nice if parsing presumed the time/date to already be local, but it doesn't*/
            line_date -= utc_offset;

            // variables for line parsing
            bool is_today = date_to_string(date) == date_to_string(line_date);
            std::string word;
            chrono::minutes start, end;

            // update elapsed counters
            elapsed_past += elapsed_today;
            elapsed_today = chrono::minutes::zero();
            bool has_clockin = false;
            // get clock timestamps
            while (ssline >> word) {
                std::stringstream ssword(word);
                // read timestamps as durations, subtract them for the elapsed
                if (ssword >> hhdate::parse(time_format, !has_clockin ? start : end)) {
                    if(is_today && !has_clockin){
                        // if this is today's date, we're going to set time_in
                        time_in = line_date + start; // we need to use line_date because of the utc doubling problem
                    } else if (has_clockin) {
                        // if clocked in we need to update elapsed
                        elapsed_today += chrono::duration_cast<chrono::minutes>(end - start);
                    }
                    has_clockin = !has_clockin;
                }
            }
            if (is_today) {
                has_today = true;
                _clocked_in = has_clockin;
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
    date = now;

    if (!fs::exists(file)) {
        log.open(file, std::ios::out);
    } else {
        trim_file(file);
        log.open(file, std::ios::out | std::ios::app);
    }
    if(!has_today) {
        has_today = true;
        log << date_to_string(now);
    }
    append(now);
    log.close();
}

void HoursFile::clockout() {
    assert(clocked_in);
    _clocked_in = false;
    time_point_sc now = std::chrono::system_clock::now();
    std::string date_now = date_to_string(now);
    elapsed_today += chrono::duration_cast<chrono::minutes>(now - time_in);

    // we gotta make sure appending to the file doesn't result in a malformed file, so we trim trailing whitespace
    trim_file(file);
    log.open(file, std::ios::out | std::ios::app);

    // if we clocked in yesterday, we gotta do special stuff
    // todo: make this not break if someone works for 50 hours straight or something ridiculous
    if(date_to_string(time_in) != date_now) {
        // we're going to add a clockout just before midnight and clockin just after, so the file can be parsed correctly
        log << ",23:59:59\n" << date_now << ",00:00:00";
    }
    // finally, we now append the current clockout time
    append(now);
    log.close();
}
