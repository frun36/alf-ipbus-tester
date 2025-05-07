#pragma once

#include <cstdint>
#include <fstream>
#include <list>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <expected>
#include <format>
#include <vector>

struct Parameter {
    const std::string name;
    const uint32_t startBit;
    const uint32_t bitLength;
    const std::optional<int64_t> minValue;
    const std::optional<int64_t> maxValue;
    const bool isSigned;
    const bool isFifo;
    const bool isReadOnly;
};

struct Register {
    std::list<Parameter> parameters;
    uint32_t value;

    std::expected<void, std::string> writeValue(uint32_t newValue)
    {
        for (auto&& param : parameters) {
            uint32_t tmp = newValue >> param.startBit;
            uint64_t mask = (1 << param.bitLength) - 1;
            int64_t paramValue = tmp & mask;
            if (param.isSigned && (paramValue & (1 << (param.bitLength - 1)))) {
                paramValue -= (1 << param.bitLength);
            }

            if (param.minValue) {
                if (paramValue < *param.minValue) {
                    return std::unexpected(std::format("{}: value {} is smaller than min value ({})", param.name, paramValue, *param.minValue));
                }
            }

            if (param.maxValue) {
                if (paramValue > *param.maxValue) {
                    return std::unexpected(std::format("{}: value {} is larger than max value ({})", param.name, paramValue, *param.maxValue));
                }
            }
        }

        value = newValue;
        return {};
    }
};

class RegisterMap
{
   private:
    std::unordered_map<uint32_t, Register> m_registers;

    std::expected<void, std::string> handleLine(std::string_view line)
    {
        // base_addr,param_name,start_bit,end_bit,reg_block_size,min_value,max_value,is_signed,is_fifo,is_read_only
        auto parts = line | std::views::split(',');
        std::vector<std::string_view> tokens;
        tokens.reserve(10);
        for (auto part : parts) {
            tokens.emplace_back(part.begin(), part.end());
        }

        if (tokens.size() != 10) {
            return std::unexpected("Invalid line in register file");
        }

        try {
            uint32_t baseAddr = std::stoul(std::string{ tokens[0] }, nullptr, 16);
            std::string paramName{ tokens[1] };
            uint32_t startBit = std::stoul(std::string{ tokens[2] }, nullptr);
            uint32_t endBit = std::stoul(std::string{ tokens[3] }, nullptr);

            std::optional<int64_t> minValue =
                tokens[5].empty()
                    ? std::nullopt
                    : std::make_optional<int64_t>(std::stol(std::string{ tokens[5] }, nullptr));

            std::optional<int64_t> maxValue =
                tokens[6].empty()
                    ? std::nullopt
                    : std::make_optional<int64_t>(std::stol(std::string{ tokens[6] }, nullptr));

            bool isSigned = tokens[7] == "Y";
            bool isFifo = tokens[8] == "Y";
            bool isReadOnly = tokens[9] == "Y";

            m_registers[baseAddr].parameters.emplace_back(baseAddr, startBit, endBit - startBit + 1, minValue, maxValue, isSigned, isFifo, isReadOnly);
        } catch (...) {
            return std::unexpected("Invalid line in register file");
        }

        return {};
    }

   public:
    static std::expected<RegisterMap, std::string> readFromFile(std::string path)
    {
        RegisterMap map;

        std::ifstream f(path);
        if (!f.is_open()) {
            return std::unexpected(std::format("Couldn't open register file {}", path));
        }

        std::string line;
        while (std::getline(f, line)) {
            if (line.empty()) {
                continue;
            }

            auto res = map.handleLine(line);
            if (!res) {
                return std::unexpected(res.error());
            }
        }

        return map;
    }
};
