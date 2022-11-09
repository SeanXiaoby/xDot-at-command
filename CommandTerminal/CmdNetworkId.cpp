#include "CmdNetworkId.h"

CmdNetworkId::CmdNetworkId() :
#if MTS_CMD_TERM_VERBOSE
    Command("Network ID", "AT+NI", "Configured Network Name/EUI (MSB, App EUI in LoRaWAN) AT+NI=0/2,hex AT+NI=1,network_name  (Net ID = crc64(network_name)) (8 bytes)", "(0/2,(hex:8)),(1,(string:128))")
#else
    Command("AT+NI")
#endif
{
    _queryable = true;
}

uint32_t CmdNetworkId::action(const std::vector<std::string>& args)
{
    char arg1 = '\0';
    if ((args.size() >= 2) && (args[1].size() == 1)) {
        arg1 = args[1][0];
    }

    if ((args.size() == 1) || ((args.size() == 3) && (args[2].size() == 1) && (args[2][0] == '?'))) {
        if (arg1 == '2') {
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getProtectedAppEUI(), "-").c_str());
        } else {
            CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkId(), "-").c_str());

            if (!CommandTerminal::Dot()->getNetworkName().empty())
                CommandTerminal::Serial()->writef("Passphrase: '%s'\r\n", CommandTerminal::Dot()->getNetworkName().c_str());
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

            if (CommandTerminal::Dot()->setNetworkName(text) == mDot::MDOT_OK)
            {
#if MTS_CMD_TERM_VERBOSE
                CommandTerminal::Serial()->writef("Set Network Name: ");
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
            std::vector<uint8_t> NewEUI;
            readByteArray(args[2], NewEUI, EUI_LENGTH);
            if (arg1 != '2') {
                if (CommandTerminal::Dot()->setNetworkId(NewEUI) == mDot::MDOT_OK)
                {
#if MTS_CMD_TERM_VERBOSE
                    CommandTerminal::Serial()->writef("Set Network ID: ");
                    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewEUI, "-").c_str());
#endif
                }
                else
                {
                    return 1;
                }
            } else {
                if (CommandTerminal::Dot()->setProtectedAppEUI(NewEUI) == mDot::MDOT_OK)
                {
#if MTS_CMD_TERM_VERBOSE
                    CommandTerminal::Serial()->writef("Set Protected AppEUI: ");
                    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewEUI, "-").c_str());
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

bool CmdNetworkId::verify(const std::vector<std::string>& args)
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

            if ((arg1 == '0' || arg1 == '2') && args[2] != "?" && !isHexString(args[2], 8)) {
    #if MTS_CMD_TERM_VERBOSE
                CommandTerminal::setErrorMessage("Invalid ID, expects (hex:8)");
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
                    CommandTerminal::setErrorMessage("Invalid name, expects (string:128)");
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
