#include "Mock.h"
#include "IPbusSlave.h"

#include <boost/asio.hpp>

int main(void) {
    boost::asio::io_context io;
    Mock mock(1024);
    ipbus::IPbusSlave slave(io, &mock, 50001);
    while(true) io.run();

    return 0;
}