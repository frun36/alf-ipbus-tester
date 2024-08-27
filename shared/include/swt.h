#pragma once

#include <boost/log/trivial.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
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
    size_t ipbusWords;

    SwtOperation(Type type, uint32_t address, uint32_t data0 = 0, uint32_t data1 = 0)
        : type(type), address(address), data0(data0), data1(data1) {
        switch(type) {
            case Type::Read:
                ipbusWords = 2;
                break;
            case Type::Write:
            case Type::RmwSum:
                ipbusWords = 3;
                break;
            case Type::RmwBits:
                ipbusWords = 4;
                break;
            default:
                ipbusWords = 0;
        }
    }

    std::string getRequest() const;

    std::string getSuccessResponse() const;
};

struct SwtSequence {
    std::vector<SwtOperation> operations;

    SwtSequence() = default;

    SwtSequence(std::initializer_list<SwtOperation> operations)
        : operations(operations) {
    }

    void addOperation(SwtOperation operation);

    void clearOperations() {
        operations.clear();
    }

    std::string getRequest() const;

    std::string getSuccessResponse() const;

    size_t getIPbusPayloadWords() const {
        size_t result = 0;
        for(const auto& op : operations)
            result += op.ipbusWords;
        return result;
    }

    static bool match(std::string expected, std::string response) {
        if (expected.size() != response.size()) {
            // BOOST_LOG_TRIVIAL(error) << "Response sizes don't match";
            return false;
        }

        for (size_t i = 0; i < expected.size(); i++) {
            if (std::tolower(expected[i]) != std::tolower(response[i]) && expected[i] != '.') {
                // BOOST_LOG_TRIVIAL(error) << "Unexpected character in response: " << response[i];
                return false;
            }
        }

        return true;
    }
};