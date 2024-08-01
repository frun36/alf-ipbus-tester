#pragma once

#include <cstdint>
#include <vector>
#include <boost/log/trivial.hpp>
#include "Memory.h"

class Mock : public ipbus::Memory {
    std::vector<uint32_t> m_registers;


};