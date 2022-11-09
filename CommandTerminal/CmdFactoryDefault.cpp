#include "CmdFactoryDefault.h"

CmdFactoryDefault::CmdFactoryDefault() : Command("Reset Factory Defaults", "AT&F",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Reset current configuration to factory defaults",
#else
    "",
#endif
    "NONE")
{
}


uint32_t CmdFactoryDefault::action(const std::vector<std::string>& args)
{
    CommandTerminal::Dot()->resetConfig();
    CommandTerminal::Dot()->resetNetworkSession();
    return 0;
}

