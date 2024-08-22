#include "Register.h"

Register::Register(std::string input)
    : isRead(true), isWrite(false), data(0) {
    size_t commaPos = input.find(',');

    if (commaPos != std::string::npos) {
        std::string strPart = input.substr(commaPos + 1);

        if (strPart == "rw") {
            isWrite = true;
        }
    } else {
        std::cerr << "Register parsing failed\n";
    }
}

std::vector<Register> Register::readMapFromFile(std::string filename) {
    std::ifstream inputFile(filename);
    std::vector<Register> registers;

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open register file " << filename << '\n';
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    // Find size of address space
    size_t commaPos = line.find(',');

    if (commaPos != std::string::npos) {
        std::string hexPart = line.substr(0, commaPos);

        uint32_t address = std::stoul(hexPart, nullptr, 16);

        registers = std::vector<Register>(address + 1);
    } else {
        std::cerr << "Register parsing failed for string: " << line << "\n";
    }

    // Set register RW properties
    for (const auto& currLine : lines) {
        size_t commaPos = currLine.find(',');

        if (commaPos != std::string::npos) {
            std::string hexPart = currLine.substr(0, commaPos);

            uint32_t address = std::stoul(hexPart, nullptr, 16);

            registers[address] = Register(currLine);
        } else {
            std::cerr << "Register parsing failed for string: " << currLine << "\n";
        }
    }

    return registers;
}