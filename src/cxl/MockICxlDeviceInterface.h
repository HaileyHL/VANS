#ifndef MOCKCXLDEVICEINTERFACE_H
#define MOCKCXLDEVICEINTERFACE_H

#include "ICxlDeviceInterface.h"
#include "CxlTypes.h"

class MockCxlHostInterface; // forward declaration

class MockCxlDeviceInterface : public ICxlDeviceInterface {
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
// class MockCxlHostInterface : public ICxlHostInterface {
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
//     void attachDevice(ICxlDeviceInterface* dev) {
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
//     ICxlDeviceInterface* attachedDevice = nullptr;
// };
