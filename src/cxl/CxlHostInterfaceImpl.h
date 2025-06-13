#ifndef CXL_HOST_INTERFACE_IMPL_H
#define CXL_HOST_INTERFACE_IMPL_H

#include "cxl_vans_interfaces.h"
#include "CxlTypes.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>
#include "CxlDeviceInterfaceImpl.cpp" 
#include "CxlProtocolHandlerImpl.cpp"

class CxlHostInterfaceImpl : public CxlHostInterface {
    CxlDeviceInterface* device_ = nullptr;

public:
    void attachDevice(CxlDeviceInterface* dev) override {
        device_ = dev;
    }

    bool sendCommand(const CxlCommand& cmd) override {
        if (!device_) return false;
        return device_->receiveCommand(cmd);
    }

    bool receiveResponse(CxlResponse& response) override {
        // Stub: you can extend this to buffer and pop from a queue if needed
        std::cout << "Received response: " << (response.success ? "Success" : "Failure") << std::endl;
        return true;
    }
};
