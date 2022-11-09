#include "CmdGenAppKey.h"

CmdGenAppKey::CmdGenAppKey() :
#if MTS_CMD_TERM_VERBOSE
    Command("Generic App Key", "AT+GK", "Configured generic app key", "(hex:16)")
#else
    Command("AT+GK")
#endif
{
    _queryable = true;
}

uint32_t CmdGenAppKey::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getGenAppKey(), ".").c_str());
    }
    else if (args.size() == 2)
    {
        std::vector<uint8_t> NewKey;
        readByteArray(args[1], NewKey, KEY_LENGTH);

        if (CommandTerminal::Dot()->setGenAppKey(NewKey) == mDot::MDOT_OK)
        {
            CommandTerminal::Serial()->writef("Set Gen App Key: ");
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
        }
        else
        {
            return 1;
        }
    }

    return 0;
}

bool CmdGenAppKey::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {

        if (!isHexString(args[1], 16)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid key, expects (hex:16)");
#endif
            return false;
        }

        return true;
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

