#include "../src/cxl/MockICxlHostInterface.h"
#include "../src/cxl/MockICxlDeviceInterface.h"
#include <cassert>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

void testBasicCommandRoundTrip() {
    MockCxlHostInterface host;
    MockCxlDeviceInterface device;
    device.attachHost(&host);

    CxlCommand cmd;
    cmd.type = CxlCommand::Type::Read;
    cmd.address = 0x1000;
    cmd.size = 4;

    CxlResponse resp;
    bool sent = host.sendCommand(cmd);
    assert(sent);
    assert(host.receiveResponse(resp));
    assert(resp.success);
    assert(resp.data.size() == 4);
    for (auto b : resp.data) assert(b == 0xBB);

    std::cout << "[Test] testBasicCommandRoundTrip passed.\n";
}

void testResponseDelay() {
    MockCxlHostInterface host;
    MockCxlDeviceInterface device;
    host.attachDevice(&device);
    device.attachHost(&host);

    CxlCommand cmd;
    cmd.type = CxlCommand::Type::Read;
    cmd.address = 0x2000;
    cmd.size = 8;

    auto start = std::chrono::high_resolution_clock::now();
    host.sendCommand(cmd);
    CxlResponse resp;
    bool received = host.receiveResponse(resp);
    auto end = std::chrono::high_resolution_clock::now();

    assert(received);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    assert(duration >= 25); // 10ms (host) + 15ms (device)

    std::cout << "[Test] testResponseDelay passed.\n";
}

void testCommandLogging() {
    std::ostringstream captured_output;
    auto* old_buf = std::cout.rdbuf(captured_output.rdbuf());

    MockCxlHostInterface host;
    MockCxlDeviceInterface device;
    host.attachDevice(&device);
    device.attachHost(&host);

    CxlCommand cmd;
    cmd.type = CxlCommand::Type::Write;
    cmd.address = 0xDEAD;
    cmd.size = 2;
    cmd.data = {0x01, 0x02};

    host.sendCommand(cmd);
    CxlResponse resp;
    host.receiveResponse(resp);

    std::cout.rdbuf(old_buf);
    std::string output = captured_output.str();
    assert(output.find("Command sent") != std::string::npos);
    assert(output.find("Sending response") != std::string::npos);

    std::cout << "[Test] testCommandLogging passed.\n";
}

void testCommandQueueProcessing() {
    MockCxlHostInterface host;
    MockCxlDeviceInterface device;
    host.attachDevice(&device);
    device.attachHost(&host);

    for (int i = 0; i < 3; ++i) {
        CxlCommand cmd;
        cmd.type = CxlCommand::Type::Write;
        cmd.address = 0x1000 + i;
        cmd.size = 1;
        cmd.data = {static_cast<uint8_t>(i)};

        assert(host.sendCommand(cmd));
        CxlResponse resp;
        assert(host.receiveResponse(resp));
        assert(resp.success);
    }

    std::cout << "[Test] testCommandQueueProcessing passed.\n";
}

int main() {
    testBasicCommandRoundTrip();
    testResponseDelay();
    testCommandLogging();
    testCommandQueueProcessing();
    std::cout << "All Host-Device Interface tests passed.\n";
    return 0;
}

// Failed Attempt [Session 18]
// class TestCxlHostDeviceMock {
// public:
//     void setUp() {
//     host.attachDevice(&device);
// }

// void tearDown() {
//     // No dynamic allocation, so nothing to clean up for now.
// }

// void testCommandTransmission() {
//     CxlCommand cmd{CxlCommand::Type::Write, 0x1000, 8};
//     bool sent = host.sendCommand(cmd);
//     assert(sent);
//     const auto& received = device.getReceivedCommands();
//     assert(!received.empty());
//     assert(received.back().address == 0x1000);
// }

// void testResponseDelivery() {
//     CxlResponse resp{true, {0xAA, 0xBB}};
//     device.sendResponse(resp);
//     host.injectResponse(resp);
//     CxlResponse out;
//     bool got = host.receiveResponse(out);
//     assert(got);
//     assert(out.success);
//     assert(out.data == std::vector<uint8_t>({0xAA, 0xBB}));
// }

// void testSequentialCommandHandling() {
//     for (int i = 0; i < 3; ++i) {
//         CxlCommand cmd{CxlCommand::Type::Read, static_cast<uint64_t>(0x2000 + i), 4};
//         host.sendCommand(cmd);
//         CxlResponse resp{true, {static_cast<uint8_t>(i)}};
//         device.sendResponse(resp);
//         host.injectResponse(resp);
//         CxlResponse out;
//         assert(host.receiveResponse(out));
//         assert(out.data[0] == static_cast<uint8_t>(i));
//     }
// }

// void testReadCommandLogged() {
//     CxlCommand cmd{CxlCommand::Type::Read, 0x3000, 16};
//     host.sendCommand(cmd);
//     const auto& received = device.getReceivedCommands();
//     assert(!received.empty());
//     assert(received.back().type == CxlCommand::Type::Read);
// }

// void testHostTimeoutOnNoResponse() {
//     CxlResponse out;
//     bool got = host.receiveResponse(out);
//     assert(!got); // Should fail since no response injected
// }

// void testResponseMatchesCommand() {
//     CxlCommand cmd{CxlCommand::Type::Write, 0x4000, 2};
//     host.sendCommand(cmd);
//     CxlResponse resp{true, {0xDE, 0xAD}};
//     device.sendResponse(resp);
//     host.injectResponse(resp);
//     CxlResponse out;
//     bool got = host.receiveResponse(out);
//     assert(got);
//     assert(out.data == std::vector<uint8_t>({0xDE, 0xAD}));
// }

// private:
//     MockCxlHostInterface host;
//     MockCxlDeviceInterface device;
// };

// int main() {
//     TestCxlHostDeviceMock test;
//     test.setUp();
//     test.testCommandTransmission();
//     test.testResponseDelivery();
//     test.testSequentialCommandHandling();
//     test.testReadCommandLogged();
//     test.testHostTimeoutOnNoResponse();
//     test.testResponseMatchesCommand();
//     test.tearDown();
//     return 0;
// }