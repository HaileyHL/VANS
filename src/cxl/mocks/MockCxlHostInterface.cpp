#include "MockCxlHostInterface.h"
#include <iostream>
#include <thread>
#include <chrono>

MockCxlHostInterface::MockCxlHostInterface()
    : device(nullptr), response_ready(false) {}

void MockCxlHostInterface::attachDevice(CxlDeviceInterface* dev) {
    device = dev;
}

bool MockCxlHostInterface::sendCommand(const CxlCommand& cmd) {
    if (!device) return false;

    std::cout << "[Host -> Device] Command sent: " << static_cast<int>(cmd.type)
              << " @ " << cmd.address << " (" << cmd.size << "B)\n";

    std::unique_lock<std::mutex> lock(mutex);
    sent_command = cmd;
    response_ready = false;

    std::thread([this, cmd]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        device->receiveCommand(cmd);
    }).detach();

    return true;
}

bool MockCxlHostInterface::receiveResponse(CxlResponse& response) {
    std::unique_lock<std::mutex> lock(mutex);
    if (!cv.wait_for(lock, std::chrono::milliseconds(100), [this]{ return response_ready; })) {
        std::cerr << "[Host] Timeout waiting for response\n";
        return false;
    }
    response = received_response;
    std::cout << "[Device -> Host] Response received: success=" << response.success << "\n";
    return true;
}

void MockCxlHostInterface::deliverResponse(const CxlResponse& response) {
    std::lock_guard<std::mutex> lock(mutex);
    received_response = response;
    response_ready = true;
    cv.notify_one();
}

void MockCxlHostInterface::setResponse(const CxlResponse& resp) {
    std::unique_lock<std::mutex> lock(mutex);
    received_response = resp;
    response_ready = true;
    cv.notify_all();
}

// Failed Attempt [Session 18]
// class MockCxlDeviceInterface : public CxlDeviceInterface {
// public:
//     MockCxlDeviceInterface();

//     /**
//      * @brief Receives a command from the host and logs it.
//      */
//     bool receiveCommand(const CxlCommand& cmd) {
//         receivedCommands.push_back(cmd);
//         return true;
//     }

//     /**
//      * @brief Sends a response back to the host or test environment.
//      */
//     bool sendResponse(const CxlResponse& response) {
//         sentResponses.push_back(response);
//         return true;
//     }

//     /**
//      * @brief Returns the list of received commands.
//      */
//     const std::vector<CxlCommand>& getReceivedCommands() {
//         return receivedCommands;
//     }

//     /**
//      * @brief Returns the list of sent responses.
//      */
//     const std::vector<CxlResponse>& getSentResponses() {
//         return sentResponses;
//     }

// private:
//     std::vector<CxlCommand> receivedCommands;
//     std::vector<CxlResponse> sentResponses;
// };
