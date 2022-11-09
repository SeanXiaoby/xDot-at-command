#include "CmdSaveSession.h"

CmdSaveSession::CmdSaveSession() :
    Command("Save Network Session", "AT+SS",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Save network session info to flash",
#else
    "",
#endif
    "NONE")
{
}

uint32_t CmdSaveSession::action(const std::vector<std::string>& args)
{
    CommandTerminal::Dot()->saveNetworkSession();
    return 0;
}

