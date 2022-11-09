#include "CmdNetworkId.h"

CmdNetworkId::CmdNetworkId() :
    Command("Network ID", "AT+NI",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Configured Network Name/EUI (MSB, App EUI in LoRaWAN) AT+NI=0/2,hex AT+NI=1,network_name  (Net ID = crc64(network_name)) (8 bytes)",
#else
    "",
#endif
    "(0/2,(hex:8)),(1,(string:128))")
{
    _queryable = true;
}

uint32_t CmdNetworkId::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getNetworkId(), "-").c_str());

        if (!CommandTerminal::Dot()->getNetworkName().empty())
            CommandTerminal::Serial()->writef("Passphrase: '%s'\r\n", CommandTerminal::Dot()->getNetworkName().c_str());
    }
    else if ((args.size() == 2 || (args.size() == 3 && args[2].find("?") == 0)) && args[1].find("2") == 0 && args[1].size() == 1) {
        CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(CommandTerminal::Dot()->getProtectedAppEUI(), "-").c_str());
    }
    else
    {
        if (args[1].find("1") == 0 && args[1].size() == 1)
        {
            std::string text;
            if (args.size() > 3)
            {
                // passphrase was split on commas
                for (size_t i = 2; i < args.size(); i++)
                {
                    text.append(args[i]);
                    if (i < args.size() - 1)
                        text.append(",");
                }
            }
            else
            {
                text = args[2];
            }

            if (CommandTerminal::Dot()->setNetworkName(text) == mDot::MDOT_OK)
            {
                CommandTerminal::Serial()->writef("Set Network Name: ");
                CommandTerminal::Serial()->writef("%s\r\n", text.c_str());
            }
            else
            {
                CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
                return 1;
            }
        }
        else
        {
            std::vector<uint8_t> NewEUI;
            readByteArray(args[2], NewEUI, EUI_LENGTH);
            if (args[1] != "2") {
                if (CommandTerminal::Dot()->setNetworkId(NewEUI) == mDot::MDOT_OK)
                {
                    CommandTerminal::Serial()->writef("Set Network ID: ");
                    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewEUI, "-").c_str());
                }
                else
                {
                    CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
                    return 1;
                }
            } else {
                if (CommandTerminal::Dot()->setProtectedAppEUI(NewEUI) == mDot::MDOT_OK)
                {
                    CommandTerminal::Serial()->writef("Set Protected AppEUI: ");
                    CommandTerminal::Serial()->writef("%s\r\n", mts::Text::bin2hexString(NewEUI, "-").c_str());
                }
                else
                {
                    CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());
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

    if (args.size() == 2 && args[1] == "2")
        return true;

    if (args.size() == 3) {
        if (args[1] != "0" && args[1] != "1" && args[1] != "2") {
            CommandTerminal::setErrorMessage("Invalid type, expects (0,1,2)");
            return false;
        }
        if ((args[1] == "0" || args[1] == "2") && args[2] != "?" && !isHexString(args[2], 8)) {
            CommandTerminal::setErrorMessage("Invalid ID, expects (hex:8)");
            return false;
        }

        if (args[1] == "1" && args[2].size() < 8) {
            CommandTerminal::setErrorMessage("Invalid name, expects minimum 8 characters");
            return false;
        }

        if (args[1] == "1" && args[2].size() > 128) {
            CommandTerminal::setErrorMessage("Invalid name, expects (string:128)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
