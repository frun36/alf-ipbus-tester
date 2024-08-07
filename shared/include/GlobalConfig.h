#pragma once

#include <string>

#include "toml++/toml.hpp"
#include "Register.h"
#include "Rng.h"

struct GlobalConfig {
    struct Exception : public std::runtime_error {
        Exception(const std::string& msg)
            : std::runtime_error(msg) {
        }
    };

    static std::string help;

    std::string name;
    std::string registerFile;
    std::vector<Register> registerMap;
    Rng rng;
    unsigned rngSeed;

    GlobalConfig(const toml::table& tbl);
};