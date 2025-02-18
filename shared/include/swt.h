#pragma once

#include <boost/log/trivial.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include "../../IPbus/inc/IPbusPacket.h"

struct SwtOperation {
    enum class Type {
        Read,
        Write,
        RmwBits,
        RmwSum,
        BlockReadInc,
        BlockReadNonInc,
    };

    Type type;
    uint32_t address;
    uint32_t data0;
    uint32_t data1;
    size_t ipbusRequestWords;

    SwtOperation(Type type, uint32_t address, uint32_t data0 = 0, uint32_t data1 = 0)
        : type(type), address(address), data0(data0), data1(data1) {
        switch(type) {
            case Type::Read:
                ipbusRequestWords = 2;
                break;
            case Type::Write:
            case Type::RmwSum:
                ipbusRequestWords = 3;
                break;
            case Type::RmwBits:
                ipbusRequestWords = 4;
                break;
            case Type::BlockReadInc:
            case Type::BlockReadNonInc:
                ipbusRequestWords = getBlockReadRequestWords(data0);
                break;
            default:
                ipbusRequestWords = 0;
        }
    }

    uint32_t getBlockReadRequestWords(int64_t regBlockSize) {
        uint32_t maxPayload = ipbus::maxPacket - 3;
        std::array<uint32_t, 2> maxPayloads;
        maxPayloads[0] = maxPayload / 2;
        maxPayloads[1] = maxPayload - maxPayloads[0];

        uint32_t count = 0;
        while(regBlockSize > 0) {
            regBlockSize -= maxPayloads[count % 2];
            count++;
        }
        return 2 * count;
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
            result += op.ipbusRequestWords;
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
