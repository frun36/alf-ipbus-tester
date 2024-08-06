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
    std::cout << cfg.global.name << "\n"
              << cfg.global.registerFile << "\n"
              << cfg.global.rngSeed << "\n"
              << cfg.tests[0].name << "\n"
              << cfg.tests[1].name << "\n";

    // std::mutex mtx;
    // std::condition_variable cv;
    // bool isDataReceived = false;
    // std::string receivedData = "";

    // RpcInfo info(mtx, cv, isDataReceived, receivedData);

    // std::vector<Register> registers = Register::readMapFromFile("ftm_registers.csv");

    // SwtSequence s = {};

    // for (size_t addr = 0; addr < registers.size(); addr++) {
    //     if (registers[addr].isRead) {
    //         s.addOperation(SwtOperation(SwtOperation::Type::Read, addr));

    //         if (registers[addr].isWrite) {
    //             s.addOperation(SwtOperation(SwtOperation::Type::Write, addr, 0xb00b));
    //             s.addOperation(SwtOperation(SwtOperation::Type::RmwBits, addr, 0xffff, 0x4000));
    //             s.addOperation(SwtOperation(SwtOperation::Type::RmwSum, addr, 0x0002));
    //             s.addOperation(SwtOperation(SwtOperation::Type::Read, addr));
    //         }
    //     }
    // }
    

    // std::string seq = s.getRequest();
    // std::cout << "Sending data:\n" << seq << "\n\n";
    // DimClient::sendCommand("ALF_FTM/SERIAL_0/LINK_0/SWT_SEQUENCE/RpcIn", seq.c_str());

    // std::unique_lock<std::mutex> lock(mtx);
    // cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

    // // Process the received data
    // std::cout << "Received data:\n" << receivedData << std::endl;


    // isDataReceived = false;
    // std::cout << SwtSequence::match(s.getSuccessResponse(), receivedData) << "\n";


    return 0;
}
