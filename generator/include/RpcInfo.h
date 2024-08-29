#pragma once

#include <dim/dic.hxx>

#include <mutex>
#include <condition_variable>

class RpcInfo: public DimUpdatedInfo {
    std::mutex& m_mtx;
    std::condition_variable& m_cv;
    bool& m_isDataReceived;
    std::string& m_receivedData;
    
public:
    RpcInfo(std::mutex& mtx, std::condition_variable& cv, bool& isDataReceived, std::string& receivedData, const char* serviceName)
        : DimUpdatedInfo(serviceName, -1), 
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
