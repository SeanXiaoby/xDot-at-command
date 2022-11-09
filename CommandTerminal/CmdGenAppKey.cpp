#include "CmdGenAppKey.h"

CmdGenAppKey::CmdGenAppKey() :
    Command("Generic App Key", "AT+GK",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Configured generic app key",
#else
    "",
#endif
    "(hex:16)")
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
            CommandTerminal::setErrorMessage("Invalid key, expects (hex:16)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}

