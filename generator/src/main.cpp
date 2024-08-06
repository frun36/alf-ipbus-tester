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
    Config cfg = Config::readFile("../example_config.toml");

    std::mutex mtx;
    std::condition_variable cv;
    bool isDataReceived = false;
    std::string receivedData = "";

    RpcInfo info(mtx, cv, isDataReceived, receivedData);

    std::vector<Register> registers = Register::readMapFromFile("ftm_registers.csv");
    
    for(const auto& test : cfg.tests) {
        if(!test.enabled) {
            std::cout << "Test \"" << test.name << "\" is disabled\n";
            continue;
        }

        std::cout << "Performing test \"" << test.name << "\"\n";
        std::string seq = test.sequence.getRequest();

        std::cout << "Sending data:\n" << seq << "\n\n";
        DimClient::sendCommand("ALF_FTM/SERIAL_0/LINK_0/SWT_SEQUENCE/RpcIn", seq.c_str());

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

        // Process the received data
        std::cout << "Received data:\n" << receivedData << std::endl;


        isDataReceived = false;
        std::cout << SwtSequence::match(test.sequence.getSuccessResponse(), receivedData) << "\n";
    }


    return 0;
}
