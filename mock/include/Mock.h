#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include "Memory.h"

class Mock : public ipbus::Memory {
private:
    size_t m_size;
    uint32_t* m_registers;
public:
    Mock(size_t size) : m_size(size), m_registers(new uint32_t[4*size]) {}

    ~Mock() { 
        delete [] m_registers; 
    }

    bool dataRead(uint32_t address, size_t words, uint32_t* out) const override {
        if (address + words > m_size)
            return false;
        
        std::memcpy(out, m_registers + address, words);
        return true;
    }

    bool dataWrite(uint32_t address, size_t words, const uint32_t* in) override {
        if (address + words > m_size)
            return false;
        
        std::memcpy(m_registers + address, in, words);
        return true;
    }

    void lock() override { }

    void unlock() override { }

    size_t getSize() const {
        return m_size;
    }
};