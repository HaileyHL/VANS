#ifndef ICXLPROTOCOLHANDLER_H
#define ICXLPROTOCOLHANDLER_H

#include "CxlTypes.h"
#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include <cstdint>

class ICxlProtocolHandler {
public:
    virtual ~ICxlProtocolHandler() = default;

    virtual bool issueCommand(const CxlCommand& cmd) = 0;

    using ResponseCallback = std::function<void(const CxlResponse&)>;
    virtual void registerResponseCallback(ResponseCallback cb) = 0;
};


class MockCxlProtocolHandler : public ICxlProtocolHandler {
public:
    MockCxlProtocolHandler() : response_callback(nullptr) {}

    bool issueCommand(const CxlCommand& cmd) override {
        // Simulate processing command asynchronously by pushing to queue
        command_queue.push(cmd);
        processNextCommand();
        return true;
    }

    void registerResponseCallback(ResponseCallback cb) override {
        response_callback = cb;
    }

private:
    void processNextCommand() {
        if (command_queue.empty() || !response_callback) return;

        CxlCommand cmd = command_queue.front();
        command_queue.pop();

        // Simulate a simple response based on command type
        CxlResponse response;
        response.success = true;

        switch (cmd.type) {
            case CxlCommand::Type::Read:
                // Return dummy data of requested size
                response.data = std::vector<uint8_t>(cmd.size, 0xAA);
                break;
            case CxlCommand::Type::Write:
                // Acknowledge write success
                response.data.clear();
                break;
            case CxlCommand::Type::Discover:
                // Simulate discovery response
                response.data = {'C', 'X', 'L', 'D'};
                break;
            case CxlCommand::Type::Invalidate:
                // Reject invalid response
                response.success = false;
                response.data.clear();
                response_callback(response);
                return;
            default:
                // Generic success for other commands
                response.data.clear();
                break;
        }

        // Invoke callback with simulated response
        response_callback(response);
    }

    ResponseCallback response_callback;
    std::queue<CxlCommand> command_queue;
};

#endif // ICXLPROTOCOLHANDLER_H