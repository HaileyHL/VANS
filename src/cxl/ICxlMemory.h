#ifndef ICXLMEMORY_H
#define ICXLMEMORY_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cstring> // for memcpy

class ICxlMemory {
public:
    virtual ~ICxlMemory() = default;

    virtual bool read(uint64_t address, uint8_t* buffer, size_t size) = 0;
    virtual bool write(uint64_t address, const uint8_t* data, size_t size) = 0;
    virtual uint64_t allocate(size_t size) = 0;
    virtual void deallocate(uint64_t address) = 0;
};

// Simple mock implementation of ICxlMemory
class MockCxlMemory : public ICxlMemory {
public:
    MockCxlMemory() : next_free_address(1) {}

    bool read(uint64_t address, uint8_t* buffer, size_t size) override {
        auto it = memory.find(address);
        if (it == memory.end() || it->second.size() < size) {
            return false; // Address not allocated or size too large
        }
        std::memcpy(buffer, it->second.data(), size);
        return true;
    }

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