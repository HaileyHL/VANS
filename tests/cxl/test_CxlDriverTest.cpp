#include "../src/cxl/mocks/MockCxlMemory.h"
#include "../src/cxl/CxlHostInterfaceImpl.h"
#include "../src/cxl/CxlDeviceInterfaceImpl.h"
#include "../src/cxl/CxlProtocolHandlerImpl.h"
#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include "CxlTypes.h"
#include "cxl_vans_interfaces.h"

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <cstring> // for memcpy


// Driver Test
int main() {
    // Set up mocks and wiring
    auto memory = new MockCxlMemory();
    auto protocol = new CxlProtocolHandlerImpl(memory);
    auto host = new CxlHostInterfaceImpl();
    auto device = new CxlDeviceInterfaceImpl(protocol, host);
    host->attachDevice(device);

    // Allocate 16 bytes
    CxlCommand alloc_cmd{ CxlCommand::Type::Allocate, 0, 16 };
    assert(host->sendCommand(alloc_cmd));
    CxlResponse alloc_resp;
    host->receiveResponse(alloc_resp);
    assert(alloc_resp.success);

    uint64_t addr;
    std::cout << "alloc_resp.data.size() = " << alloc_resp.data.size() << ", sizeof(addr) = " << sizeof(addr) << std::endl;
    assert(alloc_resp.data.size() >= sizeof(addr));
    std::memcpy(&addr, alloc_resp.data.data(), sizeof(addr));
    std::cout << "Allocated memory at address: 0x" << std::hex << addr << "\n";

    // Write to allocated memory
    std::vector<uint8_t> write_data{1, 2, 3, 4};
    CxlCommand write_cmd{ CxlCommand::Type::Write, addr, write_data.size(), write_data };
    assert(host->sendCommand(write_cmd));
    CxlResponse write_resp;
    host->receiveResponse(write_resp);
    assert(write_resp.success);

    // Read back
    CxlCommand read_cmd{ CxlCommand::Type::Read, addr, write_data.size() };
    assert(host->sendCommand(read_cmd));
    CxlResponse read_resp;
    host->receiveResponse(read_resp);
    assert(read_resp.success);
    assert(read_resp.data == write_data);
    std::cout << "Readback data matches written data! \n";

    return 0;
}
