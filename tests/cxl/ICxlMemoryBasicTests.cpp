#include "../src/cxl/ICxlMemory.h"
#include <cassert>
#include <cstring>
#include <iostream>

void testAllocateAndDeallocate() {
    MockCxlMemory mem;

    uint64_t addr1 = mem.allocate(64);
    uint64_t addr2 = mem.allocate(128);

    assert(addr1 != addr2 && "Allocated addresses should be unique");

    mem.deallocate(addr1);
    mem.deallocate(addr2);

    // After deallocation, read/write should fail
    uint8_t buffer[10];
    bool read_result = mem.read(addr1, buffer, sizeof(buffer));
    assert(!read_result && "Read should fail after deallocation");

    bool write_result = mem.write(addr2, buffer, sizeof(buffer));
    assert(!write_result && "Write should fail after deallocation");

    std::cout << "testAllocateAndDeallocate passed.\n";
}

void testReadWrite() {
    MockCxlMemory mem;

    uint64_t addr = mem.allocate(16);
    uint8_t write_data[16];
    for (int i = 0; i < 16; ++i) {
        write_data[i] = static_cast<uint8_t>(i);
    }

    bool write_result = mem.write(addr, write_data, sizeof(write_data));
    assert(write_result && "Write should succeed on allocated memory");

    uint8_t read_data[16] = {0};
    bool read_result = mem.read(addr, read_data, sizeof(read_data));
    assert(read_result && "Read should succeed on allocated memory");

    assert(std::memcmp(write_data, read_data, sizeof(write_data)) == 0 && "Read data should match written data");

    std::cout << "testReadWrite passed.\n";
}

void testReadWriteBoundary() {
    MockCxlMemory mem;

    uint64_t addr = mem.allocate(8);
    uint8_t write_data[10] = {0}; // larger than allocated size

    // Write larger than allocated size should fail
    bool write_result = mem.write(addr, write_data, sizeof(write_data));
    assert(!write_result && "Write beyond allocated size should fail");

    uint8_t read_data[10];
    // Read larger than allocated size should fail
    bool read_result = mem.read(addr, read_data, sizeof(read_data));
    assert(!read_result && "Read beyond allocated size should fail");

    std::cout << "testReadWriteBoundary passed.\n";
}

int main() {
    testAllocateAndDeallocate();
    testReadWrite();
    testReadWriteBoundary();

    std::cout << "All ICxlMemory basic tests passed.\n";
    return 0;
}