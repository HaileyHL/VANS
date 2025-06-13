#ifndef MOCKCXLDEVICEINTERFACE_H
#define MOCKCXLDEVICEINTERFACE_H

#include "cxl_vans_interfaces.h"
#include "CxlTypes.h"

class MockCxlHostInterface; // forward declaration

class MockCxlDeviceInterface : public CxlDeviceInterface {
public:
    MockCxlDeviceInterface();

    void attachHost(MockCxlHostInterface* h);
    bool receiveCommand(const CxlCommand& cmd) override;
    bool sendResponse(const CxlResponse& response) override;

private:
    MockCxlHostInterface* host;
};

#endif // MOCKCXLDEVICEINTERFACE_H


// Failed Attempt [Session 18]
// class MockCxlHostInterface : public CxlHostInterface {
// public:
//     MockCxlHostInterface();

//     /**
//      * @brief Sends a CXL protocol command from the host to the attached device.
//      */
//     bool sendCommand(const CxlCommand& cmd) {
//     sentCommands.push_back(cmd);
//     if (attachedDevice) {
//         return attachedDevice->receiveCommand(cmd);
//     }
//     return false;
//     }

//     /**
//      * @brief Receives a response from the device, if available.
//      *
//      * This function checks if there is a response available in the internal response queue.
//      * If a response is available, it copies the response to the provided reference and returns true.
//      * If no response is available, it returns false.
//      */
//     bool receiveResponse(CxlResponse& response) {
//         if (!responseQueue.empty()) {
//             response = responseQueue.front();
//             responseQueue.pop();
//             return true;
//         }
//         return false;
//     }

//     /**
//      * @brief Attaches a mock device interface for communication.
//      */
//     void attachDevice(CxlDeviceInterface* dev) {
//         attachedDevice = dev;
//     }

//     /**
//      * @brief Injects a simulated response into the internal response queue.
//      */
//     void injectResponse(const CxlResponse& response) {
//         responseQueue.push(response);
//     }

//     /**
//      * @brief Returns the list of commands sent so far.
//      */
//     const std::vector<CxlCommand>& getSentCommands() const{
//         return sentCommands;
//     }

// private:
//     std::vector<CxlCommand> sentCommands;
//     std::queue<CxlResponse> responseQueue;
//     CxlDeviceInterface* attachedDevice = nullptr;
// };
