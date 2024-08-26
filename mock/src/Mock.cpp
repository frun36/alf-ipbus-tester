#include "Mock.h"

bool Mock::verifyRegisterBlockRead(uint32_t address, size_t words) const {
    for (size_t i = address; i < address + words; i++)
        if (!cfg.global.registerMap[i].isRead)
            return false;
    return true;
}

bool Mock::verifyRegisterBlockWrite(uint32_t address, size_t words) const {
    for (size_t i = address; i < address + words; i++)
        if (!cfg.global.registerMap[i].isWrite)
            return false;
    return true;
}

bool Mock::dataRead(uint32_t address, size_t words, uint32_t* out) const {
    if(!getCurrResponse())
        return false;

    if (!verifyRegisterBlockRead(address, words))
        return false;

    for (size_t i = 0; i < words; i++)
        out[i] = cfg.global.registerMap[address + i].data;

    return true;
}

bool Mock::dataWrite(uint32_t address, size_t words, const uint32_t* in) {
    if(!getCurrResponse())
        return false;

    if (!verifyRegisterBlockWrite(address, words))
        return false;

    for (size_t i = 0; i < words; i++)
        cfg.global.registerMap[address + i].data = in[i];

    return true;
}