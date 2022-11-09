#include "CmdRxDataRate.h"

CmdRxDataRate::CmdRxDataRate() :
#if MTS_CMD_TERM_VERBOSE
    Command("Rx Data Rate", "AT+RXDR", "Set the Rx datarate, testing purposes only", "DR0-DR15, Depends on Channel Plan")
#else
    Command("AT+RXDR")
#endif
{
    _queryable = true;
}


uint32_t CmdRxDataRate::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("DR%d - %s\r\n", CommandTerminal::Dot()->getRxDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getRxDataRate()).c_str());
    }
    else if (args.size() == 2) {
        if (CommandTerminal::Dot()->setRxDataRate(strToDataRate(args[1])) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdRxDataRate::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int dr = strToDataRate(args[1]);
        if ((dr == -1) || !CommandTerminal::Dot()->validateDataRate(dr)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("RX data rate invalid");
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
