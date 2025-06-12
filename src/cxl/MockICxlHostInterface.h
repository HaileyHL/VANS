#ifndef MOCKCXLHOSTINTERFACE_H
#define MOCKCXLHOSTINTERFACE_H

#include "ICxlHostInterface.h"
#include "CxlTypes.h"
#include <mutex>
#include <condition_variable>

class MockCxlHostInterface : public ICxlHostInterface {
public:
    MockCxlHostInterface();

    void attachDevice(ICxlDeviceInterface* dev) override;
    bool sendCommand(const CxlCommand& cmd) override;
    bool receiveResponse(CxlResponse& response) override;
    void deliverResponse(const CxlResponse& response);

private:
    ICxlDeviceInterface* device;
    CxlCommand sent_command;
    CxlResponse received_response;
    bool response_ready;

    std::mutex mutex;
    std::condition_variable cv;
};

#endif // MOCKCXLHOSTINTERFACE_H
