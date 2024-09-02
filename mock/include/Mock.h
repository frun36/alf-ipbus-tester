#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Memory.h"
#include "Register.h"
#include "Tracker.h"
#include "Config.h"

class Mock : public ipbus::Memory {
private:
public:
    Config cfg;
    Tracker trk;
    bool enableTracking;

    Mock(const Config&& cfg) : cfg(cfg), trk(cfg), enableTracking(cfg.tests.size() != 0) {
        if (!enableTracking)
            BOOST_LOG_TRIVIAL(warning) << "Test list is empty: mock tracking disabled";
    }

    bool verifyRegisterBlockRead(uint32_t address, size_t words) const;

    bool verifyRegisterBlockWrite(uint32_t address, size_t words) const;

    bool dataRead(uint32_t address, size_t words, uint32_t* out) const override;

    bool dataWrite(uint32_t address, size_t words, const uint32_t* in) override;

    void lock() override { }

    void unlock() override { }

    size_t getSize() const {
        return cfg.global.registerMap.size();
    }

    bool getCurrResponse() const {
        if(!enableTracking)
            return true;
        return cfg.tests[trk.currTest].sequenceResponses[trk.seqId];
    }
};