#include <dim/dic.hxx>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <boost/log/trivial.hpp>
#include <chrono>

#include "swt.h"
#include "RpcInfo.h"
#include "SequenceGenerator.h"
#include "Register.h"
#include "Config.h"
#include "logging.h"
#include "GeneratorConfig.h"

int main(int argc, const char** argv) {
    GeneratorConfig genCfg(argc, argv);
    
    logging::init(genCfg.logFilename, genCfg.verbose);

    std::mutex mtx;
    std::condition_variable cv;
    bool isDataReceived = false;
    std::string receivedData = "";


    try {
        Config cfg = Config::readFile(genCfg.configFilename);

        BOOST_LOG_TRIVIAL(info) << "Running generator for test suite \"" << cfg.global.name << "\"";

        std::string rpcInName =  cfg.global.alf.toString() + "/RpcIn";
        std::string rpcOutName = cfg.global.alf.toString() + "/RpcOut";

        RpcInfo info(mtx, cv, isDataReceived, receivedData, rpcOutName.c_str());
        for(const auto& test : cfg.tests) {
            size_t seqId = 0;

            if(!test.enabled) {
                BOOST_LOG_TRIVIAL(warning) << "Test \"" << test.name << "\" is disabled";
                continue;
            }
            
            BOOST_LOG_TRIVIAL(info) << "Performing test \"" << test.name << "\"";

            for(size_t i = 0; i < test.repeats; i++) {
                double rttSum = 0;

                for(const auto& seq : test.sequences) {    
                    std::string seqStr = seq.getRequest();

                    BOOST_LOG_TRIVIAL(debug) << "Sending data:\n" << seqStr;

                    auto start = std::chrono::high_resolution_clock::now();
                    DimClient::sendCommand(rpcInName.c_str(), seqStr.c_str());

                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

                    // Process the received data
                    auto stop = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::micro> elapsed = stop - start;
                    double microseconds = elapsed.count();
                    rttSum += microseconds;

                    BOOST_LOG_TRIVIAL(debug) << "Received data:\n" << receivedData << "\n";

                    isDataReceived = false;
                    bool result = SwtSequence::match(seq.getSuccessResponse(), receivedData);
                    if (result == test.sequenceResponses[seqId])
                        BOOST_LOG_TRIVIAL(debug) << test.name << ": success (repeat " << i << ", seqId " << seqId << ", RTT " << microseconds << " us)";
                    else {
                        BOOST_LOG_TRIVIAL(error) << test.name << ": failure (repeat " << i << ", seqId " << seqId << ", RTT " << microseconds << " us, expected " << test.sequenceResponses[seqId] << ")\n--- Sent ---\n" << seqStr << "\n--- Received ---\n" << receivedData;
                        exit(1);
                    }
                    seqId++;
                }
                BOOST_LOG_TRIVIAL(info) << test.name << ": success " << i+1 << "/" << test.repeats << ", mean RTT: " << rttSum / test.sequences.size() << " us";
                usleep(test.wait);
            }
        }
    } catch (const Config::Exception& ce) {
        BOOST_LOG_TRIVIAL(fatal) << "Failed to parse config file: " << ce.what();
        exit(1);
    }

    BOOST_LOG_TRIVIAL(info) << "All tests performed successfully";

    return 0;
}
