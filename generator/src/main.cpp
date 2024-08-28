#include <dim/dic.hxx>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <boost/log/trivial.hpp>

#include "swt.h"
#include "RpcInfo.h"
#include "SequenceGenerator.h"
#include "Register.h"
#include "Config.h"
#include "logging.h"
#include "GeneratorConfig.h"

int main(int argc, const char** argv) {
    GeneratorConfig genCfg(argc, argv);
    
    logging::init(genCfg.logFilename);

    std::mutex mtx;
    std::condition_variable cv;
    bool isDataReceived = false;
    std::string receivedData = "";

    RpcInfo info(mtx, cv, isDataReceived, receivedData);

    try {
        Config cfg = Config::readFile(genCfg.configFilename);

        std::string rpcInName =  cfg.global.alf.toString() + "/RpcIn";

        for(const auto& test : cfg.tests) {
            size_t seqId = 0;

            if(!test.enabled) {
                BOOST_LOG_TRIVIAL(warning) << "Test \"" << test.name << "\" is disabled";
                continue;
            }
            
            BOOST_LOG_TRIVIAL(info) << "Performing test \"" << test.name << "\"";

            for(size_t i = 0; i < test.repeats; i++) {
                for(const auto& seq : test.sequences) {    
                    std::string seqStr = seq.getRequest();

                    BOOST_LOG_TRIVIAL(debug) << "Sending data:\n" << seqStr;
                    DimClient::sendCommand(rpcInName.c_str(), seqStr.c_str());

                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

                    // Process the received data
                    BOOST_LOG_TRIVIAL(debug) << "Received data:\n" << receivedData << "\n";

                    isDataReceived = false;
                    bool result = SwtSequence::match(seq.getSuccessResponse(), receivedData);
                    if (result == test.sequenceResponses[seqId])
                        BOOST_LOG_TRIVIAL(info) << "Success";
                    else {
                        BOOST_LOG_TRIVIAL(error) << "Failure (seqId = " << seqId << ", expected " << test.sequenceResponses[seqId] << ")";
                        exit(1);
                    }
                    seqId++;
                }
                usleep(test.wait);
            }
        }
    } catch (const Config::Exception& ce) {
        BOOST_LOG_TRIVIAL(fatal) << "Failed to parse config file: " << ce.what();
        exit(1);
    }

    return 0;
}
