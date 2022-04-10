#include "system/captures.h"
#include "tracking/tracker.h"
#include <iostream>
#include <sstream>

int main( int argc, char* argv[] ){
    int initial_time = 0;
    if( argc >= 2 ){
        std::stringstream converter(argv[1]);
        converter >> initial_time;
        if( converter.fail() ){
            std::cerr << "Invalid parameter. Please enter a number for the initial time.";
            return 2;
        }
    }
    if( CreateCaptures() ) {
        Tracker t(initial_time);
        //return 0;
        t.track_time();
    }
    else {
        printf("\nERROR: Could not set control handler");
        return 1;
    }
    return 0;
}

