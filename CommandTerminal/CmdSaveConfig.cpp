#include "CmdSaveConfig.h"

CmdSaveConfig::CmdSaveConfig() : Command("Save Configuration", "AT&W",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Save configuration to flash memory",
#else
    "",
#endif
    "NONE")
{
}


uint32_t CmdSaveConfig::action(const std::vector<std::string>& args)
{
    if (!CommandTerminal::Dot()->saveConfig()) {
      CommandTerminal::setErrorMessage("Failed to save to flash");
      return 1;
    }

    return 0;
}
