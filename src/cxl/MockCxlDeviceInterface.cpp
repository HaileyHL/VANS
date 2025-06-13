#include "MockCxlDeviceInterface.h"
#include "MockCxlHostInterface.h"
#include <iostream>
#include <thread>
#include <chrono>

MockCxlDeviceInterface::MockCxlDeviceInterface() : host(nullptr) {}

void MockCxlDeviceInterface::attachHost(MockCxlHostInterface* h) {
    host = h;
}

bool MockCxlDeviceInterface::receiveCommand(const CxlCommand& cmd) {
    std::cout << "[Device] Received command: " << static_cast<int>(cmd.type)
              << " @ " << cmd.address << " (" << cmd.size << "B)\n";

    std::thread([this, cmd]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        CxlResponse response;
        response.success = true;

        if (cmd.type == CxlCommand::Type::Read) {
            response.data = std::vector<uint8_t>(cmd.size, 0xBB);
        } else if (cmd.type == CxlCommand::Type::Write) {
            response.data.clear();
        } else {
            response.data = {'D', 'E', 'V'};
        }

        sendResponse(response);
    }).detach();

    return true;
}

bool MockCxlDeviceInterface::sendResponse(const CxlResponse& response) {
    if (!host) return false;
    std::cout << "[Device -> Host] Sending response: success=" << response.success << "\n";
    host->deliverResponse(response);
    return true;
}
