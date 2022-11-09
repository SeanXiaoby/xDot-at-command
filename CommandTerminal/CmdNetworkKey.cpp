#include "CmdNetworkKey.h"

CmdNetworkKey::CmdNetworkKey() :
#if MTS_CMD_TERM_VERBOSE
    Command("Network Key", "AT+NK", "Configured network key/passphrase (App Key in LoRaWAN) ##  AT+NK=0/2,hex  AT+NK=1,passphrase (Net key = cmac(passphrase)) (16 bytes)", "(0/2,(hex:16)),(1,(string:128))")
#else
    Command("AT+NK")
#endif
{
    _queryable = true;
}

uint32_t CmdNetworkKey::action(const std::vector<std::string>& args)
{
    char arg1 = '\0';
    if ((args.size() >= 2) && (args[1].size() == 1)) {
        arg1 = args[1][0];
    }

    if ((args.size() == 1) || ((args.size() == 3) && (args[2].size() == 1) && (args[2][0] == '?'))) {
        if (arg1 == '2') {
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getProtectedAppKey(), ".").c_str());
        } else {
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkKey(), ".").c_str());
            if (!CommandTerminal::Dot()->getNetworkPassphrase().empty())
                CommandTerminal::Serial()->writef("Passphrase: '%s'\r\n", CommandTerminal::Dot()->getNetworkPassphrase().c_str());
        }
    } else if (args.size() >= 3) {
        if (arg1 == '1')
        {
            std::string text = args[2];
            // passphrase was split on commas
            for (size_t i = 3; i < args.size(); i++)
            {
                text.append(",");
                text.append(args[i]);
            }

            if (CommandTerminal::Dot()->setNetworkPassphrase(text) == mDot::MDOT_OK)
            {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::Serial()->writef("Set Network Passphrase: ");
                CommandTerminal::Serial()->writef("%s\r\n", text.c_str());
#endif
            }
            else
            {
                return 1;
            }
        }
        else
        {
            std::vector<uint8_t> NewKey;
            readByteArray(args[2], NewKey, KEY_LENGTH);
            if (arg1 != '2') {
                if (CommandTerminal::Dot()->setNetworkKey(NewKey) == mDot::MDOT_OK)
                {
#if MTS_CMD_TERM_VERBOSE
                    CommandTerminal::Serial()->writef("Set Network Key: ");
                    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
#endif
                }
                else
                {
                    return 1;
                }
            } else {
                if (CommandTerminal::Dot()->setProtectedAppKey(NewKey) == mDot::MDOT_OK)
                {
#if MTS_CMD_TERM_VERBOSE
                    CommandTerminal::Serial()->writef("Set Protected AppKey: ");
                    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
#endif
                }
                else
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

bool CmdNetworkKey::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;
    if (args.size() > 1 && args[1].size() == 1) {

        char arg1 = args[1][0];

        if (args.size() == 2 && arg1 == '2')
            return true;

        if (args.size() >= 3) {
            if (arg1 != '0' && arg1 != '1' && arg1 != '2') {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid type, expects (0,1,2)");
#endif
                return false;
            }

            if ((arg1 == '0' || arg1 == '2') && args[2] != "?" && !isHexString(args[2], KEY_LENGTH)) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid key, expects (hex:16)");
#endif
                return false;
            } else {
                if (args[2].size() < 8) {
                    if (!((args[2].size() == 1) && (args[2][0] == '?'))) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid name, expects minimum 8 characters");
#endif
                        return false;
                    }
                } else if (args[2].size() > 128) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid passphrase, expects (string:8-128)");
#endif
                    return false;
                }
            }

            return true;
        }
    }

#if MTS_CMD_TERM_VERBOSE
    CommandTerminal::setErrorMessage("Invalid arguments");
#endif
    return false;
}

