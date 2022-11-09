#include "CmdRepeat.h"

CmdRepeat::CmdRepeat() :
#if MTS_CMD_TERM_VERBOSE
    Command("Packet Repeats", "AT+REP", "Configure number of times to repeat a packet", "(0-15)")
#else
    Command("AT+REP")
#endif
{
    _queryable = true;
}

uint32_t CmdRepeat::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%u\r\n", CommandTerminal::Dot()->getRepeat());
    }
    else if (args.size() == 2)
    {
        int repeat;
        sscanf(args[1].c_str(), "%d", &repeat);

        if (CommandTerminal::Dot()->setRepeat(repeat) != mDot::MDOT_OK)
        {
            return 1;
        }
    }

    return 0;
}

bool CmdRepeat::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int repeat;
        if (sscanf(args[1].c_str(), "%d", &repeat) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (repeat < 0 || repeat > 15)
        {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid repeats, expects (0-15)");
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
