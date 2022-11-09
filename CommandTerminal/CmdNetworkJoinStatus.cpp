#include "CmdNetworkJoinStatus.h"

CmdNetworkJoinStatus::CmdNetworkJoinStatus() :
#if MTS_CMD_TERM_VERBOSE
    Command("Network Join Status", "AT+NJS", "0: Not joined, 1: Joined", "(0,1)")
#else
    Command("AT+NJS")
#endif
{
    _queryable = true;
}

uint32_t CmdNetworkJoinStatus::action(const std::vector<std::string>& args)
{
    CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getNetworkJoinStatus());
    return 0;
}


