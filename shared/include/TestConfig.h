#pragma once

#include <error.h>
#include <cstdint>
#include <exception>
#include <vector>

#include "Rng.h"
#include "swt.h"
#include "toml++/toml.hpp"

struct TestConfig {
    TestConfig(
        std::string name,
        bool enabled,
        bool randomiseOperations,
        bool randomiseResponse,
        std::vector<uint32_t> registers,
        std::vector<SwtSequence> sequences,
        std::vector<bool> sequenceResponses,
        unsigned int repeats,
        unsigned int wait,
        size_t singleIPbusPayloadWords,
        bool splitSeq)
        : name(std::move(name)),
          enabled(enabled),
          randomiseOperations(randomiseOperations),
          randomiseResponse(randomiseResponse),
          registers(std::move(registers)),
          sequences(std::move(sequences)),
          sequenceResponses(std::move(sequenceResponses)),
          repeats(repeats),
          wait(wait),
          singleIPbusPayloadWords(singleIPbusPayloadWords),
          splitSeq(splitSeq) {}

    static std::string help;

    std::string name;
    bool enabled;
    bool randomiseOperations;
    bool randomiseResponse;
    std::vector<uint32_t> registers;
    std::vector<SwtSequence> sequences;
    std::vector<bool> sequenceResponses;
    unsigned repeats;
    unsigned wait;
    size_t singleIPbusPayloadWords;

    bool splitSeq;

    TestConfig() = default;
    static Result<TestConfig> fromToml(const toml::table& tbl);

    void randomiseSequences(Rng& rng);
    void randomiseResponses(Rng& rng);
};
