#ifndef CXL_DEVICE_INTERFACE_IMPL_H
#define CXL_DEVICE_INTERFACE_IMPL_H

#include "cxl_vans_interfaces.h"
#include "CxlTypes.h"
#include "CxlProtocolHandlerImpl.h"
#include <functional>
#include <vector>

class CxlDeviceInterfaceImpl : public CxlDeviceInterface {
    CxlProtocolHandler* handler_;
    CxlHostInterface* host_;

public:
    CxlDeviceInterfaceImpl(CxlProtocolHandler* h, CxlHostInterface* host)
        : handler_(h), host_(host) {
        handler_->registerResponseCallback([this](const CxlResponse& resp) {
            host_->receiveResponse(const_cast<CxlResponse&>(resp));
        });
    }

    bool receiveCommand(const CxlCommand& cmd) override {
        return handler_->issueCommand(cmd);
    }

    bool sendResponse(const CxlResponse& response) override {
        return host_->receiveResponse(const_cast<CxlResponse&>(response));
    }
};

#endif // CXL_DEVICE_INTERFACE_IMPL_H