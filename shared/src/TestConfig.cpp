#include "TestConfig.h"

TestConfig::TestConfig(const toml::table& tbl) {
    name = tbl["name"].value_or<std::string>("No name");
    enabled = tbl["enabled"].value_or<bool>(true);
    randomise_operations = tbl["randomise_operations"].value_or<bool>(false);
    randomise_response = tbl["randomise_response"].value_or<bool>(false);

    // ToDo: handle invalid arguments!
    tbl["registers"].as_array()->for_each([this](auto& r) {
        if (r.is_table()) {
            const toml::table& t = *r.as_table();
            uint32_t begin = t["begin"].value_or<uint32_t>(0);
            uint32_t end = t["end"].value_or<uint32_t>(0);

            for (uint32_t addr = begin; addr <= end; addr++)
                registers.push_back(addr);
        } else {
            auto i = *r.as_integer();
            registers.push_back(i.get());
        }
    });

    for (auto reg : registers)
        tbl["operations"].as_array()->for_each([this, reg](const auto& o) {
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
                std::cerr << "Invalid SWT operation type " << typeStr << "\n";
            }

            sequence.addOperation(SwtOperation(type, reg, data0, data1));
        });
}