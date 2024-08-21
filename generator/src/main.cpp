#include <dim/dic.hxx>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "swt.h"
#include "RpcInfo.h"
#include "SequenceGenerator.h"
#include "Register.h"
#include "Config.h"

int main(void) {
    std::mutex mtx;
    std::condition_variable cv;
    bool isDataReceived = false;
    std::string receivedData = "";

    RpcInfo info(mtx, cv, isDataReceived, receivedData);

    try {
        Config cfg = Config::readFile("../example_config.toml");

        for(const auto& test : cfg.tests) {
            if(!test.enabled) {
                std::cout << "Test \"" << test.name << "\" is disabled\n";
                continue;
            }
            
            std::cout << "Performing test \"" << test.name << "\"\n";

            for(size_t i = 0; i < test.repeats; i++) {
                for(const auto& seq : test.sequences) {    
                    std::string seqStr = seq.getRequest();

                    std::cout << "Sending data:\n" << seqStr << "\n\n";
                    DimClient::sendCommand("ALF_FTM/SERIAL_0/LINK_0/SWT_SEQUENCE/RpcIn", seqStr.c_str());

                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

                    // Process the received data
                    std::cout << "Received data:\n" << receivedData << std::endl;

                    isDataReceived = false;
                    std::cout << "\nResult: " << (test.shouldSequenceSucceed(cfg.global.rng) ? SwtSequence::match(seq.getSuccessResponse(), receivedData) : true) << "\n";
                }
                usleep(test.wait);
            }


        }
    } catch (const Config::Exception& ce) {
        std::cerr << "Failed to parse config file: " << ce.what() << "\n";
        exit(1);
    }

    return 0;
}
