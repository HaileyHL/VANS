#ifndef CXLPROTOCOLHANDLER_H
#define CXLPROTOCOLHANDLER_H

#include "CxlTypes.h"
#include "cxl_vans_interfaces.h"
#include <functional>
#include <vector>
#include <cstdint>
#include <cstring> 
#include <iostream>

using ResponseCallback = std::function<void(const CxlResponse&)>;

class CxlProtocolHandlerImpl : public CxlProtocolHandler {
    CxlMemory* memory_;
    ResponseCallback response_cb_;

public:
    explicit CxlProtocolHandlerImpl(CxlMemory* mem) : memory_(mem) {}

    void registerResponseCallback(ResponseCallback cb) override {
        response_cb_ = std::move(cb);
    }

    bool issueCommand(const CxlCommand& cmd) override {
        CxlResponse response{};

        switch (cmd.type) {
            case CxlCommand::Type::Read: {
                response.data.resize(cmd.size);
                response.success = memory_->read(cmd.address, response.data.data(), cmd.size);
                break;
            }
            case CxlCommand::Type::Write: {
                response.success = memory_->write(cmd.address, cmd.data.data(), cmd.size);
                break;
            }
            case CxlCommand::Type::Allocate: {
                uint64_t addr = memory_->allocate(cmd.size);
                response.success = addr != 0;
                response.data.resize(sizeof(addr));
                std::memcpy(response.data.data(), &addr, sizeof(addr));
                break;
            }
            case CxlCommand::Type::Deallocate: {
                memory_->deallocate(cmd.address);
                response.success = true;
                break;
            }
            default:
                response.success = false;
                break;
        }

        // Trigger callback if set
        if (response_cb_) response_cb_(response);
        return response.success;
    }
};

#endif // CXLPROTOCOLHANDLER_H