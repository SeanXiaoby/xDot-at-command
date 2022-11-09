#include "CmdFactoryDefault.h"

CmdFactoryDefault::CmdFactoryDefault() :
#if MTS_CMD_TERM_VERBOSE
    Command("Reset Factory Defaults", "AT&F", "Reset current configuration to factory defaults", "NONE")
#else
    Command("AT&F")
#endif
{
}


uint32_t CmdFactoryDefault::action(const std::vector<std::string>& args)
{
    CommandTerminal::Dot()->resetConfig();
    CommandTerminal::Dot()->resetNetworkSession();
    return 0;
}

