#pragma once

#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct MockConfig {
    std::string configFilename;
    std::string logFilename;
    bool verbose;

    MockConfig(int argc, const char** argv);
};