#include "MockConfig.h"

#include <iostream>

MockConfig::MockConfig(int argc, const char** argv) {
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "see available options")("verbose,v", "enable verbose mode")("config_file,c", po::value<std::string>(&configFilename), "set path to config file")
        ("log_file,f", po::value<std::string>(&logFilename)->default_value(""), "specify file path for the logs. If unspecified, logs get printed to stdout");

    // Parse the provided arguments
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Handle options
    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }

    if (!vm.count("config_file")) {
        std::cerr << "Error: no config file specified\n";
        exit(1);
    }

    verbose = vm.count("verbose");
}