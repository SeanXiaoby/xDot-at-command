#include "CmdResetStats.h"

CmdResetStats::CmdResetStats() :
#if MTS_CMD_TERM_VERBOSE
    Command("Reset Stats", "AT&R", "Reset statistics", "NONE")
#else
    Command("AT&R")
#endif
{
}

uint32_t CmdResetStats::action(const std::vector<std::string>& args) {

    CommandTerminal::Dot()->resetStats();
    return 0;
}

