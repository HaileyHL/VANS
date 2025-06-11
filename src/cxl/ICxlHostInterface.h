#ifndef ICXLHOSTINTERFACE_H
#define ICXLHOSTINTERFACE_H

#include "CXLVANS Interface.txt"  // Assumes CxlCommand and CxlResponse are defined here
#include <queue>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>

class MockCxlHostInterface : public ICxlHostInterface {
public:
    MockCxlHostInterface()
        : device(nullptr), response_ready(false) {}

    void attachDevice(ICxlDeviceInterface* dev) {
        device = dev;
    }

    bool sendCommand(const CxlCommand& cmd) override {
        if (!device) return false;

        std::cout << "[Host -> Device] Command sent: " << static_cast<int>(cmd.type)
                  << " @ " << cmd.address << " (" << cmd.size << "B)\n";

        std::unique_lock<std::mutex> lock(mutex);
        sent_command = cmd;
        response_ready = false;
        
        std::thread([this, cmd]() {
            // Simulate latency
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            CxlResponse resp;
            device->receiveCommand(cmd);  // device will handle and eventually call sendResponse
        }).detach();

        return true;
    }

    bool receiveResponse(CxlResponse& response) override {
        std::unique_lock<std::mutex> lock(mutex);
        if (!cv.wait_for(lock, std::chrono::milliseconds(100), [this]{ return response_ready; })) {
            std::cerr << "[Host] Timeout waiting for response\n";
            return false;
        }
        response = received_response;
        std::cout << "[Device -> Host] Response received: success=" << response.success << "\n";
        return true;
    }

    // Called by the device to deliver a response back to the host
    void deliverResponse(const CxlResponse& response) {
        std::lock_guard<std::mutex> lock(mutex);
        received_response = response;
        response_ready = true;
        cv.notify_one();
    }

private:
    ICxlDeviceInterface* device;
    CxlCommand sent_command;
    CxlResponse received_response;
    bool response_ready;

    std::mutex mutex;
    std::condition_variable cv;
};

#endif // ICXLHOSTINTERFACE_H