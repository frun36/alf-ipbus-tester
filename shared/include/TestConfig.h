#pragma once

#include <cstdint>
#include <exception>
#include <vector>

#include "swt.h"
#include "toml++/toml.hpp"

struct TestConfig {
    struct Exception : public std::runtime_error {
        Exception(const std::string& msg)
            : std::runtime_error(msg) {
        }
    };

    static std::string help;

    std::string name;
    bool enabled;
    bool randomise_operations;
    bool randomise_response;
    std::vector<uint32_t> registers;
    SwtSequence sequence;

    TestConfig(const toml::table& tbl);
};