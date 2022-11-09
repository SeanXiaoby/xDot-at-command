#include "CmdTxDataRate.h"
#include "MTSText.h"
#include "MTSLog.h"


CmdTxDataRate::CmdTxDataRate()
: Command("Tx Data Rate", "AT+TXDR",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the Tx data rate for all channels",
#else
    "",
#endif
    "DR0-DR15, Depends on Channel Plan") {
    _queryable = true;
}

uint32_t CmdTxDataRate::action(const std::vector<std::string>& args) {
    if (args.size() == 1)
        {
        CommandTerminal::Serial()->writef("DR%d - %s\r\n", CommandTerminal::Dot()->getTxDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getTxDataRate()).c_str());
    } else if (args.size() == 2) {
        std::string dr = mts::Text::toUpper(args[1]);

        int datarate = -1;
        uint8_t i;

        int res = sscanf(dr.c_str(), "%d", &datarate);

        if (res == 0) {
            for (i = 0; i < 24; i++) {
                if (mDot::DataRateStr(i).find(dr) != std::string::npos) {
                    datarate = i;
                    break;
                }
            }
        }

        if (CommandTerminal::Dot()->setTxDataRate(datarate) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
            return 1;
        }
    }

    return 0;
}

bool CmdTxDataRate::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
