#include <stdafx.h>
#include <stopwatch.h>
#include <timeformat.h>

void StopWatch::start(int initial_time){
    //printf("\nstart\n");
    m_paused = false;
    m_start = m_end = my_clock.now();
    duration<double> t(initial_time * 60);
    m_seconds = t;
}

void StopWatch::stop(){
    //printf("\nstop\n");
    if(!m_paused){
        m_paused = true;
        m_end = my_clock.now();
        m_seconds += m_end - m_start;
    }
}

void StopWatch::pause(){
    //printf("\npause\n");
    m_paused = true;
    m_end = my_clock.now();
    m_seconds += m_end - m_start;
}

void StopWatch::resume(){
    //printf("\nresume\n");
    m_paused = false;
    m_start = m_end = my_clock.now();
}

double StopWatch::elapsed_seconds(){
    if( !m_paused ){
        duration<double> duration = my_clock.now() - m_start;
        auto temp = duration + m_seconds;
        return temp.count();
    }
    return m_seconds.count();
}

double StopWatch::elapsed_hours(){
    if( !m_paused ){
        duration<double> duration = my_clock.now() - m_start;
        auto temp = duration + m_seconds;
        return temp.count() / 3600.f;
    }
    return m_seconds.count() / 3600.f;
}

std::string StopWatch::elapsed_timestamp(){
    if( !m_paused ){
        duration<double> duration = my_clock.now() - m_start;
        auto temp = std::chrono::duration_cast<std::chrono::seconds>(duration + m_seconds);
        return formatDuration(temp);
    }
    return formatDuration(std::chrono::duration_cast<std::chrono::seconds>(m_seconds));
}
