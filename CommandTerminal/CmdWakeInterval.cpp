#include "CmdWakeInterval.h"

CmdWakeInterval::CmdWakeInterval() :
#if MTS_CMD_TERM_VERBOSE
    Command("Wake Interval", "AT+WI", "Wakeup interval (seconds)", "(2-2147483646) s")
#else
    Command("AT+WI")
#endif
{
    _queryable = true;
}

uint32_t CmdWakeInterval::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getWakeInterval());
    }
    else if (args.size() == 2)
    {
        int timeout;
        sscanf(args[1].c_str(), "%d", &timeout);

        if (CommandTerminal::Dot()->setWakeInterval(timeout) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdWakeInterval::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        int timeout;
        if (sscanf(args[1].c_str(), "%d", &timeout) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (timeout < 2 || timeout > INT_MAX-1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid interval, expects (2-2147483646) s");
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
