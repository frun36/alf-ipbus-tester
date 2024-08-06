#pragma once

#include "toml++/toml.hpp"
#include "swt.h"

#include <vector>
#include <cstdint>

struct TestConfig {
    std::string name;
    bool enabled;
    bool randomise_operations;
    bool randomise_response;
    std::vector<uint32_t> registers;
    SwtSequence sequence;

    TestConfig(const toml::table& tbl);
};