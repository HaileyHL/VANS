#ifndef ICXLMEMORY_H
#define ICXLMEMORY_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cstring> // for memcpy
/**
 * @file ICxlMemory.h
 * @brief Defines the ICxlMemory interface and a mock implementation for CXL memory simulation.
 *
 * This file contains the abstract interface ICxlMemory for CXL memory operations,
 * as well as a simple mock implementation (MockCxlMemory) for testing and simulation.
 */
class ICxlMemory {
public:
    virtual ~ICxlMemory() = default;

    virtual bool read(uint64_t address, uint8_t* buffer, size_t size) = 0;
    virtual bool write(uint64_t address, const uint8_t* data, size_t size) = 0;
    virtual uint64_t allocate(size_t size) = 0;
    virtual void deallocate(uint64_t address) = 0;
};


/**
 * @class MockCxlMemory
 * @brief A simple mock implementation of the ICxlMemory interface.
 *
 * This class simulates a memory device by managing memory allocations
 * and read/write operations using an internal unordered_map. Each allocation
 * is assigned a unique address, and memory is represented as a vector of bytes.
 *
 * The class is intended for testing and simulation purposes, providing
 * basic functionality for memory operations without interacting with real hardware.
 *
 * @note This implementation is not thread-safe and is designed for single-threaded use.
 */
class MockCxlMemory : public ICxlMemory {
public:
    MockCxlMemory() : next_free_address(1) {}

    /**
     * @brief Reads data from the specified address into the provided buffer.
     */
    bool read(uint64_t address, uint8_t* buffer, size_t size) override {
        auto it = memory.find(address);
        if (it == memory.end() || it->second.size() < size) {
            return false; // Address not allocated or size too large
        }
        std::memcpy(buffer, it->second.data(), size);
        return true;
    }

    /**
     * @brief Writes data from the provided buffer to the specified address.
     */
    bool write(uint64_t address, const uint8_t* data, size_t size) override {
        auto it = memory.find(address);
        if (it == memory.end() || it->second.size() < size) {
            return false; // Address not allocated or size too large
        }
        std::memcpy(it->second.data(), data, size);
        return true;
    }

    uint64_t allocate(size_t size) override {
        uint64_t addr = next_free_address++;
        memory[addr] = std::vector<uint8_t>(size, 0);
        return addr;
    }

    void deallocate(uint64_t address) override {
        memory.erase(address);
    }

private:
    uint64_t next_free_address;
    std::unordered_map<uint64_t, std::vector<uint8_t>> memory;
};

#endif // ICXLMEMORY_H