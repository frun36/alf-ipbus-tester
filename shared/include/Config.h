#pragma once

#include <error.h>
#include <string>
#include <vector>

#include "GlobalConfig.h"
#include "TestConfig.h"
#include "toml++/toml.hpp"

struct Config {
    static std::string help;

    GlobalConfig global;
    std::vector<TestConfig> tests;

    static Result<Config> readFile(std::string filename);
};
