#include "CmdLinkCheckCount.h"

CmdLinkCheckCount::CmdLinkCheckCount() :
#if MTS_CMD_TERM_VERBOSE
    Command("Link Check Count", "AT+LCC", "Set number of packets between each link check if ACK's are disabled", "(0:off,N:Packets (max 255))")
#else
    Command("AT+LCC")
#endif
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
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (count < 0 || count > 255) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid count, expects (0:off,N:Packets (max 255))");
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
