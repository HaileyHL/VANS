#ifndef CXLDEVICEINTERFACE_H
#define CXLDEVICEINTERFACE_H

#include "CxlTypes.h"
#include <vector>
#include <cstdint>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>


class ICxlDeviceInterface {
public:
    virtual ~ICxlDeviceInterface() = default;
    virtual bool receiveCommand(const CxlCommand& cmd) = 0;
    virtual bool sendResponse(const CxlResponse& response) = 0;
};

#endif // CXLDEVICEINTERFACE_H