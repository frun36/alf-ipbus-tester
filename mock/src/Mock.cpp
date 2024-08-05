#include "Mock.h"

Mock::Mock(std::string filename) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
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

        m_registers = std::vector<Register>(address + 1);
    } else {
        std::cerr << "Register parsing failed for string: " << line << "\n";
    }

    // Set register RW properties
    for (const auto& currLine : lines) {
        size_t commaPos = currLine.find(',');

        if (commaPos != std::string::npos) {
            std::string hexPart = currLine.substr(0, commaPos);

            uint32_t address = std::stoul(hexPart, nullptr, 16);

            m_registers[address] = Register(currLine);
        } else {
            std::cerr << "Register parsing failed for string: " << currLine << "\n";
        }
    }
}

bool Mock::verifyRegisterBlockRead(uint32_t address, size_t words) const {
    for (size_t i = address; i < address + words; i++)
        if (!m_registers[i].isRead)
            return false;
    return true;
}

bool Mock::verifyRegisterBlockWrite(uint32_t address, size_t words) const {
    for (size_t i = address; i < address + words; i++)
        if (!m_registers[i].isWrite)
            return false;
    return true;
}

bool Mock::dataRead(uint32_t address, size_t words, uint32_t* out) const {
    if (!verifyRegisterBlockRead(address, words))
        return false;

    for (size_t i = 0; i < words; i++)
        out[i] = m_registers[address + i].data;

    return true;
}

bool Mock::dataWrite(uint32_t address, size_t words, const uint32_t* in) {
    if (!verifyRegisterBlockWrite(address, words))
        return false;

    for (size_t i = 0; i < words; i++)
        m_registers[address + i].data = in[i];

    return true;
}