#include "../src/cxl/CxlCommandProcessor.h"
#include "../src/cxl/mocks/MockCxlMemory.h"
#include "../src/cxl/mocks/MockCxlProtocolHandler.h"
#include "CxlTypes.h"
#include <cassert>
#include <iostream>
#include <cstring>

int main() {
    MockCxlMemory memory;
    MockCxlProtocolHandler protocol;

    CxlCommandProcessor processor(&memory, &protocol);
    processor.initialize();

    std::vector<CxlResponse> responses;
    protocol.registerResponseCallback([&responses](const CxlResponse& resp) {
        responses.push_back(resp);
    });

    // Step 1: Allocate memory
    CxlCommand allocCmd;
    allocCmd.type = CxlCommand::Type::Allocate;
    allocCmd.size = 8;

    protocol.issueCommand(allocCmd);
    assert(!responses.empty());
    assert(responses.back().success);

    uint64_t address;
    std::memcpy(&address, responses.back().data.data(), sizeof(address));
    responses.clear();

    // Step 2: Write data
    std::vector<uint8_t> dataToWrite = {1, 2, 3, 4, 5, 6, 7, 8};
    CxlCommand writeCmd;
    writeCmd.type = CxlCommand::Type::Write;
    writeCmd.address = address;
    writeCmd.size = dataToWrite.size();
    writeCmd.data = dataToWrite;

    protocol.issueCommand(writeCmd);
    assert(!responses.empty());
    assert(responses.back().success);
    responses.clear();

    // Step 3: Read back the data
    CxlCommand readCmd;
    readCmd.type = CxlCommand::Type::Read;
    readCmd.address = address;
    readCmd.size = dataToWrite.size();

    protocol.issueCommand(readCmd);
    assert(!responses.empty());
    assert(responses.back().success);
    assert(responses.back().data == dataToWrite);
    responses.clear();

    // Step 4: Deallocate
    CxlCommand deallocCmd;
    deallocCmd.type = CxlCommand::Type::Deallocate;
    deallocCmd.address = address;

    protocol.issueCommand(deallocCmd);
    assert(!responses.empty());
    assert(responses.back().success);

    std::cout << "All CxlCommandProcessor tests passed." << std::endl;
    return 0;
}
