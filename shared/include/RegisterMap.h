#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <expected>

class RegisterMap
{
   public:
    struct Parameter {
        Parameter(std::string name,
                  uint32_t startBit,
                  uint32_t bitLength,
                  std::optional<int64_t> minValue,
                  std::optional<int64_t> maxValue,
                  bool isSigned,
                  bool isFifo,
                  bool isReadOnly)
            : name(std::move(name)),
              startBit(startBit),
              bitLength(bitLength),
              minValue(minValue),
              maxValue(maxValue),
              isSigned(isSigned),
              isFifo(isFifo),
              isReadOnly(isReadOnly) {}
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

        std::expected<void, std::string> writeValue(uint32_t newValue);
    };

    size_t size() const
    {
        return m_registers.size();
    }

    std::optional<std::reference_wrapper<const Register>> getRegisterOptional(uint32_t addr) const;
    std::optional<std::reference_wrapper<Register>> getRegisterOptional(uint32_t addr);

    std::expected<void, std::string> blockRead(uint32_t baseAddr, uint32_t words, uint32_t* out) const;
    std::expected<void, std::string> blockWrite(uint32_t baseAddr, uint32_t words, const uint32_t* in);

    static std::expected<RegisterMap, std::string> readFromFile(std::string path);

   private:
    std::unordered_map<uint32_t, Register> m_registers;

    std::expected<void, std::string> handleLine(std::string_view line);

};
