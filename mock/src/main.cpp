#include "Mock.h"
#include "IPbusSlave.h"
#include "IPbusRequest.h"

#include <boost/asio.hpp>

#include "logging.h"

int main(void) {
    logging::init();

    boost::asio::io_context io;
    Mock mock("ftm_registers.csv");
    ipbus::IPbusSlave slave(io, mock, 50001);

    slave.setRequestCallback([](const ipbus::IPbusRequest& req) {
        std::cout << "Received words: " << req.getSize() << "\n";
    });

    while(true) 
        io.run();

    return 0;
}