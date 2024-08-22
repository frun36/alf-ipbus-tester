#include "Mock.h"

Mock::Mock(std::string filename) {
    m_registers = Register::readMapFromFile(filename);
    m_size = m_registers.size();
}

bool Mock::verifyRegisterBlockRead(uint32_t address, size_t words) const {
    for (size_t i = address; i < address + words; i++)
        if (!m_registers[i].isRead)
            return false;
    return true;
}

bool Mock::verifyRegisterBlockWrite(uint32_t address, size_t words) const {
    for (size_t i = address; i < address + words; i++)
        if (!m_registers[i].isWrite)
            return false;
    return true;
}

bool Mock::dataRead(uint32_t address, size_t words, uint32_t* out) const {
    if (!verifyRegisterBlockRead(address, words))
        return false;

    for (size_t i = 0; i < words; i++)
        out[i] = m_registers[address + i].data;

    return true;
}

bool Mock::dataWrite(uint32_t address, size_t words, const uint32_t* in) {
    if (!verifyRegisterBlockWrite(address, words))
        return false;

    for (size_t i = 0; i < words; i++)
        m_registers[address + i].data = in[i];

    return true;
}