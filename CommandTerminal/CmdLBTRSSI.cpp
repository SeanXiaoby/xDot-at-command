#include "CmdLBTRSSI.h"

CmdLBTRSSI::CmdLBTRSSI() :
#if MTS_CMD_TERM_VERBOSE
    Command("LBT RSSI", "AT+LBTRSSI", "READ LBT RSSI", "(-128-0)")
#else
    Command("AT+LBTRSSI")
#endif
{
    _queryable = true;
}

uint32_t CmdLBTRSSI::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->lbtRssi());
    return 0;
}
