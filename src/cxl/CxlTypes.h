#ifndef CXLTYPES_H
#define CXLTYPES_H

#include <cstdint>
#include <vector>


struct CxlCommand {
    enum class Type { Read, Write, Discover, Invalidate, Flush, Allocate, Deallocate };
    Type type;
    uint64_t address;
    size_t size;
    std::vector<uint8_t> data;
};

struct CxlResponse {
    bool success;
    std::vector<uint8_t> data;
};

#endif // CXLTYPES_H
