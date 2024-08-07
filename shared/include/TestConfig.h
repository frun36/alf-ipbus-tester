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
    bool randomiseOperations;
    bool randomiseResponse;
    std::vector<uint32_t> registers;
    std::vector<SwtSequence> sequences;
    bool splitSeq;

    TestConfig(const toml::table& tbl);
};