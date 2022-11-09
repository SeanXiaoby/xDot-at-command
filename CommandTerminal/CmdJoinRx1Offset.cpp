#include "CmdJoinRx1Offset.h"
#include "CommandTerminal.h"

CmdJoinRx1Offset::CmdJoinRx1Offset() :
#if MTS_CMD_TERM_VERBOSE
    Command("Join Rx1 DR Offset", "AT+JRXO", "Join Rx1 datarate offset", "(US:0-3,AU:0-3,EU:0-5)")
#else
    Command("AT+JRXO")
#endif
{

}

uint32_t CmdJoinRx1Offset::action(const std::vector<std::string>& args) {

    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getJoinRx1DataRateOffset());
    }
    else if (args.size() == 2) {
        std::string dr = mts::Text::toUpper(args[1]);

        int offset = 0;
        sscanf(dr.c_str(), "%d", &offset);

        if (CommandTerminal::Dot()->setJoinRx1DataRateOffset(offset) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdJoinRx1Offset::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}
