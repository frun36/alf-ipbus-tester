#include "GlobalConfig.h"
#include <error.h>

#include <iostream>
#include <optional>
#include <sstream>

std::string GlobalConfig::help =
    "Global config parameters:\n"
    " - name [= \"No name\"]\n"
    " - register_file [= registers.csv; path relative to config file]\n"
    " - rng_seed [= 1]\n"
    " - alf = { name [= str], serial [= 0], link [= 0] }";

Result<GlobalConfig> GlobalConfig::fromToml(const toml::table& tbl)
{
    auto name = tbl["name"].value_or<std::string>("No name");
    auto registerFile = tbl["register_file"].value_or<std::string>("registers.csv");
    auto rngSeed = tbl["rng_seed"].value_or<unsigned>(1);
    auto rng = Rng::minstd(rngSeed);

    if (!tbl["alf"].is_table()) {
        return Err("alf must be a table");
    }

    Alf alf;
    TRY_ASSIGN(Alf::fromToml(*tbl["alf"].as_table()), alf);

    return GlobalConfig(name, registerFile, rngSeed, rng, alf);
}

Result<GlobalConfig::Alf> GlobalConfig::Alf::fromToml(const toml::table& tbl)
{
    std::optional<std::string> nameOpt = tbl["name"].value<std::string>();

    if (!nameOpt.has_value()) {
        return Err("alf.name must be a string");
    }

    auto name = nameOpt.value();
    auto serial = tbl["serial"].value_or<unsigned>(0);
    auto endpoint = tbl["endpoint"].value_or<unsigned>(0);
    auto link = tbl["link"].value_or<unsigned>(0);

    std::stringstream ss;

    ss << name << "/SERIAL_" << serial << "/ENDPOINT_" << endpoint << "/LINK_" << link << "/SWT_SEQUENCE";

    auto serviceName = ss.str();

    return Alf(name, serial, endpoint, link, serviceName);
}
