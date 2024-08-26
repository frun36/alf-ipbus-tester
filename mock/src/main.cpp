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
            switch(trk.registerPacket(req.getSize())) {
                case Tracker::Status::Ok:
                case Tracker::Status::Split:
                    BOOST_LOG_TRIVIAL(debug) << "Test id: " << trk.currTest << " register id: " << trk.currTestRegister << " repeat: " << trk.currTestRepeat;
                    break;
                case Tracker::Status::Error:
                    BOOST_LOG_TRIVIAL(warning) << "Tracking failed";
                    break;
                case Tracker::Status::Finished:
                    BOOST_LOG_TRIVIAL(info) << "All tests performed";
                    break;
            }
        }
    });

    while(true) 
        io.run();

    return 0;
}