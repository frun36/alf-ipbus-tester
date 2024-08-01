#pragma once

#include "swt.h"
#include <cstdlib>

class SequenceGenerator {
public:
    SwtSequence generateRandom(size_t operations) {
        SwtSequence seq = {};

        for (size_t i = 0; i < operations; i++) {
            SwtOperation::Type type = static_cast<SwtOperation::Type>(std::rand() % 4);

            // uint32_t address = std::rand();
            uint32_t address = 0x1004;
            uint32_t data0 = std::rand();
            uint32_t data1 = std::rand();

            seq.addOperation(SwtOperation(type, address, data0, data1));
        }
        
        return seq;
    };
};