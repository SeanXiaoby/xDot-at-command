#include "CmdSessionDataRate.h"

CmdSessionDataRate::CmdSessionDataRate() :
#if MTS_CMD_TERM_VERBOSE
    Command("Session Data Rate", "AT+SDR", "Get the current datarate", "US:(7-10|DR0-DR4|DR8-DR13)) EU:(7-12|DR0-DR7))")
#else
    Command("AT+SDR")
#endif
{
    _queryable = true;
}

uint32_t CmdSessionDataRate::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("DR%d - %s\r\n", CommandTerminal::Dot()->getSessionDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getSessionDataRate()).c_str());
    }

    return 0;
}
