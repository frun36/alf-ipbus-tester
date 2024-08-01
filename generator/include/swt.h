#pragma once

#include <cstdint>
#include <string>
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

    std::string getResponse() const;
};

struct SwtSequence {
    std::vector<SwtOperation> operations;

    SwtSequence(std::initializer_list<SwtOperation> operations) : operations(operations) {}

    void addOperation(SwtOperation operation);

    std::string getRequest() const;

    std::string getResponse() const;
};