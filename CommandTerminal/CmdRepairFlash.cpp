#include "CmdRepairFlash.h"

CmdRepairFlash::CmdRepairFlash() :
#if MTS_CMD_TERM_VERBOSE
    Command("Repair Flash Filesystem", "AT+REPAIR", "Repair file system", "1")
#else
    Command("AT+REPAIR")
#endif
{
    _queryable = false;
}

uint32_t CmdRepairFlash::action(const std::vector<std::string>& args)
{
#if defined(TARGET_MTS_MDOT_F411RE)
    logWarning("Repairing flash, do not reset device, please wait...");
    CommandTerminal::Serial()->writef("Repairing flash, do not reset device, please wait...\r\n");
    CommandTerminal::Dot()->repairFlashFileSystem();
    CommandTerminal::Serial()->writef("Repairs completed\r\n");
#endif
    return 0;
}

bool CmdRepairFlash::verify(const std::vector<std::string>& args)
{
    if (args.size() == 2 && args[1] == "1")
        return true;

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("All files will be erased, please provide parameter of '1' to confirm");
#endif
    return false;
}
