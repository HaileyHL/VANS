// ICxlProtocolHandlerBasicTests.cpp
#include <iostream>
#include <cassert>
#include "../src/cxl/ICxlProtocolHandler.h"

// Helper functions to create commands for testing
static CxlCommand CreateValidReadCommand() {
    CxlCommand cmd;
    cmd.type = CxlCommand::Type::Read;
    cmd.size = 64; // typical valid length
    cmd.data = std::vector<uint8_t>(cmd.size, 0xAA);
    return cmd;
}

static CxlCommand CreateInvalidCommand() {
    CxlCommand cmd;
    cmd.type = CxlCommand::Type::Invalid;
    cmd.size = 0;
    cmd.data.clear();
    return cmd;
}

static CxlCommand CreateCommandOfSize(size_t size) {
    CxlCommand cmd;
    cmd.type = CxlCommand::Type::Read; // Use Read for boundary test
    cmd.size = size;
    cmd.data = std::vector<uint8_t>(size, 0x55);
    return cmd;
}

void TestValidReadCommand() {
    MockCxlProtocolHandler handler;
    CxlCommand cmd = CreateValidReadCommand();
    bool result = handler.issueCommand(cmd);
    assert(result && "Valid read command should be accepted");
    assert(handler.WasReadCommandProcessed() && "Read command should be processed");
    std::cout << "TestValidReadCommand passed.\n";
}

void TestInvalidCommandRejected() {
    MockCxlProtocolHandler handler;
    CxlCommand cmd = CreateInvalidCommand();
    bool result = handler.issueCommand(cmd);
    assert(!result && "Invalid command should be rejected");
    std::cout << "TestInvalidCommandRejected passed.\n";
}

void TestCommandBoundaryConditions() {
    MockCxlProtocolHandler handler;
    const size_t MIN_COMMAND_SIZE = 1;    // Define as per protocol spec
    const size_t MAX_COMMAND_SIZE = 1024; // Define as per protocol spec

    CxlCommand minCmd = CreateCommandOfSize(MIN_COMMAND_SIZE);
    bool minResult = handler.issueCommand(minCmd);
    assert(minResult && "Command at minimum size should be accepted");

    CxlCommand maxCmd = CreateCommandOfSize(MAX_COMMAND_SIZE);
    bool maxResult = handler.issueCommand(maxCmd);
    assert(maxResult && "Command at maximum size should be accepted");

    std::cout << "TestCommandBoundaryConditions passed.\n";
}

int main() {
    std::cout << "Starting ICxlProtocolHandlerBasicTests...\n";

    TestValidReadCommand();
    TestInvalidCommandRejected();
    TestCommandBoundaryConditions();

    std::cout << "All tests passed.\n";
    return 0;
}