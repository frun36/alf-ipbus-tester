#include "Mock.h"

bool Mock::dataRead(uint32_t address, size_t words, uint32_t* out) const
{
    if (!getCurrResponse()) {
        return false;
    }

    auto res = cfg.global.registerMap.blockRead(address, words, out);

    if (!res) {
        BOOST_LOG_TRIVIAL(error) << res.error();
        return false;
    }

    return true;
}

bool Mock::dataWrite(uint32_t address, size_t words, const uint32_t* in)
{
    if (!getCurrResponse()) {
        return false;
    }

    auto res = cfg.global.registerMap.blockWrite(address, words, in);

    if (!res) {
        BOOST_LOG_TRIVIAL(error) << res.error();
        return false;
    }

    return true;
}
