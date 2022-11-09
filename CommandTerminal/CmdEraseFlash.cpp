#include "CmdEraseFlash.h"

CmdEraseFlash::CmdEraseFlash() :
#if MTS_CMD_TERM_VERBOSE
    Command("Erase File System", "AT+ERASE", "Format file system (1) or erase entire external flash memory (2)", "(1-2)")
#else
    Command("AT+ERASE")
#endif
{
}

uint32_t CmdEraseFlash::action(const std::vector<std::string>& args)
{
    if (args[1][0] == '1') {
        CommandTerminal::Dot()->eraseFileSystem();
    } else {
        CommandTerminal::Dot()->eraseFlash();
    }
    return 0;
}

bool CmdEraseFlash::verify(const std::vector<std::string>& args)
{
    if ((args.size() == 2) && (args[1].length() == 1 && ((args[1][0] == '1') || (args[1][0] == '2')))) {
        return true;
    }
#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid parameter, expects (1-2)");
#endif
    return false;
}
