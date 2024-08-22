#include "GlobalConfig.h"
#include "Config.h"

#include <iostream>

std::string GlobalConfig::help = 
    "Global config parameters:\n"
    " - name [= \"No name\"]\n"
    " - register_file [= registers.csv; path relative to config file]\n"
    " - rng_seed [= 1]\n";

GlobalConfig::GlobalConfig(const toml::table& tbl) {
    name = tbl["name"].value_or<std::string>("No name");
    registerFile = tbl["register_file"].value_or<std::string>("registers.csv");
    registerMap = Register::readMapFromFile(registerFile);
    rngSeed = tbl["rng_seed"].value_or<unsigned>(1);
    rng = Rng::minstd(rngSeed);
}