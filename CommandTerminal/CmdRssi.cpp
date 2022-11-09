#include "CmdRssi.h"

CmdRssi::CmdRssi() :
#if MTS_CMD_TERM_VERBOSE
    Command("Signal Strength", "AT+RSSI", "Displays signal strength information for received packets: last, min, max, avg in dB", "(-140-0),(-140-0),(-140-0),(-140-0)")
#else
    Command("AT+RSSI")
#endif
{
    _queryable = true;
}

uint32_t CmdRssi::action(const std::vector<std::string>& args)
{
    mDot::rssi_stats stats = CommandTerminal::Dot()->getRssiStats();
    if (stats.last == lora::INVALID_RSSI) {
#if MTS_CMD_TERM_VERBOSE
        CommandTerminal::Serial()->writef("No data\r\n");
#endif
        return 1;
    }
    CommandTerminal::Serial()->writef("%d, %d, %d, %d\r\n", stats.last, stats.min, stats.max, stats.avg);
    return 0;
}

