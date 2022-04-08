#include <stdafx.h>
#include <tracker.h>
#include <timeformat.h>
#include <input.h>

struct delimiter_ctype : std::ctype<char> {
    static const mask* make_table(std::string delims)
    {
        // make a copy of the "C" locale table
        static std::vector<mask> v(classic_table(), classic_table() + table_size);
        for(mask m : v){
            m &= ~space;
        }
        for(char d : delims){
            v[d] |= space;
        }
        return &v[0];
    }
    delimiter_ctype(std::string delims, ::size_t refs = 0) : ctype(make_table(delims), false, refs) {}
};

int initial_time = 0;
tracker* instance = nullptr;

inline void sleep(uint16_t milli_seconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milli_seconds));
}

BOOL CtrlHandler(DWORD fdwCtrlType){
    return instance->close(fdwCtrlType);
}

tracker::tracker(int init_time){
    initial_time = init_time;
    instance = this;
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler,TRUE);
}

void tracker::load(){
    log.open("time_log.csv", std::ios::in);
    std::string line;
    std::stringstream ssline;
    ssline.imbue(std::locale(ssline.getloc(),new delimiter_ctype(",")));
    while(std::getline(log,line)){
        ssline.clear();
        ssline.str(line);
        if(line.substr(0,line.find(',')).find('.') == std::string::npos){
            int dollars = 0;
            ssline >> dollars;
            hourly_rate = dollars / 100.f;
        }
        previous_seconds = 0.0;
        double seconds;
        while(ssline >> seconds){
            previous_seconds += seconds;
        }
        previous_hours = previous_seconds / 3600;
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
    //std::stringstream buffer("\n");
    //buffer << (int)(hourly_rate * 100) << ",0.0,";
    log << "\n0.0,"; //buffer.str();
    log.close();
    previous_seconds = 0.0;
    previous_hours = 0.0;
}

BOOL tracker::close(DWORD fdwCtrlType){ 
    if(fdwCtrlType == CTRL_C_EVENT){ 
        // Handle the CTRL-C signal. 
        clock.stop();
        exiting = true;
        paused = false;
        sleep(666); //finish printing
        print();
        save();
        exit(0);
    } 
    return FALSE;
}

void tracker::track_time(){
    load();
    clock.start(initial_time);
    printing = std::thread(&tracker::print,this);
    while( !exiting ){
        short key = key_wait();
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
                previous_seconds = 0.0;
                previous_hours = 0.0;
                break;
            case 21216:
                load();
                break;
        }
    }
    printing.join();
}

void tracker::print(){
    while(!exiting){
        std::string time = formatDuration(std::chrono::duration_cast<std::chrono::seconds>(duration<double>(previous_seconds + clock.elapsed_seconds())));
        printf("\x1b[A\r                               ");
        printf("\x1b[A\r                               ");
        printf("\x1b[A\r              ");
        if (paused){
            printf("\r    - PAUSED -   ");
            if(hourly_rate < 0)
                printf("\nAccumulated Time: %s",time.c_str());
            else
                printf("\nAccumulated Time: %s - $%.2f",time.c_str(),(previous_hours + clock.elapsed_hours())*hourly_rate);
            printf("\nSession Time: %s\n",clock.elapsed_timestamp().c_str());
        }
        else{
            if(hourly_rate < 0)
                printf("\nAccumulated Time: %s",time.c_str());
            else
                printf("\nAccumulated Time: %s - $%.2f",time.c_str(),(previous_hours + clock.elapsed_hours())*hourly_rate);
            printf("\nSession Time: %s\n",clock.elapsed_timestamp().c_str());
        }
        sleep(333);
    }
}