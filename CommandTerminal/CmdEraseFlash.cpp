#include "CmdEraseFlash.h"

CmdEraseFlash::CmdEraseFlash() :
        Command("Erase Entire Flash ", "AT+ERASE",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Erase all configurations saved in flash memory",
#else
    "",
#endif
    "1")
{
}

uint32_t CmdEraseFlash::action(const std::vector<std::string>& args)
{
    CommandTerminal::Dot()->eraseFlash();
    return 0;
}

bool CmdEraseFlash::verify(const std::vector<std::string>& args)
{
    if (args.size() == 2 && args[1] == "1")
        return true;

    CommandTerminal::setErrorMessage("Entire flash will be erased, please provide parameter of '1'");
    return false;
}
