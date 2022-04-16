#pragma once
#include "posh.h"
#include "tracking/tracker.h"
#include <csignal>

extern Tracker* instance;

inline void SigHandler(int signal) {
    switch(signal){
        case SIGINT:
        case SIGABRT:
        case SIGTERM:
            instance->shutdown();
    }
}

inline bool CreateCaptures() {
    return signal(SIGINT, SigHandler) != SIG_ERR &&
           signal(SIGABRT, SigHandler) != SIG_ERR &&
           signal(SIGTERM, SigHandler) != SIG_ERR;
}
