#include "CxlCommandProcessor.h"
#include <cstring>

CxlCommandProcessor::CxlCommandProcessor(CxlMemory* memory, CxlProtocolHandler* protocol)
    : memory_(memory), protocol_(protocol) {}

void CxlCommandProcessor::initialize() {
    protocol_->registerCommandCallback([this](const CxlCommand& cmd) {
        this->handleCommand(cmd);
    });

    protocol_->registerResponseCallback([this](const CxlResponse& resp) {
        this->response_callback_ = [resp](const CxlResponse&) {
            // This can be a no-op or logger depending on intent.
        };
    });
}

void CxlCommandProcessor::handleCommand(const CxlCommand& cmd) {
    CxlResponse response;
    response.success = false;

    switch (cmd.type) {
        case CxlCommand::Type::Read: {
            std::vector<uint8_t> buffer(cmd.size);
            if (memory_->read(cmd.address, buffer.data(), cmd.size)) {
                response.success = true;
                response.data = std::move(buffer);
            }
            break;
        }
        case CxlCommand::Type::Write: {
            if (memory_->write(cmd.address, cmd.data.data(), cmd.size)) {
                response.success = true;
            }
            break;
        }
        case CxlCommand::Type::Allocate: {
            uint64_t addr = memory_->allocate(cmd.size);
            response.success = true;
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
    if (protocol_) {
        protocol_->invokeResponse(response);
    }
}
