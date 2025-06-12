#ifndef CXL_COMMAND_PROCESSOR_H
#define CXL_COMMAND_PROCESSOR_H

#include "CxlTypes.h"
#include "ICxlProtocolHandler.h"
#include "ICxlMemory.h" // For ICxlMemory

class CxlCommandProcessor {
public:
    CxlCommandProcessor(ICxlMemory* memory, ICxlProtocolHandler* protocol);

    void initialize();  // Registers the dispatcher callback with the protocol handler

private:
    void handleCommand(const CxlCommand& cmd);  // Translates and processes commands

    ICxlMemory* memory_;
    ICxlProtocolHandler* protocol_;
    ICxlProtocolHandler::ResponseCallback response_callback_;
};

#endif // CXL_COMMAND_PROCESSOR_H
