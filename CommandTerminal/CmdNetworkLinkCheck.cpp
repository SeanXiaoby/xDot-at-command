#include "CmdNetworkLinkCheck.h"
#include "CommandTerminal.h"

CmdNetworkLinkCheck::CmdNetworkLinkCheck()
:
  Command("Network Link Check", "AT+NLC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Perform network link check, displays dBm above floor, number of gateways in range and optional packet payload if received",
#else
    "",
#endif
    "(0-254),(1-)") {

}

uint32_t CmdNetworkLinkCheck::action(const std::vector<std::string>& args) {
    mDot::link_check lc;

    lc = CommandTerminal::Dot()->networkLinkCheck();
    if (lc.status) {
        CommandTerminal::Serial()->writef("%u,%lu\r\n", lc.dBm, lc.gateways);

        if (!lc.payload.empty())
            CommandTerminal::Serial()->writef("%s\r\n", CommandTerminal::formatPacketData(lc.payload, CommandTerminal::Dot()->getRxOutput()).c_str());

    } else {
        CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
        return 1;
    }

    return 0;
}

