#ifndef CXL_VANS_INTERFACES_H
#define CXL_VANS_INTERFACES_H

// Forward declarations
class CxlDeviceInterface;
class CxlHostInterface;
class CxlMemory;
class CxlProtocolHandler;


#include "CxlTypes.h"

#include <cstdint>
#include <vector>
#include <functional>
#include <unordered_map>
#include <cstring> 

// Core Memory Simulation Layer
class CxlMemory {
public:
    virtual ~CxlMemory() = default;

    // Read data from CXL memory at given address
    virtual bool read(uint64_t address, uint8_t* buffer, size_t size) = 0;

    // Write data to CXL memory at given address
    virtual bool write(uint64_t address, const uint8_t* data, size_t size) = 0;

    // Allocate memory region in CXL memory pool
    virtual uint64_t allocate(size_t size) = 0;

    // Deallocate memory region
    virtual void deallocate(uint64_t address) = 0;
};

// CXL Protocol Simulation Layer
class CxlProtocolHandler {
public:
    virtual ~CxlProtocolHandler() = default;

    virtual bool issueCommand(const CxlCommand& cmd) = 0;

    using ResponseCallback = std::function<void(const CxlResponse&)>;
    virtual void registerResponseCallback(ResponseCallback cb) = 0;

    using CommandCallback = std::function<void(const CxlCommand&)>;
    virtual void registerCommandCallback(CommandCallback cb) = 0;

    virtual void invokeResponse(const CxlResponse& resp) = 0;

};

// Communication Interface Layer
class CxlHostInterface {
public:
    virtual ~CxlHostInterface() = default;

    // Send a CXL protocol command from host to device
    virtual bool sendCommand(const CxlCommand& cmd) = 0;

    // Receive response from device
    virtual bool receiveResponse(CxlResponse& response) = 0;

    virtual void attachDevice(CxlDeviceInterface* dev) = 0; // Add this
};

class CxlDeviceInterface {
public:
    virtual ~CxlDeviceInterface() = default;
    virtual bool receiveCommand(const CxlCommand& cmd) = 0;
    virtual bool sendResponse(const CxlResponse& response) = 0;
};

#endif // CXL_VANS_INTERFACES_H