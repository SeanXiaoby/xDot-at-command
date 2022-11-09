#include "CmdJoinRx2Datarate.h"
#include "CommandTerminal.h"

CmdJoinRx2Datarate::CmdJoinRx2Datarate()
:
  Command("Join Rx2 Datarate", "AT+JR2D",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Join Rx2 datarate",
#else
    "",
#endif
    "(US:DR8-DR13,AU:DR8-DR13,EU:DR0-DR7)") {

}

uint32_t CmdJoinRx2Datarate::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%s\r\n", mDot::DataRateStr(CommandTerminal::Dot()->getJoinRx2DataRate()).c_str());
    }
    else if (args.size() == 2) {
        std::string dr = mts::Text::toUpper(args[1]);

        int datarate = 0;
        uint8_t i;

        int res = sscanf(dr.c_str(), "%d", &datarate);

        if (res == 0) {
            for (i = 0; i < 16; i++) {
                if (mDot::DataRateStr(i).find(dr) != std::string::npos) {
                    datarate = i;
                    break;
                }
            }
        }

        if (CommandTerminal::Dot()->setJoinRx2DataRate(datarate) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
            return 1;
        }
    }

    return 0;
}

bool CmdJoinRx2Datarate::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
