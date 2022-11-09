#include "CmdRxDataRate.h"

CmdRxDataRate::CmdRxDataRate()
:
  Command("Rx Data Rate", "AT+RXDR",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set the Rx datarate, testing purposes only",
#else
    "",
#endif
    "DR0-DR15, Depends on Channel Plan")

{
    _queryable = true;
}

uint32_t CmdRxDataRate::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("DR%d - %s\r\n", CommandTerminal::Dot()->getRxDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getRxDataRate()).c_str());
    }
    else if (args.size() == 2) {
        std::string dr = mts::Text::toUpper(args[1]);

        int datarate = 0;
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

        if (CommandTerminal::Dot()->setRxDataRate(datarate) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdRxDataRate::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
