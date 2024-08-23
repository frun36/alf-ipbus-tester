#pragma once

#include "Config.h"

struct Tracker {
    const Config& cfg;

    size_t currTest;
    size_t currTestRegister;
    size_t currTestRepeat;
    size_t remaining;

    Tracker(const Config& cfg) : cfg(cfg), currTest(0), currTestRegister(0), currTestRepeat(0), remaining(0) { 
        while (!cfg.tests[currTest].enabled && currTest < cfg.tests.size())
            currTest++;
    }

    bool registerPacket(size_t words) {
        if (words <= 1) 
            return false;
        words -= 1; // remove header

        size_t expectedSize = (remaining == 0 ? cfg.tests[currTest].singleIPbusPayloadWords : remaining);

        BOOST_LOG_TRIVIAL(trace) << "Received words: " << words << "/" << expectedSize;

        if (words == expectedSize) {
            return moveForwards();
        } else if (words < expectedSize) {
            remaining = words - expectedSize;
            return true;
        } else {
            return false;
        }
    }

    bool moveForwards() {
        if (cfg.tests[currTest].splitSeq) {
            BOOST_LOG_TRIVIAL(trace) << "Moving to next register";
            currTestRegister++;
            if(currTestRegister >= cfg.tests[currTest].registers.size()) {
                BOOST_LOG_TRIVIAL(trace) << "All registers complete: moving to next repeat";
                currTestRegister = 0;
                currTestRepeat++;
            }
        } else {
            BOOST_LOG_TRIVIAL(trace) << "Moving to next repeat";
            currTestRepeat++;
        }

        if (currTestRepeat >= cfg.tests[currTest].repeats) {
            BOOST_LOG_TRIVIAL(trace) << "All repeats performed";
            currTestRepeat = 0;
            do {
                currTest++;
            } while (currTest < cfg.tests.size() && !cfg.tests[currTest].enabled);

            if (currTest >= cfg.tests.size()) {
                BOOST_LOG_TRIVIAL(trace) << "All tests performed";
                currTest = 0;
                return false;
            }
        }

        return true;
    }
};