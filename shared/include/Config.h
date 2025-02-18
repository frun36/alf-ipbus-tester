#pragma once

#include <exception>
#include <string>
#include <vector>

#include "GlobalConfig.h"
#include "TestConfig.h"
#include "toml++/toml.hpp"

struct Config {
    struct Exception : public std::runtime_error {
        Exception(const std::string& msg)
            : std::runtime_error(msg) {
        }
    };

    static std::string help;

    GlobalConfig global;
    std::vector<TestConfig> tests;

    static Config readFile(std::string filename);


   private:
    Config(const toml::table& tbl);
};
