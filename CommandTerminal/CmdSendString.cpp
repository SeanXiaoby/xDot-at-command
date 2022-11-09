#include "CmdSendString.h"
#include "CommandTerminal.h"

CmdSendString::CmdSendString() :
#if MTS_CMD_TERM_VERBOSE
    Command("Send Once", "AT+SEND", "Sends supplied packet data one time and return response, (max:242 bytes)", "(string:242)")
#else
    Command("AT+SEND")
#endif
{}

uint32_t CmdSendString::action(const std::vector<std::string>& args) {
    // Detach the serial rx irq handler temporarily here to allow for deep sleep
    //CommandTerminal::Serial()->detach();

    // Argument had been split on each comma, rebuild payload
    std::string* text = new std::string();
    for (size_t i = 1; i < args.size(); i++) {
        text->append(args[i]);
        if (i != args.size() - 1)
            text->append(",");
    }

    std::vector<uint8_t>* data = new std::vector<uint8_t>(text->begin(), text->end());
    delete text;

    if (CommandTerminal::Dot()->send(*data, CommandTerminal::Dot()->getTxWait()) != mDot::MDOT_OK) {
        //CommandTerminal::Serial()->attach();
        delete data;
        return 1;
    } else if (CommandTerminal::Dot()->getTxWait()) {
        printRecvData();
    }

    //CommandTerminal::Serial()->attach();
    delete data;
    return 0;
}

bool CmdSendString::verify(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        // allow sending empty packet to retrieve downstream messages
        return true;
    }

    if (args.size() >= 2) {
        size_t size = 0;
        for (size_t i = 1; i < args.size() - 1; i++)
            size += args[i].size() + 1;
        if (size > 242) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid packet, expects (string:242)");
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
