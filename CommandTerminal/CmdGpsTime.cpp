#include "CmdGpsTime.h"

CmdGpsTime::CmdGpsTime()
    : Command("GPS Time", "AT+GPSTIME",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Get the GPS Time in millisecs",
#else
    "",
#endif
    "NONE")
{
    _queryable = true;
}

uint32_t CmdGpsTime::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        uint64_t gps_time = CommandTerminal::Dot()->getGPSTime();
        if (gps_time > 0) {
            CommandTerminal::Serial()->writef("%llu\r\n", gps_time);
        } else {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
            return 1;
        }
    }

    return 0;
}

bool CmdGpsTime::verify(const std::vector<std::string>& args) {
    if (args.size() == 1)
        return true;

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
