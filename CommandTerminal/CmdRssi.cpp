#include "CmdRssi.h"

CmdRssi::CmdRssi() :
        Command("Signal Strength", "AT+RSSI",
#if defined(TARGET_MTS_MDOT_F411RE)
     "Displays signal strength information for received packets: last, min, max, avg in dB",
#else
    "",
#endif
    "(-140-0),(-140-0),(-140-0),(-140-0)")
{
    _queryable = true;
}

uint32_t CmdRssi::action(const std::vector<std::string>& args)
{
    mDot::rssi_stats stats = CommandTerminal::Dot()->getRssiStats();
    CommandTerminal::Serial()->writef("%d, %d, %d, %d\r\n", stats.last, stats.min, stats.max, stats.avg);
    return 0;
}

