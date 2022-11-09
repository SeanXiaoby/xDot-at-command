#include "CmdWakeInterval.h"

CmdWakeInterval::CmdWakeInterval() :
 Command("Wake Interval", "AT+WI",
#if defined(TARGET_MTS_MDOT_F411RE)
    "Wakeup interval (seconds)",
#else
    "",
#endif
    "(2-2147483646) s") {
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
            CommandTerminal::setErrorMessage(CommandTerminal::Dot()->getLastError());;
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
            CommandTerminal::setErrorMessage("Invalid argument");
            return false;
        }

        if (timeout < 2 || timeout > INT_MAX-1) {
            CommandTerminal::setErrorMessage("Invalid interval, expects (2-2147483646) s");
            return false;
        }

        return true;
    }

    CommandTerminal::setErrorMessage("Invalid arguments");
    return false;
}
