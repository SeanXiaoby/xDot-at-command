#include "CmdWriteProtectedConfig.h"

CmdWriteProtectedConfig::CmdWriteProtectedConfig() :
    Command("Write Protected Config", "AT&WP",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Write protected config to flash (DevEUI, AppEUI, AppKey, Frequency Band)",
#else
    "",
#endif
    "NONE")
{
}

uint32_t CmdWriteProtectedConfig::action(const std::vector<std::string>& args)
{
    if (!CommandTerminal::Dot()->saveProtectedConfig()) {
      CommandTerminal::setErrorMessage("Failed to save to flash");
      return 1;
    }

    return 0;
}
