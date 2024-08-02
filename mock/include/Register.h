#pragma once

#include <cstdint>
#include <string>
#include <iostream>

struct Register {
    bool isRead = false;
    bool isWrite = false;
    uint32_t data = 0;

    Register() = default;

    Register(std::string input) {
        size_t commaPos = input.find(',');

        if (commaPos != std::string::npos) {
            std::string strPart = input.substr(commaPos + 1);

            isRead = true;

            if (strPart == "rw") {
                isWrite = true;
            }
        } else {
            std::cerr << "Register parsing failed\n";
        }
    }
};