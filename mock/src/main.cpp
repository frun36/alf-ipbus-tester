#include "Mock.h"
#include "IPbusSlave.h"
#include "IPbusRequest.h"
#include "logging.h"
#include "Config.h"
#include "Tracker.h"

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>


int main(void) {
    logging::init();

    Config cfg = Config::readFile("../example_config.toml");

    Tracker trk(cfg);

    boost::asio::io_context io;
    Mock mock("ftm_registers.csv");
    ipbus::IPbusSlave slave(io, mock, 50001);

    BOOST_LOG_TRIVIAL(debug) << "Test id: " << trk.currTest << " register id: " << trk.currTestRegister << " repeat: " << trk.currTestRepeat;
    
    slave.setRequestCallback([&trk](const ipbus::IPbusRequest& req) {
        if(req.isStatusRequest()) {
            BOOST_LOG_TRIVIAL(info) << "Status packet received";
        } else {
            if(trk.registerPacket(req.getSize()))
                BOOST_LOG_TRIVIAL(debug) << "Test id: " << trk.currTest << " register id: " << trk.currTestRegister << " repeat: " << trk.currTestRepeat;
            else
                BOOST_LOG_TRIVIAL(warning) << "Tracking failed";
        }
    });

    while(true) 
        io.run();

    return 0;
}