#include "Tracker.h"

Tracker::Status Tracker::registerPacket(size_t words, bool isSuccessful) {
    if (words <= 1) 
        return Status::Error;
    words -= 1; // remove header

    size_t expectedSize = (remaining == 0 ? cfg.tests[currTest].singleIPbusPayloadWords : remaining);

    BOOST_LOG_TRIVIAL(trace) << "Received words: " << words << "/" << expectedSize;

    if (words == expectedSize) {
        remaining = 0;
        return moveForwards();
    } else if (words < expectedSize) {
        if (isSuccessful) {
            remaining = expectedSize - words;
            BOOST_LOG_TRIVIAL(trace) << "Split packet";
            return Status::Split;
        } else {
            BOOST_LOG_TRIVIAL(trace) << "Split simulated failure packet";
            remaining = 0;
            return moveForwards();
        }
    } else {
        return Status::Error;
    }
}

Tracker::Status Tracker::moveForwards() {
    seqId++;


    if (cfg.tests[currTest].splitSeq) {
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
        seqId = 0;

        if (currTest >= cfg.tests.size()) {
            BOOST_LOG_TRIVIAL(trace) << "All tests performed, resetting tracker";
            reset();
            return Status::Finished;
        }
    }

    return Status::Ok;
}