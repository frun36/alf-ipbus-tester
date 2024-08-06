#pragma once

#include "toml++/toml.hpp"
#include "GlobalConfig.h"
#include "TestConfig.h"

#include <string>
#include <vector>

class Config {
public:
    GlobalConfig global;
    std::vector<TestConfig> tests;

    Config(const toml::table& tbl);

    static Config readFile(std::string filename);
};