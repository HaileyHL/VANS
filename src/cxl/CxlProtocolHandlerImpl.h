#ifndef CXLPROTOCOLHANDLER_H
#define CXLPROTOCOLHANDLER_H

#include "CxlTypes.h"
#include "cxl_vans_interfaces.h"
#include <functional>
#include <vector>
#include <cstdint>
#include <cstring> 

using ResponseCallback = std::function<void(const CxlResponse&)>;

class CxlProtocolHandlerImpl : public CxlProtocolHandler {
    CxlMemory* memory_;
    ResponseCallback response_cb_;
    CommandCallback command_cb_;

public:
    explicit CxlProtocolHandlerImpl(CxlMemory* mem) : memory_(mem) {}

    void registerResponseCallback(ResponseCallback cb) override {
        response_cb_ = std::move(cb);
    }

    void registerCommandCallback(CommandCallback cb) override {
        command_cb_ = std::move(cb);
    }

    void invokeResponse(const CxlResponse& resp) override {
        if (response_cb_) response_cb_(resp);
    }

bool issueCommand(const CxlCommand& cmd) override {
    CxlResponse response{};

    switch (cmd.type) {
        case CxlCommand::Type::Read:
            response.data.resize(cmd.size);
            response.success = memory_->read(cmd.address, response.data.data(), cmd.size);
            break;

        case CxlCommand::Type::Write:
            response.success = memory_->write(cmd.address, cmd.data.data(), cmd.size);
            break;

        case CxlCommand::Type::Allocate: {
            uint64_t addr = memory_->allocate(cmd.size);
            if (addr != 0) {
                response.success = true;
                response.data.resize(sizeof(addr));
                std::memcpy(response.data.data(), &addr, sizeof(addr));
            } else {
                response.success = false;
            }
            break;
        }

        case CxlCommand::Type::Deallocate:
            memory_->deallocate(cmd.address);
            response.success = true;
            break;

        default:
            response.success = false;
            break;
    }

    if (response_cb_) response_cb_(response);
    return response.success;
}
};

#endif // CXLPROTOCOLHANDLER_H