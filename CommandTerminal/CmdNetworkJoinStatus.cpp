#include "CmdNetworkJoinStatus.h"

CmdNetworkJoinStatus::CmdNetworkJoinStatus() :
    Command("Network Join Status", "AT+NJS",
#if defined(TARGET_MTS_MDOT_F411RE)
    "0: Not joined, 1: Joined",
#else
    "",
#endif
    "(0,1)")
{
    _queryable = true;
}

uint32_t CmdNetworkJoinStatus::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getNetworkJoinStatus());
    return 0;
}


