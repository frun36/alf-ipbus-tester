#include "TestConfig.h"
#include "Config.h"

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

TestConfig::TestConfig(const toml::table& tbl) : singleIPbusPayloadWords(0) {
    name = tbl["name"].value_or<std::string>("No name");
    enabled = tbl["enabled"].value_or<bool>(true);
    randomiseOperations = tbl["randomise_operations"].value_or<bool>(false);
    randomiseResponse = tbl["randomise_response"].value_or<bool>(false);
    splitSeq = tbl["split_seq"].value_or<bool>(false);
    repeats = tbl["repeats"].value_or<unsigned>(1);
    wait = tbl["wait"].value_or<unsigned>(0);

    if(!tbl["registers"].is_array())
        throw TestConfig::Exception("registers must be an array");

    // ToDo: handle invalid arguments!
    tbl["registers"].as_array()->for_each([this](auto& r) {
        if (r.is_table()) {
            const toml::table& t = *r.as_table();
            std::optional<uint32_t> begin = t["begin"].value<uint32_t>();
            std::optional<uint32_t> end = t["end"].value<uint32_t>();

            if(!begin.has_value())
                throw TestConfig::Exception("Register block entry must contain begin value");
            
            if(!end.has_value())
                throw TestConfig::Exception("Register block entry must contain end value");

            for (uint32_t addr = begin.value(); addr <= end.value(); addr++)
                registers.push_back(addr);
        } else {
            if(!r.is_integer())
                throw TestConfig::Exception("Register address must be an integer");

            auto i = *r.as_integer();
            registers.push_back(i.get());
        }
    });

    SwtSequence sequence;
    for (auto reg : registers) {

        if(!tbl["operations"].is_array())
            throw TestConfig::Exception("operations must be an array");

        tbl["operations"].as_array()->for_each([this, reg, &sequence](const auto& o) {
            if(!o.is_table())
                throw TestConfig::Exception("Invalid operations entry");
            
            const toml::table& t = *o.as_table();

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
            } else {
                throw TestConfig::Exception("Invalid SWT operation type " + typeStr);
            }

            sequence.addOperation(SwtOperation(type, reg, data0, data1));
        });

        if (splitSeq) {
            sequences.push_back(sequence);
            sequence.clearOperations();
        }
    }

    if(!splitSeq)
        sequences.push_back(sequence);

    singleIPbusPayloadWords = sequences[0].getIPbusPayloadWords();
}

void TestConfig::randomiseSequences(Rng& rng) {
    for(auto& seq : sequences) {
        size_t length = seq.operations.size();
        std::vector<SwtOperation> newOperations;
        for(size_t i = 0; i < length; i++)
            newOperations.push_back(seq.operations[rng.randint(0, length - 1)]);
        
        seq.operations = newOperations;
    }
}

bool TestConfig::shouldSequenceSucceed(Rng& rng) const {
    return !(randomiseResponse && rng.randint(0, 4) == 0);
}