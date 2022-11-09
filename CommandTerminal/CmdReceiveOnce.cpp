#include "CmdReceiveOnce.h"
#include "CommandTerminal.h"

CmdReceiveOnce::CmdReceiveOnce() : Command("Receive Once", "AT+RECV",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Receive and display one packet.",
#else
    "",
#endif
    "(string:242) or (hex:242)")
{}

uint32_t CmdReceiveOnce::action(const std::vector<std::string>& args) {
    std::vector<uint8_t> data;

    if (CommandTerminal::Dot()->recv(data) == mDot::MDOT_OK) {
        if (!data.empty()) {
            CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::formatPacketData(data, CommandTerminal::Dot()->getRxOutput()).c_str());
        }
    }

    return 0;
}

bool CmdReceiveOnce::verify(const std::vector<std::string>& args) {
	if (args.size() == 1)
		return true;

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
