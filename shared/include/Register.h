#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct Register {
    bool isRead;
    bool isWrite;
    uint32_t data;

    Register()
        : isRead(false), isWrite(false), data(0) {
    }

    Register(std::string input);

    static std::vector<Register> readMapFromFile(std::string filename);
};