#include "CmdPublicNetwork.h"

CmdPublicNetwork::CmdPublicNetwork() :
        Command("Network Mode", "AT+PN",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set public network mode (0: PRIVATE_MTS, 1: PUBLIC_LORAWAN, 2: PRIVATE_LORAWAN)",
#else
    "",
#endif
    "(0-2)")
{
    _queryable = true;
}

uint32_t CmdPublicNetwork::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%d\r\n", CommandTerminal::Dot()->getPublicNetwork());
    }
    else if (args.size() == 2)
    {
        uint8_t mode = lora::PRIVATE_MTS;

        if (args[1] == "1") {
            mode = lora::PUBLIC_LORAWAN;
        } else if (args[1] == "2") {
            mode = lora::PRIVATE_LORAWAN;
        }

        if (CommandTerminal::Dot()->setPublicNetwork(mode) != mDot::MDOT_OK) {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdPublicNetwork::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (args[1] != "2" && args[1] != "1" && args[1] != "0") {
            CommandTerminal::setErrorMessage("Invalid parameter, expects (0: PRIVATE_MTS, 1: PUBLIC_LORAWAN, 2: PRIVATE_LORAWAN)");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
