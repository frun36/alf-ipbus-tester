#pragma once

#include <expected>
#include <string>

#include "RegisterMap.h"
#include "toml++/toml.hpp"
#include "Rng.h"

struct GlobalConfig {
    struct Exception : public std::runtime_error {
        Exception(const std::string& msg)
            : std::runtime_error(msg)
        {
        }
    };

    class Alf
    {
       private:
        std::string name;
        unsigned serial;
        unsigned endpoint;
        unsigned link;

        std::string serviceName;

       public:
        Alf() = default;
        Alf(const toml::table& tbl);

        const std::string& getName() const { return name; }

        const std::string& toString() const { return serviceName; }
    };

    static std::string help;

    std::string name;
    std::string registerFile;
    RegisterMap registerMap;
    Rng rng;
    unsigned rngSeed;
    Alf alf;

    GlobalConfig(const toml::table& tbl);

    void initRegisterMap()
    {
        auto res = RegisterMap::readFromFile(registerFile);
        if (!res) {
            throw std::runtime_error(res.error());
        }
        registerMap = std::move(*res);
    }
};
