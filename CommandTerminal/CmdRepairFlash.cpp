#include "CmdRepairFlash.h"

CmdRepairFlash::CmdRepairFlash() :
        Command("Repair Flash Filesystem", "AT+REPAIR",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Repair file system",
#else
    "",
#endif
    "1")
{
    _queryable = false;
}

uint32_t CmdRepairFlash::action(const std::vector<std::string>& args)
{
#if defined(TARGET_MTS_MDOT_F411RE)
    logWarning("Repairing flash, do not reset device, please wait...");
    CommandTerminal::Serial()->writef("Repairing flash, do not reset device, please wait...\r\n");
    CommandTerminal::Dot()->repairFlashFileSystem();
#endif
    return 0;
}

bool CmdRepairFlash::verify(const std::vector<std::string>& args)
{
    if (args.size() == 2 && args[1] == "1")
        return true;

    CommandTerminal::setErrorMessage("Entire flash will be erased and config files rewritten, please provide parameter of '1' to confirm");
    return false;
}
