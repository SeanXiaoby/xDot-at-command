#include "CmdGpsTime.h"

CmdGpsTime::CmdGpsTime() :
#if MTS_CMD_TERM_VERBOSE
    Command("GPS Time", "AT+GPSTIME", "Get the GPS Time in millisecs", "NONE")
#else
    Command("AT+GPSTIME")
#endif
{
    _queryable = true;
}

uint32_t CmdGpsTime::action(const std::vector<std::string>& args) {
    if (args.size() == 1) {
        uint64_t gps_time = CommandTerminal::Dot()->getGPSTime();
        if (gps_time > 0) {
            CommandTerminal::Serial()->writef("%llu\r\n", gps_time);
        } else {
            return 1;
        }
    }

    return 0;
}
