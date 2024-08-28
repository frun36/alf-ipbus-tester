#include "Config.h"

#include <iostream>

std::string Config::help = 
    "Config file structure:\n"
    "[global]\n...\n\n\n"
    "[[tests]]\n...\n\n"
    "[[tests]]\n...\n\n...\n";

Config Config::readFile(std::string filename) {
    toml::table tbl;
    try {
        tbl = toml::parse_file(filename);
    } catch (const toml::parse_error& err) {
        std::cerr << "TOML parsing error:\n"
                  << err << "\n";
        throw Config::Exception("Invalid TOML file format");
    }

    try {
        Config c(*tbl.as_table());
        c.global.registerFile = filename.substr(0, filename.find_last_of("/") + 1) + c.global.registerFile;
        return c;
    } catch (const GlobalConfig::Exception& ge) {
        std::cerr << "Global config parsing exception: " << ge.what() << ".\n" << GlobalConfig::help << "\n";
        throw Config::Exception("Invalid global config");
    } catch (const TestConfig::Exception& te) {
        std::cerr << "Test config parsing exception: " << te.what() << ".\n" << TestConfig::help << "\n";
        throw Config::Exception("Invalid tests config");
    } catch (const Config::Exception& ce) {
        std::cerr << "Config parsing exception: " << ce.what() << ".\n" << Config::help << "\n";
        throw Config::Exception("Invalid config structure");
    } catch (...) {
        throw Config::Exception("Unexpected exception");
    }
}

Config::Config(const toml::table& tbl) 
    : global(*tbl["global"].as_table()) {
    if(!tbl["tests"].is_array_of_tables())
        throw Config::Exception("Tests must be an array of tables");
    
    const auto& testTables = *tbl["tests"].as_array();

    testTables.for_each([this](const auto& t) { tests.emplace_back(TestConfig(*t.as_table())); });

    for (auto& test : tests) {
        if (test.randomiseOperations)
            test.randomiseSequences(global.rng);
        if (test.randomiseResponse)
            test.randomiseResponses(global.rng);
    }
}