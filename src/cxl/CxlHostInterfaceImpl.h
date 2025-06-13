#ifndef CXL_HOST_INTERFACE_IMPL_H
#define CXL_HOST_INTERFACE_IMPL_H

#include "cxl_vans_interfaces.h"
#include "CxlTypes.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>
#include "CxlDeviceInterfaceImpl.h" 
#include "CxlProtocolHandlerImpl.h"

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
        if (!response_ready_) return false;
        response = response_;
        response_ready_ = false;
        std::cout << "Received response: " << (response.success ? "Success" : "Failure") << std::endl;
        return true;
    }

    void setResponse(const CxlResponse& resp) {
        // Store the response and mark it as ready, or handle as appropriate for your design
        this->response_ = resp;
        this->response_ready_ = true;
    }
private:
    CxlResponse response_;
    bool response_ready_ = false;
};

#endif // CXL_HOST_INTERFACE_IMPL_H