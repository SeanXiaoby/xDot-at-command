#include "CmdResetStats.h"

CmdResetStats::CmdResetStats()
:
  Command("Reset Stats", "AT&R",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Reset statistics",
#else
    "",
#endif
    "NONE") {
}

uint32_t CmdResetStats::action(const std::vector<std::string>& args) {

    CommandTerminal::Dot()->resetStats();
    return 0;
}

