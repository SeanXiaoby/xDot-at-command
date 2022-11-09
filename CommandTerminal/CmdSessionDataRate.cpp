#include "CmdSessionDataRate.h"

CmdSessionDataRate::CmdSessionDataRate()
: Command("Session Data Rate", "AT+SDR",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Get the current datarate",
#else
    "",
#endif
    "US:(7-10|DR0-DR4|DR8-DR13)) EU:(7-12|DR0-DR7))")
{
    _queryable = true;
}

uint32_t CmdSessionDataRate::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("DR%d - %s\r\n", CommandTerminal::Dot()->getSessionDataRate(), CommandTerminal::Dot()->getDataRateDetails(CommandTerminal::Dot()->getSessionDataRate()).c_str());
    }

    return 0;
}

bool CmdSessionDataRate::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
