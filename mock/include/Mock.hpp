#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <boost/log/trivial.hpp>
#include "Memory.h"

class Mock : public ipbus::Memory {
    uint32_t* m_registers;
    size_t m_size;

    Mock(size_t size) : m_size(size), m_registers(new uint32_t[size]) {}

    ~Mock() { 
        delete [] m_registers; 
    }

    ipbus::InfoCode dataRead(uint32_t address, size_t words, uint32_t* out) const override {
        if (address + words > m_size)
            return ipbus::InfoCode::ErrorRead;
        
        std::memcpy(out, m_registers + address, words);
        return ipbus::InfoCode::Response;
    }

    ipbus::InfoCode dataWrite(uint32_t address, size_t words, const uint32_t* in) override {
        if (address + words > m_size)
            return ipbus::InfoCode::ErrorWrite;
        
        std::memcpy(m_registers + address, in, words);
        return ipbus::InfoCode::Response;
    }

    size_t getSize() const {
        return m_size;
    }
};