#include "CmdLBTRSSI.h"

CmdLBTRSSI::CmdLBTRSSI()
:
  Command("LBT RSSI", "AT+LBTRSSI",
#if defined(TARGET_MTS_MDOT_F411RE)
    "READ LBT RSSI",
#else
    "",
#endif
    "(-128-0)")
{
    _queryable = true;
}

uint32_t CmdLBTRSSI::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->lbtRssi());
    return 0;
}

bool CmdLBTRSSI::verify(const std::vector<std::string>& args)
{
    return true;
}
