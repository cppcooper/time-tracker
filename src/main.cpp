#include "stdafx.h"
#include "tracker.h"

extern BOOL CtrlHandler(DWORD fdwCtrlType);

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
    if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ) ) 
    { 
        tracker t(initial_time);
        printf("\nTime Tracker\n-----------");
        printf("\n -        Space   -- pause/resume tracking.");
        printf("\n -        Insert  -- load accumulated time.");
        printf("\n -        BkSpace -- don't show accumulated time.");
        printf("\n - Ctrl + BkSpace -- reset accumulated time.");
        printf("\n - Ctrl + C       -- stop tracking and exit.\n\n\n\n\n");
        t.track_time();
    }
    else {
        printf("\nERROR: Could not set control handler");
        return 1;
    }
    return 0;
}

