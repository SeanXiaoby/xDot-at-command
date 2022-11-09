#include "CmdWakeTimeout.h"

CmdWakeTimeout::CmdWakeTimeout() :
#if MTS_CMD_TERM_VERBOSE
    Command("Wake Timeout", "AT+WTO", "Read serial data until timeout (milliseconds)", "(0-65000) ms")
#else
    Command("AT+WTO")
#endif
{
    _queryable = true;
}

uint32_t CmdWakeTimeout::action(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        CommandTerminal::Serial()->writef("%lu\r\n", CommandTerminal::Dot()->getWakeTimeout());
    }
    else if (args.size() == 2)
    {
        int timeout;
        sscanf(args[1].c_str(), "%d", &timeout);

        if (CommandTerminal::Dot()->setWakeTimeout(timeout) != mDot::MDOT_OK) {
            return 1;
        }
    }

    return 0;
}

bool CmdWakeTimeout::verify(const std::vector<std::string>& args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        int timeout;
        if (sscanf(args[1].c_str(), "%d", &timeout) != 1) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid argument");
#endif
            return false;
        }

        if (timeout < 0 || timeout > 65000) {
#if MTS_CMD_TERM_VERBOSE
            CommandTerminal::setErrorMessage("Invalid timeout, expects (0-65000) ms");
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
