#pragma once

#include <string>

#include "toml++/toml.hpp"

struct GlobalConfig {
    std::string name;
    std::string registerFile;
    unsigned rngSeed;

    GlobalConfig(const toml::table& tbl);
};