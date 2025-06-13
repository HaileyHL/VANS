#ifndef CXL_COMMAND_PROCESSOR_H
#define CXL_COMMAND_PROCESSOR_H

#include "CxlTypes.h"
#include "CxlProtocolHandlerImpl.h"
#include "cxl_vans_interfaces.h"

class CxlCommandProcessor {
public:
    CxlCommandProcessor(CxlMemory* memory, CxlProtocolHandler* protocol);

    void initialize();  // Registers the dispatcher callback with the protocol handler

private:
    void handleCommand(const CxlCommand& cmd);  // Translates and processes commands

    CxlMemory* memory_;
    CxlProtocolHandler* protocol_;
    CxlProtocolHandler::ResponseCallback response_callback_;
};

#endif // CXL_COMMAND_PROCESSOR_H
