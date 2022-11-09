#include "CmdRestoreSession.h"

CmdRestoreSession::CmdRestoreSession() :
    Command("Restore Network Session", "AT+RS",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Restore network session info from flash",
#else
    "",
#endif
    "NONE")
{
}

uint32_t CmdRestoreSession::action(const std::vector<std::string>& args)
{
    CommandTerminal::Dot()->restoreNetworkSession();
    return 0;
}

