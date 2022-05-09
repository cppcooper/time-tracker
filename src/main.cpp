#include "system/captures.h"
#include "tracking/tracker.h"
#include <iostream>

int main( int argc, char* argv[] ){
    freopen("stderr.txt", "a+", stderr);
    if( CreateCaptures() ) {
        try {
            Tracker t;
            t.start();
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "\nERROR: Could not set control handler" << std::endl;
        return 1;
    }
    return 0;
}

