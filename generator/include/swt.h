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
    
    std::string getRequest() const {
        std::stringstream ss;

        ss << "0x00";
        switch (type) {
            case Type::Read:
                ss << "0";
                break;
            case Type::Write:
                ss << "1";
                break;
            case Type::RmwBits:
                ss << "2";
                break;
            case Type::RmwSum:
                ss << "4";
                break;
            default:
                ss << "5";
        }

        ss << std::hex << std::setw(8) << std::setfill('0') << address;
        ss << std::hex << std::setw(8) << std::setfill('0') << data0;
        ss << ",write";
        if (type == Type::Read || type == Type::RmwBits || type == Type::RmwSum) {
            ss << "\nread";
        }

        if (type == Type::RmwBits) {
            ss << "\n0x003";
            ss << std::hex << std::setw(8) << std::setfill('0') << address;
            ss << std::hex << std::setw(8) << std::setfill('0') << data1;
            ss << ",write";
        }

        return ss.str();
    }

    std::string getResponse() const {
        std::stringstream ss;
        ss << "0\n";
        
        switch (type) {
            case Type::Read:
                ss << "0x000";
                ss << std::hex << std::setw(8) << std::setfill('0') << address;
                ss << "........";
                break;
            case Type::RmwBits:
                ss << "0x002";
                ss << std::hex << std::setw(8) << std::setfill('0') << address;
                ss << "........";
                ss << "\n0";
                break;
            case Type::RmwSum:
                ss << "0x004";
                ss << std::hex << std::setw(8) << std::setfill('0') << address;
                ss << "........";
                break;
        }

        return ss.str();
    }
};

struct SwtSequence {
    std::vector<SwtOperation> operations;

    SwtSequence(std::initializer_list<SwtOperation> operations) : operations(operations) {}

    void addOperation(SwtOperation operation) {
        operations.emplace_back(operation);
    }

    std::string getRequest() const {
        std::string result = "reset\n";
        for (const auto& op : operations) {
            result += op.getRequest();
            result += "\n";
        }

        if (result.back() == '\n') result.pop_back();

        return result;
    }

    std::string getResponse() const {
        std::string result = "success\n";

        for (const auto& op : operations) {
            result += op.getResponse();
            result += "\n";
        }

        return result;
    }
};