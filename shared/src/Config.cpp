#include "Config.h"
#include <error.h>

#include <iostream>

std::string Config::help =
    "Config file structure:\n"
    "[global]\n...\n\n\n"
    "[[tests]]\n...\n\n"
    "[[tests]]\n...\n\n...\n";

Result<Config> Config::readFile(std::string filename)
{
    toml::table tbl;
    try {
        tbl = toml::parse_file(filename);
    } catch (const toml::parse_error& err) {
        std::cerr << "TOML parsing error:\n"
                  << err << "\n";
        return Err("Invalid TOML file format");
    }

    Config c;
    TRY_ASSIGN(GlobalConfig::fromToml(*tbl["global"].as_table()), c.global);
    if (tbl["tests"].is_array_of_tables()) {
        const auto& testTables = *tbl["tests"].as_array();

        for (auto it = testTables.begin(); it < testTables.end(); it++) {
            TestConfig cfg;
            TRY_ASSIGN(TestConfig::fromToml(*it->as_table()), cfg);
        }

        for (auto& test : c.tests) {
            if (test.randomiseOperations) {
                test.randomiseSequences(c.global.rng);
            }
            if (test.randomiseResponse) {
                test.randomiseResponses(c.global.rng);
            }
        }
    }
    c.global.registerFile = filename.substr(0, filename.find_last_of("/") + 1) + c.global.registerFile;
    TRY(c.global.initRegisterMap());
    return c;
}
