#include "Config.h"

#include <iostream>

Config Config::readFile(std::string filename) {
    toml::table tbl;
    try {
        tbl = toml::parse_file(filename);
    } catch (const toml::parse_error& err) {
        std::cerr << "TOML parsing error:\n"
                  << err << "\n";
    }

    return Config(*tbl.as_table());
}

Config::Config(const toml::table& tbl) 
    : global(*tbl["global"].as_table()) {
    const auto& testTables = *tbl["tests"].as_array();

    testTables.for_each([this](const auto& t) { tests.emplace_back(TestConfig(*t.as_table())); });
}