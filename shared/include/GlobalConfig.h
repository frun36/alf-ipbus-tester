#pragma once

#include <string>

#include "toml++/toml.hpp"

struct GlobalConfig {
    struct Exception : public std::runtime_error {
        Exception(const std::string& msg)
            : std::runtime_error(msg) {
        }
    };

    static std::string help;

    std::string name;
    std::string registerFile;
    unsigned rngSeed;

    GlobalConfig(const toml::table& tbl);
};