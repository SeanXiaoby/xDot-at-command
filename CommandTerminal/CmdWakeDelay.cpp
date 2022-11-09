#include "CmdWakeDelay.h"

CmdWakeDelay::CmdWakeDelay() :
#if MTS_CMD_TERM_VERBOSE
    Command("Wake Delay", "AT+WD", "Time to wait for data after wakeup signal  (milliseconds)", "(2-2147483646) ms")
#else
    Command("AT+WD")
#endif
{
    _queryable = true;
}

uint32_t CmdWakeDelay::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getWakeDelay());
    }
    else if (args.size() == 2)
    {
        int delay;
        sscanf(args[1].c_str(), "%d", &delay);

        if (CommandTerminal::Dot()->setWakeDelay(delay) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdWakeDelay::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int delay;
        if (sscanf(args[1].c_str(), "%d", &delay) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (delay < 2 || delay > INT_MAX-1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid delay, expects (2-2147483646) ms");
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
