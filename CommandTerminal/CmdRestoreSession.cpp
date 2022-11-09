#include "CmdRestoreSession.h"

CmdRestoreSession::CmdRestoreSession() :
#if MTS_CMD_TERM_VERBOSE
    Command("Restore Network Session", "AT+RS", "Restore network session info from flash", "NONE")
#else
    Command("AT+RS")
#endif
{
}

uint32_t CmdRestoreSession::action(const std::vector<std::string>& args)
{
    CommandTerminal::Dot()->restoreNetworkSession();
    return 0;
}

