#include "TestConfig.h"

TestConfig::TestConfig(const toml::table& tbl) {
    name = tbl["name"].value_or<std::string>("No name");
    enabled = tbl["enabled"].value_or<bool>(true);
    randomise_operations = tbl["randomise_operations"].value_or<bool>(false);
    randomise_response = tbl["randomise_response"].value_or<bool>(false);
}