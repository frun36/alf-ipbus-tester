#include "Mock.h"
#include "IPbusSlave.h"
#include "IPbusRequest.h"
#include "logging.h"
#include "Config.h"
#include "Tracker.h"
#include "MockConfig.h"

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>


int main(int argc, const char **argv) {
    MockConfig mockCfg(argc, argv);
    
    logging::init(mockCfg.logFilename);

    Config cfg = Config::readFile(mockCfg.configFilename);

    boost::asio::io_context io;
    Mock mock(std::move(cfg));
    ipbus::IPbusSlave slave(io, mock, 50001);
    
    slave.setRequestCallback([&mock](const ipbus::IPbusRequest& req) {
        BOOST_LOG_TRIVIAL(debug) << "Test id: " << mock.trk.currTest << " register id: " << mock.trk.currTestRegister << " repeat: " << mock.trk.currTestRepeat << " (seq ID " << mock.trk.seqId << ", result: " << mock.cfg.tests[mock.trk.currTest].sequenceResponses[mock.trk.seqId] << ")";
        if(req.isStatusRequest()) {
            BOOST_LOG_TRIVIAL(info) << "Status packet received";
        } else {
            switch(mock.trk.registerPacket(req.getSize(), mock.getCurrResponse())) {
                case Tracker::Status::Ok:
                case Tracker::Status::Split:
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