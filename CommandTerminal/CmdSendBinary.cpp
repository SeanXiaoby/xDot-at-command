#include "CmdSendBinary.h"
#include "CommandTerminal.h"

CmdSendBinary::CmdSendBinary() :
#if MTS_CMD_TERM_VERBOSE
    Command("Send Binary", "AT+SENDB", "Sends supplied binary (hex) packet data one time and return response", "(hex:242)")
#else
    Command("AT+SENDB")
#endif
{}

uint32_t CmdSendBinary::action(const std::vector<std::string>& args) {
    std::vector<uint8_t> data;

    int temp;
    uint32_t length = args[1].size();

    // Detach the serial rx irq handler temporarily here to allow for deep sleep
    //CommandTerminal::Serial()->detach();

    // Convert the ASCII hex data to binary...
    for (uint32_t i = 0; i < length; i += 2) {
        sscanf(&args[1][i], "%2x", &temp);
        data.push_back(temp);
    }

    if (CommandTerminal::Dot()->send(data, CommandTerminal::Dot()->getTxWait()) != mDot::MDOT_OK) {
        //CommandTerminal::Serial()->attach();
        return 1;
    }

    if (CommandTerminal::Dot()->getTxWait()) {
        printRecvData();
    }

    //CommandTerminal::Serial()->attach();
    return 0;
}

bool CmdSendBinary::verify(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        if (args[1].size() > 484 || !isHexString(args[1], args[1].size() / 2)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid hex string, (hex:242)");
#endif
            return false;
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

