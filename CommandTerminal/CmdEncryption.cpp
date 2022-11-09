#include "CmdEncryption.h"

CmdEncryption::CmdEncryption() :
        Command("AES Encryption", "AT+ENC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Enable/disable AES encryption (0: off, 1: on)",
#else
    "",
#endif
        "(0,1)")
{
    _queryable = true;
}

uint32_t CmdEncryption::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getAesEncryption());
    }
    else if (args.size() == 2)
    {
        bool enable = (args[1] == "1");

        if (CommandTerminal::Dot()->setAesEncryption(enable) != mDot::MDOT_OK)
        {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdEncryption::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (args[1] != "1" && args[1] != "0") {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
