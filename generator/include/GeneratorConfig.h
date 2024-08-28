#pragma once

#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct GeneratorConfig {
    std::string configFilename;
    std::string logFilename;
    bool verbose;

    GeneratorConfig(int argc, const char** argv);
};