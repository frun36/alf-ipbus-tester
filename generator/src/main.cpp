#include <dim/dic.hxx>
#include <iostream>
#include <mutex>
#include <condition_variable>

class RpcInfo: public DimUpdatedInfo {
    std::mutex& m_mtx;
    std::condition_variable& m_cv;
    bool& m_isDataReceived;
    std::string& m_receivedData;
    
public:
    RpcInfo(std::mutex& mtx, std::condition_variable& cv, bool& isDataReceived, std::string& receivedData)
        : DimUpdatedInfo("ALF_FTM/SERIAL_0/LINK_0/SWT_SEQUENCE/RpcOut", -1), 
          m_mtx(mtx), 
          m_cv(cv), 
          m_isDataReceived(isDataReceived), 
          m_receivedData(receivedData) {}

    void infoHandler() {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_receivedData = getString();
        m_isDataReceived = true;
        m_cv.notify_one();
    }
};

int main(void) {
    std::mutex mtx;
    std::condition_variable cv;
    bool isDataReceived = false;
    std::string receivedData = "";

    RpcInfo info(mtx, cv, isDataReceived, receivedData);

    while(true) {
        std::string sequence = "";
        std::cout << "Input SWT sequence:\nBEGIN\n";
        std::string line = "";
        std::getline(std::cin, line);
        while (line != "END") {
            sequence += line + "\n";
            std::getline(std::cin, line);
        }

        if (sequence.back() == '\n') sequence.pop_back();

        DimClient::sendCommand("ALF_FTM/SERIAL_0/LINK_0/SWT_SEQUENCE/RpcIn", sequence.c_str());

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&isDataReceived]{ return isDataReceived; });

        // Process the received data
        std::cout << "Received data: " << receivedData << std::endl;

        // Reset the flag
        isDataReceived = false;
    }


    return 0;
}