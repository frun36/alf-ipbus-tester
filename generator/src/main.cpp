#include <dim/dic.hxx>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "swt.h"
#include "RpcInfo.h"
#include "SequenceGenerator.h"

int main(void) {
    std::mutex mtx;
    std::condition_variable cv;
    bool isDataReceived = false;
    std::string receivedData = "";

    RpcInfo info(mtx, cv, isDataReceived, receivedData);

    SwtSequence s = {
        SwtOperation(SwtOperation::Type::Write, 0x1004, 0xc0ffee),
    };
    
    std::cout << "Request:\n" << s.getRequest() << "\nExpected response:\n" << s.getResponse() << "\n";

    std::string seq = s.getRequest();

    DimClient::sendCommand("ALF_FTM/SERIAL_0/LINK_0/SWT_SEQUENCE/RpcIn", seq.c_str());

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

    // Process the received data
    std::cout << "Received data:\n" << receivedData << std::endl;

    // Reset the flag
    isDataReceived = false;

    std::cout << SwtSequence::match(s.getResponse(), receivedData) << "\n";


    return 0;
}
