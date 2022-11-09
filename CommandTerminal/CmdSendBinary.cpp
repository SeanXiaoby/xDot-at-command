#include "CmdSendBinary.h"
#include "CommandTerminal.h"

CmdSendBinary::CmdSendBinary() : Command("Send Binary", "AT+SENDB",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Sends supplied binary (hex) packet data one time and return response",
#else
    "",
#endif
    "(hex:242)"){}

uint32_t CmdSendBinary::action(const std::vector<std::string>& args) {
    std::vector<uint8_t> data;
    std::vector<uint8_t> mt;

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
        CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
        //CommandTerminal::Serial()->attach();
        return 1;
    }

    // clear and release memory
    data.swap(mt);

    if (CommandTerminal::Dot()->getTxWait() && CommandTerminal::Dot()->recv(data) == mDot::MDOT_OK) {
        if (!data.empty()) {
            CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::formatPacketData(data, CommandTerminal::Dot()->getRxOutput()).c_str());
        }
    }

    //CommandTerminal::Serial()->attach();
    return 0;
}

bool CmdSendBinary::verify(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        if (args[1].size() > 484 || !isHexString(args[1], args[1].size() / 2)) {
            CommandTerminal::setErrorMessage("Invalid hex string, (hex:242)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

