#include "CmdTxDataRate.h"
#include "MTSText.h"
#include "MTSLog.h"


CmdTxDataRate::CmdTxDataRate() :
#if MTS_CMD_TERM_VERBOSE
    Command("Tx Data Rate", "AT+TXDR", "Set the Tx data rate for all channels", "DR0-DR15, Depends on Channel Plan")
#else
    Command("AT+TXDR")
#endif
{
    _queryable = true;
}


uint32_t CmdTxDataRate::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("DR%d - %s\r\n", CommandTerminal::Dot()->getTxDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getTxDataRate()).c_str());
    } else if (args.size() == 2) {
        if (CommandTerminal::Dot()->setTxDataRate(strToDataRate(args[1])) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdTxDataRate::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int dr = strToDataRate(args[1]);
        if ((dr == -1) || !CommandTerminal::Dot()->validateDataRate(dr)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("TX data rate invalid");
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
