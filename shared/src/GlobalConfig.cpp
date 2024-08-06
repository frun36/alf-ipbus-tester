#include "GlobalConfig.h"

#include <iostream>

GlobalConfig::GlobalConfig(const toml::table& tbl) {
    name = tbl["name"].value_or<std::string>("No name");
    registerFile = tbl["register_file"].value_or<std::string>("registers.csv");
    rngSeed = tbl["rng_seed"].value_or<unsigned>(0);
}