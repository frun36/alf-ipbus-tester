#include "GlobalConfig.h"
#include "Config.h"

#include <iostream>
#include <optional>
#include <sstream>

std::string GlobalConfig::help = 
    "Global config parameters:\n"
    " - name [= \"No name\"]\n"
    " - register_file [= registers.csv; path relative to config file]\n"
    " - rng_seed [= 1]\n"
    " - alf = { name [= str], serial [= 0], link [= 0] }";

GlobalConfig::GlobalConfig(const toml::table& tbl) {
    name = tbl["name"].value_or<std::string>("No name");
    registerFile = tbl["register_file"].value_or<std::string>("registers.csv");
    registerMap = Register::readMapFromFile(registerFile);
    rngSeed = tbl["rng_seed"].value_or<unsigned>(1);
    rng = Rng::minstd(rngSeed);

    if(!tbl["alf"].is_table())
        throw GlobalConfig::Exception("alf must be a table");

    alf = Alf(*tbl["alf"].as_table());
}

GlobalConfig::Alf::Alf(const toml::table& tbl) {
    std::optional<std::string> nameOpt = tbl["name"].value<std::string>();

    if(!nameOpt.has_value())
        throw GlobalConfig::Exception("alf.name must be a string");

    name = nameOpt.value();
    serial = tbl["serial"].value_or<unsigned>(0);
    link = tbl["link"].value_or<unsigned>(0);

    std::stringstream ss;

    ss << name << "/SERIAL_" << serial << "/LINK_" << link << "/SWT_SEQUENCE";

    serviceName = ss.str();
}