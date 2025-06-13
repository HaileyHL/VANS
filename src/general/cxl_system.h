#ifndef VANS_CXL_SYSTEM_H
#define VANS_CXL_SYSTEM_H

#include "component.h"
#include "common.h"
#include "../src/cxl/CxlTypes.h"
#include "../src/cxl/CxlProtocolHandlerImpl.h"
#include "../src/cxl/mocks/MockCxlMemory.h"

namespace vans::cxl_system {

class cxl_memory {
public:
    std::unique_ptr<CxlMemory> backend;
    std::unique_ptr<CxlProtocolHandlerImpl> protocol;

    explicit cxl_memory(const config& cfg) {
        backend = std::make_unique<MockCxlMemory>();
        protocol = std::make_unique<CxlProtocolHandlerImpl>(backend.get());
    }

    base_response issue_request(base_request& req) {
        CxlCommand cmd;
        cmd.type = (req.type == base_request_type::read)
                     ? CxlCommand::Type::Read
                     : CxlCommand::Type::Write;

        cmd.address = req.addr;
        cmd.size = 64;  // placeholder

        protocol->issueCommand(cmd);
        return {true, true, 0};
    }

    void tick(clk_t) {}
    bool full() { return false; }
    bool pending() { return false; }
    void drain() {}
};

class cxl_system_controller {
public:
    std::shared_ptr<cxl_memory> mem;

    explicit cxl_system_controller(const config& cfg) {
        mem = std::make_shared<cxl_memory>(cfg);
    }

    base_response issue_request(base_request& req) {
        return mem->issue_request(req);
    }

    void tick(clk_t) {}
    bool full() { return false; }
    bool pending() { return false; }
    void drain() {}
    void print_counters() {}

    std::vector<std::shared_ptr<base_component>> next_level_components;
    std::shared_ptr<dumper> counter_dumper;
};

class cxl_system : public component<cxl_system_controller, cxl_memory> {
public:
    explicit cxl_system(const config& cfg) {
        this->memory_component = std::make_shared<cxl_memory>(cfg);
        this->ctrl = std::make_shared<cxl_system_controller>(cfg);
    }

    base_response issue_request(base_request& req) override {
        return this->ctrl->issue_request(req);
    }
};

} // namespace vans::cxl_system

#endif // VANS_CXL_SYSTEM_H
