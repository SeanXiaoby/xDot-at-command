#include "CmdLinkCheckCount.h"

CmdLinkCheckCount::CmdLinkCheckCount() :
        Command("Link Check Count", "AT+LCC",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Set number of packets between each link check if ACK's are disabled",
#else
    "",
#endif
    "(0:off,N:Packets (max 255))")
{
    _queryable = true;
}

uint32_t CmdLinkCheckCount::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
             CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getLinkCheckCount());
    }
    else if (args.size() == 2)
    {
        int count;
        sscanf(args[1].c_str(), "%d", &count);

        if (CommandTerminal::Dot()->setLinkCheckCount(count) != mDot::MDOT_OK)
        {
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
            return 1;
        }
    }

    return 0;
}

bool CmdLinkCheckCount::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int count;
        if (sscanf(args[1].c_str(), "%d", &count) != 1) {
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (count < 0 || count > 255) {
            CommandTerminal::setErrorMessage("Invalid count, expects (0:off,N:Packets (max 255))");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
