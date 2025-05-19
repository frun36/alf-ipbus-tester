#pragma once

#include <error.h>
#include <expected>
#include <string>

#include "RegisterMap.h"
#include "toml++/toml.hpp"
#include "Rng.h"

struct GlobalConfig {
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
        Alf(std::string name, unsigned int serial, unsigned int endpoint, unsigned int link, std::string serviceName)
            : name(std::move(name)), serial(serial), endpoint(endpoint), link(link), serviceName(std::move(serviceName)) {}
        static Result<Alf> fromToml(const toml::table& tbl);

        const std::string& getName() const { return name; }

        const std::string& toString() const { return serviceName; }
    };

    static std::string help;

    std::string name;
    std::string registerFile;
    RegisterMap registerMap;
    unsigned rngSeed;
    Rng rng;
    Alf alf;

    static Result<GlobalConfig> fromToml(const toml::table& tbl);

    Result<void> initRegisterMap()
    {
        TRY_ASSIGN(RegisterMap::readFromFile(registerFile), registerMap);
        return {};
    }

    GlobalConfig() = default;

    GlobalConfig(std::string name, std::string registerFile, unsigned int rngSeed, Rng rng, Alf alf)
        : name(std::move(name)), registerFile(std::move(registerFile)), rngSeed(rngSeed), rng(rng), alf(std::move(alf)) {}
};
