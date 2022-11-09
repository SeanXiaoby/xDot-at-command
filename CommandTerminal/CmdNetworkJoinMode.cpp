#include "CmdNetworkJoinMode.h"

CmdNetworkJoinMode::CmdNetworkJoinMode() :
        Command("Network Join Mode", "AT+NJM",
#if defined(TARGET_MTS_MDOT_F411RE)
    "0: Manual configuration, 1: OTA Network Join, 2: Auto OTA Network Join on start up, 3: Peer-to-Peer (default: 1)",
#else
    "",
#endif
    "(0-3)")
{
    _queryable = true;
}

uint32_t CmdNetworkJoinMode::action(const std::vector<std::string>& args)
{
    if (args.size() == 1) {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getJoinMode());
    }
    else if (args.size() == 2)
    {
        uint8_t mode = (args[1] == "1") ? 1 : 0;
        mode = (args[1] == "2" ? 2 : mode);
        mode = (args[1] == "3" ? 3 : mode);

        if (CommandTerminal::Dot()->setJoinMode(mode) != mDot::MDOT_OK)
        {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }

        if (mode == 3) {
            Fota::getInstance()->enable(false);
            CommandTerminal::Dot()->clearMacCommands();
            CommandTerminal::Dot()->setTxDataRate(CommandTerminal::Dot()->getTxDataRate());
        }

    }

    return 0;
}

bool CmdNetworkJoinMode::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (!(args[1] == "0" || args[1] == "1" || args[1] == "2" || args[1] == "3"))
        {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: Manual, 1: OTA, 2: Auto OTA, 3:Peer-to-Peer)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
