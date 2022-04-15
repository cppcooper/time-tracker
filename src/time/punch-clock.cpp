#include "time/punch-clock.h"
#include "time/formatting.h"
#include <string>
#include <fmt/format.h>
#include <chrono>
#include <fmt/chrono.h>
//#include <date/date.h>

void PunchClock::start(int initial_time){
    //printf("\nstart\n");
    m_paused = false;
    m_start = m_end = my_clock.now();
    duration<double> t(initial_time * 60);
    m_seconds = t;
}

void PunchClock::stop(){
    //printf("\nstop\n");
    if(!m_paused){
        m_paused = true;
        m_end = my_clock.now();
        m_seconds += m_end - m_start;
    }
}

void PunchClock::pause(){
    //printf("\npause\n");
    m_paused = true;
    m_end = my_clock.now();
    m_seconds += m_end - m_start;
}

void PunchClock::resume(){
    //printf("\nresume\n");
    m_paused = false;
    m_start = m_end = my_clock.now();
}

double PunchClock::elapsed_seconds(){
    if( !m_paused ){
        duration<double> duration = my_clock.now() - m_start;
        auto temp = duration + m_seconds;
        return temp.count();
    }
    return m_seconds.count();
}

double PunchClock::elapsed_hours(){
    if( !m_paused ){
        duration<double> duration = my_clock.now() - m_start;
        auto temp = duration + m_seconds;
        return temp.count() / 3600.f;
    }
    return m_seconds.count() / 3600.f;
}

std::string PunchClock::elapsed_timestamp(){
    if( !m_paused ){
        duration<double> duration = my_clock.now() - m_start;
        auto temp = std::chrono::duration_cast<std::chrono::seconds>(duration + m_seconds);
        return format_duration(temp);
    }
    return format_duration(std::chrono::duration_cast<std::chrono::seconds>(m_seconds));
}

void PunchClock::clock_in() {
    auto t = std::chrono::system_clock::now();
    std::string current_date = fmt::format(date_fmt, t);
    std::string timestamp = fmt::format(time_fmt, t);
}

void PunchClock::clock_out() {

}
