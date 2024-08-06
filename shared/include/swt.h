#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

struct SwtOperation {
    enum class Type {
        Read,
        Write,
        RmwBits,
        RmwSum,
    };

    Type type;
    uint32_t address;
    uint32_t data0;
    uint32_t data1;

    SwtOperation(Type type, uint32_t address, uint32_t data0 = 0, uint32_t data1 = 0) 
     : type(type), address(address), data0(data0), data1(data1) {}
    
    std::string getRequest() const;

    std::string getSuccessResponse() const;
};

struct SwtSequence {
    std::vector<SwtOperation> operations;

    SwtSequence() = default;

    SwtSequence(std::initializer_list<SwtOperation> operations) : operations(operations) {}

    void addOperation(SwtOperation operation);

    std::string getRequest() const;

    std::string getSuccessResponse() const;

    static bool match(std::string expected, std::string response) {
        if(expected.size() != response.size()) {
            std::cerr << "Response sizes don't match";
            return false;
        }

        for(size_t i = 0; i < expected.size(); i++) {
            if(std::tolower(expected[i]) != std::tolower(response[i]) && expected[i] != '.') {
                std::cerr << "Unexpected character in response: " << response[i];
                return false;
            }
        }

        return true;
    }
};