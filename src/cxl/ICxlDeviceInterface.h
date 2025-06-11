#ifndef ICXLDEVICEINTERFACE_H
#define ICXLDEVICEINTERFACE_H

#include "CXLVANS Interface.txt"  // Assumes CxlCommand and CxlResponse are defined here
#include "ICxlHostInterface.h"
#include <iostream>
#include <thread>
#include <chrono>

class MockCxlDeviceInterface : public ICxlDeviceInterface {
public:
    MockCxlDeviceInterface() : host(nullptr) {}

    void attachHost(MockCxlHostInterface* h) {
        host = h;
    }

    bool receiveCommand(const CxlCommand& cmd) override {
        std::cout << "[Device] Received command: " << static_cast<int>(cmd.type)
                  << " @ " << cmd.address << " (" << cmd.size << "B)\n";

        std::thread([this, cmd]() {
            // Simulate processing delay
            std::this_thread::sleep_for(std::chrono::milliseconds(15));

            CxlResponse response;
            response.success = true;

            if (cmd.type == CxlCommand::Type::Read) {
                response.data = std::vector<uint8_t>(cmd.size, 0xBB);
            } else if (cmd.type == CxlCommand::Type::Write) {
                // Simulate write acknowledgment
                response.data.clear();
            } else {
                response.data = {'D', 'E', 'V'};  // Generic response
            }

            sendResponse(response);
        }).detach();

        return true;
    }

    bool sendResponse(const CxlResponse& response) override {
        if (!host) return false;

        std::cout << "[Device -> Host] Sending response: success=" << response.success << "\n";
        host->deliverResponse(response);
        return true;
    }

private:
    MockCxlHostInterface* host;
};

#endif // ICXLDEVICEINTERFACE_H
