#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Memory.h"
#include "Register.h"

class Mock : public ipbus::Memory {
private:
    size_t m_size;
    std::vector<Register> m_registers;
public:
    Mock(size_t size) : m_size(size), m_registers(size) {}

    Mock(std::string filename);

    bool verifyRegisterBlockRead(uint32_t address, size_t words) const;

    bool verifyRegisterBlockWrite(uint32_t address, size_t words) const;

    bool dataRead(uint32_t address, size_t words, uint32_t* out) const override;

    bool dataWrite(uint32_t address, size_t words, const uint32_t* in) override;

    void lock() override { }

    void unlock() override { }

    size_t getSize() const {
        return m_size;
    }
};