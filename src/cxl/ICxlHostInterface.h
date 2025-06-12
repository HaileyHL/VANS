#ifndef CXLHOSTINTERFACE_H
#define CXLHOSTINTERFACE_H

#include "CxlTypes.h"
#include "ICxlDeviceInterface.h"
#include <vector>
#include <cstdint>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>


class ICxlHostInterface {
public:
    virtual ~ICxlHostInterface() = default;

    // Send a CXL protocol command from host to device
    virtual bool sendCommand(const CxlCommand& cmd) = 0;

    // Receive response from device
    virtual bool receiveResponse(CxlResponse& response) = 0;

    virtual void attachDevice(ICxlDeviceInterface* dev) = 0; // Add this
};

#endif // CXLHOSTINTERFACE_H