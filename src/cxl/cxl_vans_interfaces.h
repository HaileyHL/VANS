#include "CxlTypes.h"

// Core Memory Simulation Layer
class ICxlMemory {
public:
    virtual ~ICxlMemory() = default;

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
class ICxlProtocolHandler {
public:
    virtual ~ICxlProtocolHandler() = default;

    // Issue a CXL protocol command (e.g., device discovery, coherency command)
    virtual bool issueCommand(const CxlCommand& cmd) = 0;

    // Register callback to receive memory operation responses
    using ResponseCallback = std::function<void(const CxlResponse&)>;
    virtual void registerResponseCallback(ResponseCallback cb) = 0;
};

// Communication Interface Layer
class ICxlHostInterface {
public:
    virtual ~ICxlHostInterface() = default;

    // Send a CXL protocol command from host to device
    virtual bool sendCommand(const CxlCommand& cmd) = 0;

    // Receive response from device
    virtual bool receiveResponse(CxlResponse& response) = 0;

    virtual void attachDevice(ICxlDeviceInterface* dev) = 0; // Add this
};

class ICxlDeviceInterface {
public:
    virtual ~ICxlDeviceInterface() = default;
    virtual bool receiveCommand(const CxlCommand& cmd) = 0;
    virtual bool sendResponse(const CxlResponse& response) = 0;
};
