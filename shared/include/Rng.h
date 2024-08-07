#pragma once

#include <cstdint>

class Rng {
   private:
    uint32_t m_prev;
    uint32_t m_m;
    uint32_t m_a;

    Rng(uint32_t seed, uint32_t m, uint32_t a)
        : m_prev(seed), m_m(m), m_a(a) {
    }

   public:
    static Rng minstd(uint32_t seed) {
        return Rng(seed, ((uint32_t)1 << 31) - 1, 48271);
    }

    uint32_t randint(uint32_t min, uint32_t max) {
        uint32_t next = (m_a * m_prev) % m_m;

        m_prev = next;

        return (next % (1 + max - min)) + min;
    }
};