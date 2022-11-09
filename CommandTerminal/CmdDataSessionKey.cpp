#include "CmdDataSessionKey.h"

CmdDataSessionKey::CmdDataSessionKey() :
    Command("Data Session Key", "AT+DSK",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Data session encryption key (16 bytes)",
#else
    "",
#endif
    "(hex:16) or (1-8),(hex:16)")
{
    _queryable = true;
}

uint32_t CmdDataSessionKey::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getDataSessionKey(), ".").c_str());
    }
    else if (args.size() == 2 || args.size() == 3)
    {
        int index = 0;
        int arg_index = 1;
        if (args.size() == 3) {
            arg_index = 2;
            sscanf(args[1].c_str(), "%d", &index);
        }

        if (args.size() == 3 && args[2] == "?") {
            uint8_t dsk[16];
            CommandTerminal::Dot()->getMulticastApplicationSessionKey(index-1, dsk);
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(dsk, 16, ".").c_str());
        } else {
            std::vector<uint8_t> NewKey;

            // Read in the key components...
            readByteArray(args[arg_index], NewKey, KEY_LENGTH);

            if (arg_index == 2 && CommandTerminal::Dot()->setMulticastApplicationSessionKey(index-1, &NewKey[0]) == mDot::MDOT_OK) {
                CommandTerminal::Serial()->writef("Set Multicast Application Session Key %d: ", index);
                CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
            } else if (CommandTerminal::Dot()->setDataSessionKey(NewKey) == mDot::MDOT_OK) {
                CommandTerminal::Serial()->writef("Set Data Session Key: ");
                CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
            } else {
                CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
                return 1;
            }
        }
    }

    return 0;
}

bool CmdDataSessionKey::verify(const std::vector<std::string>& args)
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

    if (args.size() == 3) {
        int index = 0;
        sscanf(args[1].c_str(), "%d", &index);

        if (args[1].size() == 1 && (index < 1 || index > 8)) {
            CommandTerminal::setErrorMessage("Invalid index, expects 1-8");
            return false;
        } else if (args[2] != "?" && !isHexString(args[2], 16)) {
            CommandTerminal::setErrorMessage("Invalid key, expects (hex:16)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
