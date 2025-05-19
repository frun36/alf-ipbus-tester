#include "TestConfig.h"

#include <optional>

std::string TestConfig::help =
    "Test config parameters:\n"
    " - name [= \"No name\"]\n"
    " - enabled [= true]\n"
    " - randomise_operations [= false]\n"
    " - randomise_response [= false]\n"
    " - registers [required: array of u32s or regblocks { begin = u32, end = u32 }]\n"
    " - operations [required: array of { type = str, data [= [0, 0]; array of u32] }]\n"
    " - split_seq [= false]\n"
    " - repeats [= 1]\n"
    " - wait [= 0]";

Result<TestConfig> TestConfig::fromToml(const toml::table& tbl)
{
    auto name = tbl["name"].value_or<std::string>("No name");
    auto enabled = tbl["enabled"].value_or<bool>(true);
    auto randomiseOperations = tbl["randomise_operations"].value_or<bool>(false);
    auto randomiseResponse = tbl["randomise_response"].value_or<bool>(false);
    auto splitSeq = tbl["split_seq"].value_or<bool>(false);
    auto repeats = tbl["repeats"].value_or<unsigned>(1);
    auto wait = tbl["wait"].value_or<unsigned>(0);
    auto singleIPbusPayloadWords = 0;
    std::vector<uint32_t> registers;

    if (!tbl["registers"].is_array()) {
        return Err("registers must be an array");
    }

    for (auto it = tbl["registers"].as_array()->begin(); it < tbl["registers"].as_array()->end(); it++) {
        if (it->is_table()) {
            const toml::table& t = *it->as_table();
            std::optional<uint32_t> begin = t["begin"].value<uint32_t>();
            std::optional<uint32_t> end = t["end"].value<uint32_t>();

            if (!begin.has_value()) {
                return Err("Register block entry must contain begin value");
            }

            if (!end.has_value()) {
                return Err("Register block entry must contain end value");
            }

            for (uint32_t addr = begin.value(); addr <= end.value(); addr++) {
                registers.push_back(addr);
            }
        } else {
            if (!it->is_integer()) {
                return Err("Register address must be an integer");
            }

            auto i = *it->as_integer();
            registers.push_back(i.get());
        }
    }

    std::vector<SwtSequence> sequences;
    std::vector<bool> sequenceResponses;
    SwtSequence sequence;
    for (auto reg : registers) {

        if (!tbl["operations"].is_array()) {
            return Err("operations must be an array");
        }

        for (auto it = tbl["operations"].as_array()->begin(); it < tbl["operations"].as_array()->end(); it++) {
            if (!it->is_table()) {
                return Err("Invalid operations entry");
            }

            const toml::table& t = *it->as_table();

            std::string typeStr = t["type"].value_or<std::string>("");

            SwtOperation::Type type;
            uint32_t data0 = 0;
            uint32_t data1 = 0;

            if (typeStr == "read") {
                type = SwtOperation::Type::Read;
            } else if (typeStr == "write") {
                type = SwtOperation::Type::Write;

                data0 = t["data"][0].value_or<uint32_t>(0);
            } else if (typeStr == "rmw_bits") {
                type = SwtOperation::Type::RmwBits;

                data0 = t["data"][0].value_or<uint32_t>(0);
                data1 = t["data"][1].value_or<uint32_t>(0);
            } else if (typeStr == "rmw_sum") {
                type = SwtOperation::Type::RmwSum;

                data0 = t["data"][0].value_or<uint32_t>(0);
            } else if (typeStr == "block_read_incrementing") {
                type = SwtOperation::Type::BlockReadInc;

                data0 = t["data"][0].value_or<uint32_t>(0);
            } else if (typeStr == "block_read_non_incrementing") {
                type = SwtOperation::Type::BlockReadNonInc;

                data0 = t["data"][0].value_or<uint32_t>(0);
            } else {
                return Err("Invalid SWT operation type {}", typeStr);
            }

            sequence.addOperation(SwtOperation(type, reg, data0, data1));
        }

        if (splitSeq) {
            sequences.push_back(sequence);
            sequence.clearOperations();
        }
    }

    if (!splitSeq) {
        sequences.push_back(sequence);
    }

    singleIPbusPayloadWords = sequences[0].getIPbusPayloadWords();

    sequenceResponses.resize(sequences.size() * repeats);

    for (size_t i = 0; i < sequenceResponses.size(); i++) {
        sequenceResponses[i] = true;
    }

    return TestConfig(
        name,
        enabled,
        randomiseOperations,
        randomiseResponse,
        registers,
        sequences,
        sequenceResponses,
        repeats,
        wait,
        singleIPbusPayloadWords,
        splitSeq);
}

void TestConfig::randomiseSequences(Rng& rng)
{
    for (auto& seq : sequences) {
        size_t n = seq.operations.size();
        for (size_t i = 0; i < n; i++) {
            std::swap(seq.operations[i], seq.operations[rng.randint(0, n - 1)]);
        }
    }
}

void TestConfig::randomiseResponses(Rng& rng)
{
    for (size_t i = 0; i < sequenceResponses.size(); i++) {
        sequenceResponses[i] = (rng.randint(0, 4) != 0);
    }
}
