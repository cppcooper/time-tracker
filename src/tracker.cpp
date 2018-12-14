#include "../include/tracker.h"
#include "../include/timeformat.h"
#include "../include/key_wait.h"
#include <cstdio>
#include <string>
#include <thread>
#include <cstdlib>
#include <cstdint>
#include <sstream>
#include <iostream>

int initial_time = 0;
tracker* instance = nullptr;

inline void sleep(uint16_t milli_seconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milli_seconds));
}

BOOL CtrlHandler(DWORD fdwCtrlType){
    return instance->close(fdwCtrlType);
}

void tracker::load(){
    log.open("time_log.csv", std::ios::in);
    std::string line;
    while(std::getline(log,line)){
        std::stringstream ssline(line);
        std::string word;
        previous_time = 0.0;
        while(std::getline(ssline,word,',')){
            std::stringstream token(word);
            double seconds = 0.0;
            token >> seconds;
            if(token.bad()){
                throw std::exception("token conversion to double failed");
            }
            previous_time += seconds;
        }
    }
    log.close();
}

void tracker::save(){
    log.open("time_log.csv", std::ios::out | std::ios::app);
    log << clock.elapsed_seconds() << ",";
    log.close();
    //printf("-SAVED- %f", clock.elapsed_seconds());
}

void tracker::clear(){
    log.open("time_log.csv", std::ios::out | std::ios::app);
    log << "\n0.0,";
    log.close();
    previous_time = 0.0;
}

tracker::tracker(int init_time){
    initial_time = init_time;
    instance = this;
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler,TRUE);
}

BOOL tracker::close(DWORD fdwCtrlType){ 
    if(fdwCtrlType == CTRL_C_EVENT){ 
        // Handle the CTRL-C signal. 
        clock.stop();
        loop = true;
        sleep(1000); //finish printing, catch on loop
        paused = false;
        print();
        save();
        exit(0);
    } 
    return FALSE;
}

void tracker::track_time(){
    load();
    clock.start(initial_time);
    auto capcall = [&](){
        print();
    };
    auto callback = [](void* arg){
        (*static_cast<decltype(capcall)*>(arg))();
    };
    while( true ){
        short key = key_wait(callback,&capcall);
        switch( key ){
            case ' ':
                if( paused ){
                    clock.resume();
                    paused = false;
                }
                else{
                    clock.pause();
                    paused = true;
                }
                break;
            case 127:
                clear();
                break;
            case 8:
                previous_time = 0.0;
                break;
            case 21216:
                load();
                break;
        }
        while(loop){}
    }
}

void tracker::print(){
    std::string time = formatDuration(std::chrono::duration_cast<std::chrono::seconds>(duration<double>(previous_time + clock.elapsed_seconds())));
    printf("\x1b[A\r                               ");
    printf("\x1b[A\r                               ");
    printf("\x1b[A\r              ");
    if (paused){
        printf("\r    - PAUSED -   ");
        printf("\nAccumulated Time: %s",time.c_str());
        printf("\nSession Time: %s\n",clock.elapsed_timestamp().c_str());
        sleep(25);
    }
    else{
        printf("\nAccumulated Time: %s",time.c_str());
        printf("\nSession Time: %s\n",clock.elapsed_timestamp().c_str());
        sleep(25);
    }
}