#include "CmdLinkCheckThreshold.h"

CmdLinkCheckThreshold::CmdLinkCheckThreshold() :
#if MTS_CMD_TERM_VERBOSE
    Command("Link Check Threshold", "AT+LCT", "Set threshold for number of link check or ACK failures to tolerate, (0: off, N: number of failures)", "(0-255)")
#else
    Command("AT+LCT")
#endif
{
    _queryable = true;
}

uint32_t CmdLinkCheckThreshold::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getLinkCheckThreshold());
    }
    else if (args.size() == 2)
    {
        int count;
        sscanf(args[1].c_str(), "%d", &count);

        if (CommandTerminal::Dot()->setLinkCheckThreshold(count) != mDot::MDOT_OK)
        {
            return 1;
        }
    }

    return 0;
}

bool CmdLinkCheckThreshold::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int count;
        sscanf(args[1].c_str(), "%d", &count);

        if (count < 0 || count > 255) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid count, expects (0-255))");
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
