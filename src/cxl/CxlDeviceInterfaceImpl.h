#ifndef CXL_DEVICE_INTERFACE_IMPL_H
#define CXL_DEVICE_INTERFACE_IMPL_H

#include "cxl_vans_interfaces.h"
#include "CxlProtocolHandlerImpl.h"
// #include "CxlHostInterfaceImpl.h"
#include "CxlTypes.h"

class CxlDeviceInterfaceImpl : public CxlDeviceInterface {
    CxlProtocolHandler* handler_;
    CxlHostInterface* host_;

public:
    CxlDeviceInterfaceImpl(CxlProtocolHandler* h, CxlHostInterface* host)
        : handler_(h), host_(host) {
        handler_->registerResponseCallback([this](const CxlResponse& resp) {
            this->host_->setResponse(resp);  // now valid
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
