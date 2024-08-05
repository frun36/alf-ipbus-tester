#pragma once

#include <cstdint>
#include <string>
#include <iostream>

struct Register {
    bool isRead;
    bool isWrite;
    uint32_t data;

    Register() : isRead(false), isWrite(false), data(0) {}

    Register(std::string input) : isRead(true), isWrite(false), data(0) {
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
};