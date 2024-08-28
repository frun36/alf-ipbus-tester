#pragma once

#include "Config.h"

struct Tracker {
    enum class Status {
        Ok,
        Split,
        Error,
        Finished,
    };

    const Config& cfg;

    size_t currTest;
    size_t currTestRegister;
    size_t currTestRepeat;
    size_t seqId;
    size_t remaining;

    bool printInfo;

    Tracker(const Config& cfg) : cfg(cfg), currTest(0), currTestRegister(0), currTestRepeat(0), seqId(0), remaining(0), printInfo(true) { 
        while (!cfg.tests[currTest].enabled && currTest < cfg.tests.size())
            currTest++;
    }

    Status registerPacket(size_t words, bool isSuccessful);

    Status moveForwards();

    void reset() {
        currTest = 0;
        
        while (!cfg.tests[currTest].enabled && currTest < cfg.tests.size())
            currTest++;

        currTestRegister = 0;
        currTestRepeat = 0;
        remaining = 0;
        seqId = 0;
    }
};