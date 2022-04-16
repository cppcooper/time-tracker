#include "system/captures.h"
#include "tracking/tracker.h"
#include <iostream>

int main( int argc, char* argv[] ){
    if( CreateCaptures() ) {
        Tracker t;
        t.start();
    }
    else {
        printf("\nERROR: Could not set control handler");
        return 1;
    }
    return 0;
}

