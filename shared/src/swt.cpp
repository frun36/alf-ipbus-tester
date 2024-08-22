#include "swt.h"

std::string SwtOperation::getRequest() const {
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

std::string SwtOperation::getSuccessResponse() const {
    std::stringstream ss;
    ss << "0";
    
    switch (type) {
        case Type::Read:
            ss << "\n0x000";
            ss << std::hex << std::setw(8) << std::setfill('0') << address;
            ss << "........";
            break;
        case Type::RmwBits:
            ss << "\n0x002";
            ss << std::hex << std::setw(8) << std::setfill('0') << address;
            ss << "........";
            ss << "\n0";
            break;
        case Type::RmwSum:
            ss << "\n0x004";
            ss << std::hex << std::setw(8) << std::setfill('0') << address;
            ss << "........";
            break;
        default:
            break;
    }

    return ss.str();
}

std::string SwtSequence::getRequest() const {
    std::string result = "reset\n";
    for (const auto& op : operations) {
        result += op.getRequest();
        result += "\n";
    }

    if (result.back() == '\n') result.pop_back();

    return result;
}

void SwtSequence::addOperation(SwtOperation operation) {
    operations.emplace_back(operation);
}

std::string SwtSequence::getSuccessResponse() const {
    std::string result = "success\n";

    for (const auto& op : operations) {
        result += op.getSuccessResponse();
        result += "\n";
    }

    return result;
}
